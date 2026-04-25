#include "GestorTorneo.h"
#include "Constants.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#ifdef __linux__
#include <unistd.h>
#include <fstream>
#endif

GestorTorneo::GestorTorneo() : contadorIteraciones(0) {
    srand(time(nullptr));
    for (int i = 0; i < 12; i++) {
        grupos[i] = nullptr;
    }
}

GestorTorneo::~GestorTorneo() {
    for (int i = 0; i < 12; i++) {
        if (grupos[i]) {
            delete grupos[i];
            grupos[i] = nullptr;
        }
    }
}

void GestorTorneo::leerCSV(const char* archivo) {
    FILE* f = fopen(archivo, "r");
    if (!f) {
        cout << "Error: No se pudo abrir el archivo " << archivo << endl;
        return;
    }

    char linea[512];
    fgets(linea, 512, f);

    while (fgets(linea, 512, f)) {
        incrementarIteraciones();

        char* token = strtok(linea, ",");
        if (!token) continue;

        Equipo equipo;
        equipo.setPais(token);

        token = strtok(nullptr, ",");
        if (token) equipo.setConfederacion(token);

        token = strtok(nullptr, ",");
        if (token) equipo.setDirectorTecnico(token);

        token = strtok(nullptr, ",");
        if (token) equipo.setRanking(atoi(token));

        token = strtok(nullptr, ",");
        int golesF = token ? atoi(token) : 0;

        token = strtok(nullptr, ",");
        int golesC = token ? atoi(token) : 0;

        equipo.setGolesHistoricos(golesF, golesC);
        equipos.push_back(equipo);
    }

    fclose(f);
    cout << "Cargados " << equipos.size() << " equipos." << endl;
}

void GestorTorneo::distribuirGolesJugadores() {
    for (int i = 0; i < equipos.size(); i++) {
        int golesEquipo = equipos[i].getGolesAFavor();
        equipos[i].generarJugadoresSinteticos(golesEquipo);
        incrementarIteraciones();
    }
}

void GestorTorneo::cargarDatos() {
    resetearIteraciones();

    if (equipos.size() == 0) {
        leerCSV("datos/selecciones_clasificadas.csv");
        distribuirGolesJugadores();
    } else {
        cout << "Los datos ya fueron cargados." << endl;
    }

    mostrarMedicionRecursos("Carga de datos");
}

void GestorTorneo::ordenarEquiposPorRanking(Equipo** salida, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        salida[i] = &equipos[i];
    }

    for (int i = 0; i < cantidad - 1; i++) {
        for (int j = i + 1; j < cantidad; j++) {
            if (salida[i]->getRanking() > salida[j]->getRanking()) {
                Equipo* temp = salida[i];
                salida[i] = salida[j];
                salida[j] = temp;
            }
        }
        incrementarIteraciones();
    }
}

void GestorTorneo::sorteoBombos(Equipo* bombos[4][12]) {
    Equipo* equiposOrdenados[48];
    ordenarEquiposPorRanking(equiposOrdenados, 48);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 12; j++) {
            bombos[i][j] = equiposOrdenados[i * 12 + j];
        }
    }
}

bool GestorTorneo::validarConfederaciones(Equipo* equiposGrupo[4]) {
    int conteoUEFA = 0;

    for (int i = 0; i < 4; i++) {
        if (equiposGrupo[i] == nullptr) return false;

        if (strcmp(equiposGrupo[i]->getConfederacion(), "UEFA") == 0) {
            conteoUEFA++;
            if (conteoUEFA > 2) return false;
        } else {
            for (int j = i + 1; j < 4; j++) {
                if (equiposGrupo[j] != nullptr &&
                    strcmp(equiposGrupo[i]->getConfederacion(),
                           equiposGrupo[j]->getConfederacion()) == 0) {
                    return false;
                }
            }
        }
        incrementarIteraciones();
    }

    return true;
}

