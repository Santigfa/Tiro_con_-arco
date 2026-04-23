#ifndef PORTAL_H
#define PORTAL_H

#include "Flecha.h"

class Portal
{
private:
    float x;
    float y;
    float xDestino;
    float yDestino;

public:
    Portal();
    Portal(const Portal&);
    ~Portal();

    void teletransportar(Flecha&);

    float getX();
    void setX(float);

    float getY();
    void setY(float);

    float getXDestino();
    void setXDestino(float);

    float getYDestino();
    void setYDestino(float);
};

#endif