#ifndef POINT_H
#define POINT_H


class Point
{
private:
    float x, y;
public:
    Point();
    Point(float _x, float _y);
    Point(Point &p1);
    void setX(float _x);
    void setY(float _y);
    void setXY(float _x, float _y);
    float getX();
    float getY();
    void imprime();
    Point add(Point p1);
    Point sub(Point p1);
    float norma();
    void translada(float a, float b);
};

#endif // POINT_H