void GestorTorneo::conformarGrupos() {
    resetearIteraciones();

    for (int i = 0; i < 12; i++) {
        if (grupos[i]) {
            delete grupos[i];
            grupos[i] = nullptr;
        }
    }

    Equipo* equiposOrdenados[48];
    ordenarEquiposPorRanking(equiposOrdenados, 48);

    for (int i = 0; i < 12; i++) {
        grupos[i] = new Grupo('A' + i);
    }

    for (int grupo = 0; grupo < 12; grupo++) {
        for (int bombo = 0; bombo < 4; bombo++) {
            int indice = bombo * 12 + grupo;

            if (indice < 48 && equiposOrdenados[indice] != nullptr) {
                grupos[grupo]->agregarEquipo(equiposOrdenados[indice], bombo);
            }

            incrementarIteraciones();
        }
    }

    cout << "\n========== GRUPOS CONFORMADOS ==========" << endl;

    for (int i = 0; i < 12; i++) {
        cout << *grupos[i] << endl;
    }

    mostrarMedicionRecursos("Conformacion de grupos");
}

void GestorTorneo::asignarFechasFaseGrupos() {
    int partidosPorDia[DIAS_FASE_GRUPOS] = {0};
    int ultimoPartidoEquipo[48];

    for (int i = 0; i < 48; i++) {
        ultimoPartidoEquipo[i] = -DIAS_DESCANSO_ENTRE_PARTIDOS - 1;
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 6; j++) {
            Partido* p = grupos[i]->getPartido(j);
            if (!p) continue;

            int diaAsignado = -1;

            for (int dia = 0; dia < DIAS_FASE_GRUPOS; dia++) {
                if (partidosPorDia[dia] >= MAX_PARTIDOS_POR_DIA) continue;

                Equipo* eq1 = p->getEquipo1();
                Equipo* eq2 = p->getEquipo2();

                int idx1 = -1;
                int idx2 = -1;

                for (int k = 0; k < equipos.size(); k++) {
                    if (&equipos[k] == eq1) idx1 = k;
                    if (&equipos[k] == eq2) idx2 = k;
                }

                if (idx1 != -1 && idx2 != -1 &&
                    dia - ultimoPartidoEquipo[idx1] >= DIAS_DESCANSO_ENTRE_PARTIDOS &&
                    dia - ultimoPartidoEquipo[idx2] >= DIAS_DESCANSO_ENTRE_PARTIDOS) {
                    diaAsignado = dia;
                    break;
                }

                incrementarIteraciones();
            }

            if (diaAsignado != -1) {
                char fecha[20];
                sprintf(fecha, "%d-06-2026", 20 + diaAsignado);
                p->setFecha(fecha);
                partidosPorDia[diaAsignado]++;

                int idx1 = -1;
                int idx2 = -1;

                for (int k = 0; k < equipos.size(); k++) {
                    if (&equipos[k] == p->getEquipo1()) idx1 = k;
                    if (&equipos[k] == p->getEquipo2()) idx2 = k;
                }

                if (idx1 != -1) ultimoPartidoEquipo[idx1] = diaAsignado;
                if (idx2 != -1) ultimoPartidoEquipo[idx2] = diaAsignado;
            }

            incrementarIteraciones();
        }
    }
}

void GestorTorneo::simularFaseGrupos() {
    resetearIteraciones();

    for (int i = 0; i < 12; i++) {
        grupos[i]->generarFixture();
    }

    asignarFechasFaseGrupos();

    for (int i = 0; i < 12; i++) {
        grupos[i]->simularPartidos();
        grupos[i]->calcularClasificacion();
        incrementarIteraciones();
    }

    cout << "\n========== RESULTADOS FASE DE GRUPOS ==========" << endl;

    for (int i = 0; i < 12; i++) {
        cout << *grupos[i] << endl;
    }

    mostrarMedicionRecursos("Simulacion fase de grupos");
}

