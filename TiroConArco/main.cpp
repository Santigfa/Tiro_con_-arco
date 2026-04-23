#include <iostream>
#include "Flecha.h"
#include "Jugador.h"
#include "ObjetivoFijo.h"
#include "Portal.h"

using namespace std;

int main()
{
    Jugador j;

    Flecha* f = j.disparar();

    for (int i = 0; i < 5; i++)
    {
        f->actualizar(1.0f);
        f->aplicarGravedad();

        cout << "Paso " << i + 1 << endl;
        cout << "X: " << f->getX() << endl;
        cout << "Y: " << f->getY() << endl;
        cout << "----------------" << endl;
    }

    ObjetivoFijo obj;

    obj.setX(20);
    obj.setY(5);
    obj.setRadio(2);

    cout << "Objetivo X: " << obj.getX() << endl;
    cout << "Objetivo Y: " << obj.getY() << endl;
    cout << "Radio: " << obj.getRadio() << endl;

    Portal p;

    p.setX(10);
    p.setY(10);

    p.setXDestino(100);
    p.setYDestino(50);

    p.teletransportar(*f);

    cout << "Después del portal:" << endl;
    cout << "X: " << f->getX() << endl;
    cout << "Y: " << f->getY() << endl;

    delete f;

    return 0;
}
