#include "Partido.h"
#include <cstring>
#include <cstdlib>
#include <cmath>

Partido::Partido(Equipo* eq1, Equipo* eq2, const char* fec, const char* hor, const char* sed)
    : fecha(nullptr), hora(nullptr), sede(nullptr),
    equipo1(eq1), equipo2(eq2),
    golesEq1(0), golesEq2(0),
    posesionEq1(50.0), prorroga(false),
    tarjetasAmarillasEq1(0), tarjetasAmarillasEq2(0),
    tarjetasRojasEq1(0), tarjetasRojasEq2(0),
    faltasEq1(0), faltasEq2(0) {

    copiarCadena(fecha, fec);
    copiarCadena(hora, hor);
    copiarCadena(sede, sed);

    for (int i = 0; i < 3; i++) {
        arbitros[i] = new char[20];
        strcpy(arbitros[i], "arbitro");
    }

    for (int i = 0; i < 11; i++) {
        convocadosEq1[i] = nullptr;
        convocadosEq2[i] = nullptr;
        golesConvocadosEq1[i] = 0;
        golesConvocadosEq2[i] = 0;
    }
}

Partido::~Partido() {
    delete[] fecha;
    delete[] hora;
    delete[] sede;

    for (int i = 0; i < 3; i++) {
        delete[] arbitros[i];
    }
}

Partido::Partido(const Partido& other)
    : fecha(nullptr), hora(nullptr), sede(nullptr),
    equipo1(other.equipo1), equipo2(other.equipo2),
    golesEq1(other.golesEq1), golesEq2(other.golesEq2),
    posesionEq1(other.posesionEq1), prorroga(other.prorroga),
    tarjetasAmarillasEq1(other.tarjetasAmarillasEq1),
    tarjetasAmarillasEq2(other.tarjetasAmarillasEq2),
    tarjetasRojasEq1(other.tarjetasRojasEq1),
    tarjetasRojasEq2(other.tarjetasRojasEq2),
    faltasEq1(other.faltasEq1), faltasEq2(other.faltasEq2) {

    copiarCadena(fecha, other.fecha);
    copiarCadena(hora, other.hora);
    copiarCadena(sede, other.sede);

    for (int i = 0; i < 3; i++) {
        arbitros[i] = new char[strlen(other.arbitros[i]) + 1];
        strcpy(arbitros[i], other.arbitros[i]);
    }

    for (int i = 0; i < 11; i++) {
        convocadosEq1[i] = other.convocadosEq1[i];
        convocadosEq2[i] = other.convocadosEq2[i];
        golesConvocadosEq1[i] = other.golesConvocadosEq1[i];
        golesConvocadosEq2[i] = other.golesConvocadosEq2[i];
    }
}

Partido& Partido::operator=(const Partido& other) {
    if (this != &other) {
        delete[] fecha;
        delete[] hora;
        delete[] sede;

        copiarCadena(fecha, other.fecha);
        copiarCadena(hora, other.hora);
        copiarCadena(sede, other.sede);

        equipo1 = other.equipo1;
        equipo2 = other.equipo2;

        golesEq1 = other.golesEq1;
        golesEq2 = other.golesEq2;
        posesionEq1 = other.posesionEq1;
        prorroga = other.prorroga;

        tarjetasAmarillasEq1 = other.tarjetasAmarillasEq1;
        tarjetasAmarillasEq2 = other.tarjetasAmarillasEq2;
        tarjetasRojasEq1 = other.tarjetasRojasEq1;
        tarjetasRojasEq2 = other.tarjetasRojasEq2;

        faltasEq1 = other.faltasEq1;
        faltasEq2 = other.faltasEq2;

        for (int i = 0; i < 3; i++) {
            delete[] arbitros[i];
            arbitros[i] = new char[strlen(other.arbitros[i]) + 1];
            strcpy(arbitros[i], other.arbitros[i]);
        }
    }
    return *this;
}

void Partido::copiarCadena(char*& destino, const char* origen) {
    if (!origen) return;
    destino = new char[strlen(origen) + 1];
    strcpy(destino, origen);
}

double Partido::calcularGolesEsperados(Equipo* A, Equipo* B) {
    double gfA = A->getPromedioGolesAFavor();
    double gcB = B->getPromedioGolesEnContra();

    return 1.35 * pow(gfA / 1.35, 0.6) * pow(gcB / 1.35, 0.4);
}

int Partido::simularGoles(double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do {
        k++;
        p *= (double)rand() / RAND_MAX;
    } while (p > L);

    return k - 1;
}

void Partido::simular() {
    double lambda1 = calcularGolesEsperados(equipo1, equipo2);
    double lambda2 = calcularGolesEsperados(equipo2, equipo1);

    golesEq1 = simularGoles(lambda1);
    golesEq2 = simularGoles(lambda2);
}

void Partido::resolverEmpate() {
    if (golesEq1 == golesEq2) {
        prorroga = true;

        if (rand() % 2 == 0) {
            golesEq1++;
        } else {
            golesEq2++;
        }
    }
}

void Partido::actualizarEstadisticasPostPartido() {
    equipo1->actualizarEstadisticas(golesEq1, golesEq2, 0, 0, 0);
    equipo2->actualizarEstadisticas(golesEq2, golesEq1, 0, 0, 0);
}

Equipo* Partido::getGanador() {
    if (golesEq1 > golesEq2) return equipo1;
    if (golesEq2 > golesEq1) return equipo2;
    return nullptr;
}

bool Partido::hayEmpate() const {
    return golesEq1 == golesEq2;
}

void Partido::setFecha(const char* fec) {
    delete[] fecha;
    copiarCadena(fecha, fec);
}

void Partido::setHora(const char* hor) {
    delete[] hora;
    copiarCadena(hora, hor);
}

ostream& operator<<(ostream& os, const Partido& p) {
    os << p.equipo1->getPais() << " "
       << p.golesEq1 << " - "
       << p.golesEq2 << " "
       << p.equipo2->getPais();
    return os;
}