void GestorTorneo::construirClasificacionYFixture() {
    resetearIteraciones();

    Equipo* primeros[12];
    Equipo* segundos[12];
    Equipo* terceros[12];

    for (int i = 0; i < 12; i++) {
        Equipo* clasificados[2] = {nullptr, nullptr};
        grupos[i]->getClasificados(clasificados);

        primeros[i] = clasificados[0];
        segundos[i] = clasificados[1];

        Equipo* tercero = nullptr;
        grupos[i]->getTerceros(&tercero);
        terceros[i] = tercero;

        incrementarIteraciones();
    }

    for (int i = 0; i < 11; i++) {
        for (int j = i + 1; j < 12; j++) {
            if (terceros[i] && terceros[j]) {
                if (terceros[i]->getPuntos() < terceros[j]->getPuntos()) {
                    Equipo* temp = terceros[i];
                    terceros[i] = terceros[j];
                    terceros[j] = temp;
                }
            }
            incrementarIteraciones();
        }
    }

    Equipo* clasificadosR16[32];

    for (int i = 0; i < 12; i++) {
        clasificadosR16[i] = primeros[i];
    }

    for (int i = 0; i < 12; i++) {
        clasificadosR16[12 + i] = segundos[i];
    }

    for (int i = 0; i < 8; i++) {
        clasificadosR16[24 + i] = terceros[i];
    }

    fixture.construirR16(clasificadosR16, 32);

    cout << "\n========== CLASIFICADOS A R16 ==========" << endl;

    for (int i = 0; i < 32; i++) {
        if (clasificadosR16[i]) {
            cout << i + 1 << ". " << clasificadosR16[i]->getPais() << endl;
        }
    }

    mostrarMedicionRecursos("Construccion clasificacion y fixture");
}

void GestorTorneo::simularEliminatorias() {
    resetearIteraciones();
    fixture.simularTodo();
    mostrarMedicionRecursos("Simulacion eliminatorias");
}

void GestorTorneo::generarEstadisticasFinales() {
    if (!fixture.getCampeon()) {
        cout << "Primero debe simular las eliminatorias (Opcion 5)" << endl;
        return;
    }

    cout << "\n========== ESTADISTICAS FINALES DEL TORNEO ==========" << endl;

    cout << "\n--- RANKING DE LOS 4 PRIMEROS PUESTOS ---" << endl;
    cout << "1. CAMPEON: " << fixture.getCampeon()->getPais() << endl;
    cout << "2. SUBCAMPEON: " << fixture.getSubcampeon()->getPais() << endl;
    cout << "3. TERCER PUESTO: " << fixture.getTercerPuesto()->getPais() << endl;
    cout << "4. CUARTO PUESTO: " << fixture.getCuartoPuesto()->getPais() << endl;

    cout << "\n--- MAXIMO GOLEADOR DEL EQUIPO CAMPEON ---" << endl;

    Equipo* campeon = fixture.getCampeon();
    int maxGoles = -1;
    Jugador* maxGoleador = nullptr;

    for (int i = 0; i < campeon->getJugadores().size(); i++) {
        if (campeon->getJugadores()[i].getGoles() > maxGoles) {
            maxGoles = campeon->getJugadores()[i].getGoles();
            maxGoleador = &campeon->getJugadores()[i];
        }
    }

    if (maxGoleador) {
        cout << *maxGoleador << endl;
    }

    cout << "\n--- TRES MAYORES GOLEADORES DE LA COPA ---" << endl;

    Jugador* goleadores[3] = {nullptr, nullptr, nullptr};
    int golesGoleadores[3] = {0, 0, 0};

    for (int i = 0; i < equipos.size(); i++) {
        for (int j = 0; j < equipos[i].getJugadores().size(); j++) {
            int g = equipos[i].getJugadores()[j].getGoles();

            for (int k = 0; k < 3; k++) {
                if (g > golesGoleadores[k]) {
                    for (int l = 2; l > k; l--) {
                        goleadores[l] = goleadores[l - 1];
                        golesGoleadores[l] = golesGoleadores[l - 1];
                    }

                    goleadores[k] = &equipos[i].getJugadores()[j];
                    golesGoleadores[k] = g;
                    break;
                }
            }

            incrementarIteraciones();
        }
    }

    for (int i = 0; i < 3 && goleadores[i]; i++) {
        cout << i + 1 << ". " << *goleadores[i] << endl;
    }

    cout << "\n--- EQUIPO CON MAS GOLES HISTORICOS ---" << endl;

    Equipo* maxEquipo = nullptr;
    int maxGolesEq = -1;

    for (int i = 0; i < equipos.size(); i++) {
        int totalGoles = equipos[i].getGolesAFavor();

        if (totalGoles > maxGolesEq) {
            maxGolesEq = totalGoles;
            maxEquipo = &equipos[i];
        }

        incrementarIteraciones();
    }

    if (maxEquipo) {
        cout << maxEquipo->getPais() << " con " << maxGolesEq << " goles" << endl;
    }

    mostrarMedicionRecursos("Generacion de estadisticas");
}

