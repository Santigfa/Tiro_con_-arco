#include "Portal.h"

Portal::Portal()
{
    x = 0;
    y = 0;
    xDestino = 0;
    yDestino = 0;
}

Portal::Portal(const Portal& p)
{
    x = p.x;
    y = p.y;
    xDestino = p.xDestino;
    yDestino = p.yDestino;
}

Portal::~Portal()
{
}

void Portal::teletransportar(Flecha& f)
{
    // mueve la flecha al destino
    f.setX(xDestino);
    f.setY(yDestino);
}

float Portal::getX() { return x; }
void Portal::setX(float val) { x = val; }

float Portal::getY() { return y; }
void Portal::setY(float val) { y = val; }

float Portal::getXDestino() { return xDestino; }
void Portal::setXDestino(float val) { xDestino = val; }

float Portal::getYDestino() { return yDestino; }
void Portal::setYDestino(float val) { yDestino = val; }