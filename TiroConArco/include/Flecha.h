#ifndef FLECHA_H
#define FLECHA_H

class Flecha
{
private:
    float x;
    float y;
    float vx;
    float vy;
    bool activa;

public:
    Flecha();
    Flecha(const Flecha&);

    void actualizar(float dt);
    void aplicarGravedad();

    float getX();
    float getY();

    void setX(float);
    void setY(float);

    void setVx(float);
    void setVy(float);

    Flecha operator+(const Flecha&);
    bool operator==(const Flecha&);
};

#endif