size_t GestorTorneo::calcularMemoriaTotal() {
    size_t total = sizeof(*this);

    total += equipos.size() * sizeof(Equipo);

    for (int i = 0; i < equipos.size(); i++) {
        total += equipos[i].getJugadores().size() * sizeof(Jugador);
    }

    for (int i = 0; i < 12 && grupos[i]; i++) {
        total += sizeof(Grupo);
    }

    return total;
}

void GestorTorneo::mostrarMedicionRecursos(const char* funcionalidad) {
    cout << "\n--- MEDICION DE RECURSOS: " << funcionalidad << " ---" << endl;
    cout << "Iteraciones: " << contadorIteraciones << endl;

#ifdef __linux__
    long long memoria = 0;
    ifstream statm("/proc/self/statm");
    statm >> memoria;
    memoria *= sysconf(_SC_PAGESIZE);
    cout << "Memoria consumida: " << memoria / 1024 << " KB" << endl;
#else
    cout << "Memoria calculada: " << calcularMemoriaTotal() / 1024 << " KB (estimado)" << endl;
#endif
}

void GestorTorneo::mostrarMenu() {
    int opcion = 0;

    while (opcion != 7) {
        cout << "\n========================================" << endl;
        cout << "       UdeAWorldCup - Mundial 2026" << endl;
        cout << "========================================" << endl;
        cout << "1. Cargar datos (Req. I)" << endl;
        cout << "2. Conformar grupos (Req. II)" << endl;
        cout << "3. Simular fase de grupos (Req. III-a)" << endl;
        cout << "4. Construir clasificacion y fixture (Req. III-b)" << endl;
        cout << "5. Simular eliminatorias (Req. III-c)" << endl;
        cout << "6. Generar estadisticas finales (Req. IV)" << endl;
        cout << "7. Salir" << endl;
        cout << "========================================" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cargarDatos();
            break;

        case 2:
            if (equipos.size() > 0) {
                conformarGrupos();
            } else {
                cout << "Primero debe cargar los datos (Opcion 1)" << endl;
            }
            break;

        case 3:
            if (grupos[0]) {
                simularFaseGrupos();
            } else {
                cout << "Primero debe conformar los grupos (Opcion 2)" << endl;
            }
            break;

        case 4:
            if (grupos[0]) {
                construirClasificacionYFixture();
            } else {
                cout << "Primero debe simular la fase de grupos (Opcion 3)" << endl;
            }
            break;

        case 5:
            if (grupos[0]) {
                construirClasificacionYFixture();
                simularEliminatorias();
            } else {
                cout << "Primero debe simular la fase de grupos (Opcion 3)" << endl;
            }
            break;

        case 6:
            generarEstadisticasFinales();
            break;

        case 7:
            cout << "Gracias por usar UdeAWorldCup" << endl;
            break;

        default:
            cout << "Opcion invalida" << endl;
        }
    }
}
