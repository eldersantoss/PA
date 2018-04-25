#include <iostream>
#include "retangulo.h"

using namespace std;

Retangulo::Retangulo()
{
    largura = 0; altura = 0;
}

Retangulo::Retangulo(float x, float y, float _largura, float _altura)
{
    Point aux[4];

    largura = _largura;
    altura = _altura;

    aux[0] = Point(x, y);
    aux[1] = Point(x, y-altura);
    aux[2] = Point(x+largura, y-altura);
    aux[3] = Point(x+largura, y);

    for(int i=0; i<4; i++) addVertice(aux[i]);
}

Retangulo::~Retangulo()
{
    limpar();
}
