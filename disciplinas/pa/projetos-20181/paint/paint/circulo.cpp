#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits>
#include "circulo.h"

using namespace std;

Circulo::Circulo()
{

}

Circulo::Circulo(Point _p0, int _raio, bool _cheio)
{
    p0.setXY(_p0.getX(), _p0.getY());
    raio = _raio;
    cheio = _cheio;
}

void Circulo::draw(Screen &t)
{
    cout << endl << "==== Draw da classe circulo ====" << endl;
    cout << "p0 = (" << p0.getX() << ", " << p0.getY() << ")\n"
         << "Raio = " << raio << "\nPreenchimento = " << cheio << endl;
}
