#ifndef JUGADOR_H
#define JUGADOR_H

#include "Flecha.h"

class Jugador
{
private:
    float fuerza;
    float angulo;
    char* nombre;

public:
    Jugador();
    Jugador(const Jugador&);
    ~Jugador();

    Flecha* disparar();

    float getFuerza();
    void setFuerza(float);

    float getAngulo();
    void setAngulo(float);

    char* getNombre();
    void setNombre(char*);
};

#endif // JUGADOR_H