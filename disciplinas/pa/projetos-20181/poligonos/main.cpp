#include <iostream>
#include <cmath>
#include "point.h"
#include "poligono.h"
#include "retangulo.h"

using namespace std;

int main()
{
    // Instanciando novo retangulo:
    Retangulo r1(0,0,4,3);

    // Realizando testes dos metodos da classe base Poligono:
    r1.imprimePoli();
    cout << endl << endl;
    cout << "Area = " << r1.area() << endl << endl;
    r1.transladaPoli(-3,4);
    r1.imprimePoli();
    cout << endl << endl;
    cout << "Area = " << r1.area() << endl << endl;
    r1.rotacionaPoli(M_PI/6);
    r1.imprimePoli();
    cout << endl << endl;
    cout << "Area = " << r1.area() << endl << endl;

    return 0;
}
