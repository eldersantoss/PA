#include <iostream>
#include <fstream>
#include "reta.h"

using namespace std;

Reta::Reta()
{

}

Reta::Reta(Point &_p1, Point &_p2)
{
    p1.setXY(_p1.getX(),_p1.getY());
    p2.setXY(_p2.getX(),_p2.getY());
}

void Reta::draw(Screen &t)
{
    cout << endl << "==== Draw da classe Reta ====" << endl;
    cout << "p1 = (" << p1.getX() << ", " << p1.getY() << ")\n" <<
            "p2 = (" << p2.getX() << ", " << p2.getY() << endl;
}
