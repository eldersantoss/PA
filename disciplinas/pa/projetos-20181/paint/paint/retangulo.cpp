#include <iostream>
#include <fstream>
#include "retangulo.h"

using namespace std;

Retangulo::Retangulo()
{

}

Retangulo::Retangulo(Point _p0, int _largura, int _altura, bool _cheio)
{
    p0.setXY(_p0.getX(),_p0.getY());
    largura = _largura;
    altura = _altura;
    cheio = _cheio;
}

void Retangulo::draw(Screen &t)
{
    cout << endl << "==== Draw da classe Retangulo ====" << endl;
    cout << "p0 = (" << p0.getX() << ", " << p0.getY() << ")\n"
         << "Largura = " << largura << "\nAltura = " << altura
         << "\nPreenchimento = " << cheio << endl;
}
