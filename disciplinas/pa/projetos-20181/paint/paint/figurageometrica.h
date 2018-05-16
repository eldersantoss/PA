#ifndef FIGURAGEOMETRICA_H
#define FIGURAGEOMETRICA_H

#include "screen.h"

class FiguraGeometrica
{
public:
    virtual void draw(Screen &t) = 0;
    vector<FiguraGeometrica *> read(Screen &t);
};

#endif // FIGURAGEOMETRICA_H
