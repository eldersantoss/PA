#include <iostream>
#include <cmath>

using namespace std;

class Vetor{
private:
    float x, y;

public:
    float getX(){
        return x;
    }
    void setX(float _x){
        x = _x;
    }
    float getY(){
        return y;
    }
    void setY(float _y){
        y = _y;
    }
    float norma(){
        float nrm = sqrt(x*x + y*y);
        return nrm;
    }
    float teta(){
        float tt = atan(y/x);
        tt *= 180/M_PI;
        return tt;
    }
    void print(){
        cout << "(" << x
             <<", " << y
             << ")" << endl;
    }
};

int main()
{
    Vetor v1;

    v1.setX(4);
    v1.setY(3);

    v1.print();
    cout << v1.norma() << endl;
    cout << v1.teta() << endl;

    return 0;
}
