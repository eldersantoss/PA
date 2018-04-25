#include <iostream>
#include <cmath>
#include "point.h"

using namespace std;

Point::Point()
{
    x=0; y=0;
}

Point::Point(float _x, float _y)
{
    x = _x; y = _y;
}

Point::Point(Point &p1)
{
    x = p1.x;
    y = p1.y;
}

void Point::setX(float _x)
{
    x = _x;
}

void Point::setY(float _y)
{
    y = _y;
}

void Point::setXY(float _x, float _y)
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

Point Point::add(Point p1)
{
    Point ret;
    ret.x = x+p1.x;
    ret.y = y+p1.y;
    return ret;
}

Point Point::sub(Point p1)
{
    Point ret;
    ret.x = x-p1.x;
    ret.y = y-p1.y;
    return ret;
}

float Point::norma()
{
    return sqrt(x*x + y*y);
}

void Point::translada(float a, float b)
{
    x += a;
    y += b;
}
