#ifndef POINT_H
#define POINT_H


class Point
{
private:
    int x, y;
public:
    Point();
    Point(int _x, int _y);
    Point(Point &p1);
    void setX(int _x);
    void setY(int _y);
    void setXY(int _x, int _y);
    float getX();
    float getY();
    void imprime();
};

#endif // POINT_H
