#include "Grupo.h"
#include <cstdlib>
#include <cstring>

Grupo::Grupo(char l) : letra(l) {
    limpiarArrays();

    for (int i = 0; i < 4; i++) {
        equipos[i] = nullptr;
        puntos[i] = 0;
        diferenciaGoles[i] = 0;
        golesAFavor[i] = 0;
    }

    for (int i = 0; i < 6; i++) {
        partidos[i] = nullptr;
    }
}

Grupo::~Grupo() {
    for (int i = 0; i < 6; i++) {
        if (partidos[i]) {
            delete partidos[i];
            partidos[i] = nullptr;
        }
    }
}

void Grupo::limpiarArrays() {
    for (int i = 0; i < 4; i++) {
        puntos[i] = 0;
        diferenciaGoles[i] = 0;
        golesAFavor[i] = 0;
    }
}

void Grupo::agregarEquipo(Equipo* equipo, int posicion) {
    if (posicion >= 0 && posicion < 4) {
        equipos[posicion] = equipo;
    }
}

void Grupo::generarFixture() {
    int idx = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (equipos[i] != nullptr && equipos[j] != nullptr) {
                char fecha[20];
                sprintf(fecha, "%s", FECHA_INICIO_MUNDIAL);

                partidos[idx] = new Partido(equipos[i], equipos[j], fecha, "00:00", "nombreSede");
                idx++;
            }
        }
    }
}

void Grupo::simularPartidos() {
    limpiarArrays();

    for (int i = 0; i < 6; i++) {
        if (partidos[i]) {
            partidos[i]->simular();
            partidos[i]->actualizarEstadisticasPostPartido();

            Equipo* eq1 = partidos[i]->getEquipo1();
            Equipo* eq2 = partidos[i]->getEquipo2();
            int goles1 = partidos[i]->getGolesEq1();
            int goles2 = partidos[i]->getGolesEq2();

            int idx1 = -1;
            int idx2 = -1;

            for (int j = 0; j < 4; j++) {
                if (equipos[j] == eq1) idx1 = j;
                if (equipos[j] == eq2) idx2 = j;
            }

            if (idx1 != -1 && idx2 != -1) {
                golesAFavor[idx1] += goles1;
                golesAFavor[idx2] += goles2;

                diferenciaGoles[idx1] += (goles1 - goles2);
                diferenciaGoles[idx2] += (goles2 - goles1);

                if (goles1 > goles2) {
                    puntos[idx1] += 3;
                } else if (goles2 > goles1) {
                    puntos[idx2] += 3;
                } else {
                    puntos[idx1] += 1;
                    puntos[idx2] += 1;
                }
            }
        }
    }
}

int Grupo::compararEquipos(int idx1, int idx2) {
    if (equipos[idx1] == nullptr && equipos[idx2] == nullptr) return 0;
    if (equipos[idx1] == nullptr) return 1;
    if (equipos[idx2] == nullptr) return -1;

    if (puntos[idx1] != puntos[idx2]) {
        return puntos[idx2] - puntos[idx1];
    }

    if (diferenciaGoles[idx1] != diferenciaGoles[idx2]) {
        return diferenciaGoles[idx2] - diferenciaGoles[idx1];
    }

    if (golesAFavor[idx1] != golesAFavor[idx2]) {
        return golesAFavor[idx2] - golesAFavor[idx1];
    }

    return rand() % 2 == 0 ? -1 : 1;
}

void Grupo::ordenarClasificacion(int* indices) {
    for (int i = 0; i < 4; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (compararEquipos(indices[i], indices[j]) > 0) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }
}

void Grupo::calcularClasificacion() {
    int indices[4];
    ordenarClasificacion(indices);
}

int Grupo::getCantidadClasificados() {
    return 4;
}

int Grupo::getClasificados(Equipo** salida) {
    int indices[4];
    ordenarClasificacion(indices);

    for (int i = 0; i < 2; i++) {
        salida[i] = equipos[indices[i]];
    }

    return 2;
}

int Grupo::getTerceros(Equipo** salida) {
    int indices[4];
    ordenarClasificacion(indices);

    salida[0] = equipos[indices[2]];
    return 1;
}

Equipo* Grupo::getEquipo(int posicion) const {
    if (posicion >= 0 && posicion < 4) {
        return equipos[posicion];
    }

    return nullptr;
}

Partido* Grupo::getPartido(int idx) const {
    if (idx >= 0 && idx < 6) {
        return partidos[idx];
    }

    return nullptr;
}

ostream& operator<<(ostream& os, const Grupo& g) {
    os << "Grupo " << g.letra << ":" << endl;
    os << "----------------------------------------" << endl;
    os << "Pos  Equipo                    Pts  DG  GF" << endl;
    os << "----------------------------------------" << endl;

    int indices[4];
    int tempPuntos[4];
    int tempDG[4];
    int tempGF[4];

    for (int i = 0; i < 4; i++) {
        indices[i] = i;
        tempPuntos[i] = g.puntos[i];
        tempDG[i] = g.diferenciaGoles[i];
        tempGF[i] = g.golesAFavor[i];
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (g.equipos[indices[i]] == nullptr && g.equipos[indices[j]] != nullptr) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            } else if (g.equipos[indices[i]] != nullptr && g.equipos[indices[j]] != nullptr) {
                if (tempPuntos[indices[i]] < tempPuntos[indices[j]]) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                } else if (tempPuntos[indices[i]] == tempPuntos[indices[j]] &&
                           tempDG[indices[i]] < tempDG[indices[j]]) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                } else if (tempPuntos[indices[i]] == tempPuntos[indices[j]] &&
                           tempDG[indices[i]] == tempDG[indices[j]] &&
                           tempGF[indices[i]] < tempGF[indices[j]]) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        os << i + 1 << "   ";

        if (g.equipos[indices[i]] == nullptr) {
            const char* texto = "Equipo no asignado";

            for (int j = 0; j < 25 && texto[j]; j++) os << texto[j];
            for (int j = strlen(texto); j < 25; j++) os << " ";

            os << tempPuntos[indices[i]] << "   "
               << tempDG[indices[i]] << "   "
               << tempGF[indices[i]] << endl;

            continue;
        }

        const char* pais = g.equipos[indices[i]]->getPais();

        for (int j = 0; j < 25 && pais[j]; j++) os << pais[j];
        for (int j = strlen(pais); j < 25; j++) os << " ";

        os << tempPuntos[indices[i]] << "   "
           << tempDG[indices[i]] << "   "
           << tempGF[indices[i]] << endl;
    }

    os << "----------------------------------------" << endl;

    return os;
}
