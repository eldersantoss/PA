#ifndef RETANGULO_H
#define RETANGULO_H

#include "figurageometrica.h"
#include "point.h"


class Retangulo : public FiguraGeometrica
{
private:
    Point p0;
    int largura, altura;
    bool cheio;
public:
    Retangulo();
    Retangulo(Point _p0, int _largura, int _altura, bool _cheio);
    void draw(Screen &t);
};

#endif // RETANGULO_H
