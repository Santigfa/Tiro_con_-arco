#include "Jugador.h"
#include <cstring>
#include <cmath>

Jugador::Jugador()
{
    fuerza = 10.0f;
    angulo = 45.0f;
    nombre = new char[8];
    std::strcpy(nombre, "Jugador");
}

Jugador::Jugador(const Jugador& j)
{
    fuerza = j.fuerza;
    angulo = j.angulo;

    int n = std::strlen(j.nombre);
    nombre = new char[n + 1];
    std::strcpy(nombre, j.nombre);
}

Jugador::~Jugador()
{
    delete[] nombre;
}

Flecha* Jugador::disparar()
{
    Flecha* f = new Flecha();

    float rad = angulo * 3.1416f / 180.0f;
    float vx = fuerza * std::cos(rad);
    float vy = fuerza * std::sin(rad);

    f->setX(0);
    f->setY(0);
    f->setVx(vx);
    f->setVy(vy);

    return f;
}

float Jugador::getFuerza()
{
    return fuerza;
}

void Jugador::setFuerza(float f)
{
    fuerza = f;
}

float Jugador::getAngulo()
{
    return angulo;
}

void Jugador::setAngulo(float a)
{
    angulo = a;
}

char* Jugador::getNombre()
{
    return nombre;
}

void Jugador::setNombre(char* nuevoNombre)
{
    delete[] nombre;

    int n = std::strlen(nuevoNombre);
    nombre = new char[n + 1];
    std::strcpy(nombre, nuevoNombre);
}
