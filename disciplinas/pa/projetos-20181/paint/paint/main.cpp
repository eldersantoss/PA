#include <iostream>
#include <vector>

#include "figurageometrica.h"
#include "point.h"
#include "reta.h"
#include "retangulo.h"
#include "circulo.h"

using namespace std;

int main()
{
    Screen t;
    vector<FiguraGeometrica*> v_ptr;
    FiguraGeometrica *ptr;

    v_ptr = ptr->read(t);

    for(unsigned i=0; i<v_ptr.size(); i++)
        v_ptr[i]->draw(t);

    return 0;
}
