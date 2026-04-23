#include "Flecha.h"

Flecha::Flecha()
{
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
    activa = true;
}

Flecha::Flecha(const Flecha& f)
{
    x = f.x;
    y = f.y;
    vx = f.vx;
    vy = f.vy;
    activa = f.activa;
}

void Flecha::actualizar(float dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Flecha::aplicarGravedad()
{
    vy -= 9.8f;
}

float Flecha::getX()
{
    return x;
}

float Flecha::getY()
{
    return y;
}

void Flecha::setX(float val)
{
    x = val;
}

void Flecha::setY(float val)
{
    y = val;
}

void Flecha::setVx(float val)
{
    vx = val;
}

void Flecha::setVy(float val)
{
    vy = val;
}

Flecha Flecha::operator+(const Flecha& f)
{
    Flecha temp;
    temp.vx = vx + f.vx;
    temp.vy = vy + f.vy;
    return temp;
}

bool Flecha::operator==(const Flecha& f)
{
    return (x == f.x && y == f.y);
}
