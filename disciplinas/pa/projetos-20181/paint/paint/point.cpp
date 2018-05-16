#include <iostream>
#include <cmath>
#include "point.h"

using namespace std;

Point::Point()
{
    x=0; y=0;
}

Point::Point(int _x, int _y)
{
    x = _x; y = _y;
}

Point::Point(Point &p1)
{
    x = p1.x;
    y = p1.y;
}

void Point::setX(int _x)
{
    x = _x;
}

void Point::setY(int _y)
{
    y = _y;
}

void Point::setXY(int _x, int _y)
{
    x = _x; y = _y;
}

float Point::getX()
{
    return x;
}

float Point::getY()
{
    return y;
}

void Point::imprime()
{
    cout << "(" << x << ", " << y << ")";
}
