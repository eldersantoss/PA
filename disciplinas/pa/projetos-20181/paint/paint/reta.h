#ifndef RETA_H
#define RETA_H

#include "figurageometrica.h"
#include "point.h"

class Reta : public FiguraGeometrica
{
private:
    Point p1, p2;
public:
    Reta();
    Reta(Point &_p1, Point &_p2);
    void draw(Screen &t);
};

#endif // RETA_H
