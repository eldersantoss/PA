#ifndef CIRCULO_H
#define CIRCULO_H

#include "figurageometrica.h"
#include "point.h"

class Circulo : public FiguraGeometrica
{
private:
    Point p0;
    int raio;
    bool cheio;
public:
    Circulo();
    Circulo(Point _p0, int _raio, bool _cheio);
    void draw(Screen &t);
    void read(Screen &t);
};

#endif // CIRCULO_H
