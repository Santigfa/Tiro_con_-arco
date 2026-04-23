#include <iostream>
#include "Flecha.h"
#include "Jugador.h"

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

    delete f;

    return 0;
}
