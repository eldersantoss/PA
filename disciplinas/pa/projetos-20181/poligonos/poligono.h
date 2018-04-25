#ifndef POLIGONO_H
#define POLIGONO_H

#include "point.h"
#define MAX_VERT 100

class Poligono
{
private:
    Point *vertices;
    int qtdVertices;

public:
    // Funções auxiliares:
    void limpar();
    void copiar(Poligono &p1);
    void alocaPol(int _qtdVertices);
    bool verificaVertice(Point &v1);

    Poligono();
    Poligono(Poligono &p1);
    ~Poligono();
    void operator=(Poligono &p1);
    void addVertice(Point &v1);
    int getQtdVertices();
    float area();
    void transladaPoli(float a, float b);
    void rotacionaPoli(float teta);
    void imprimePoli();
};

#endif // POLIGONO_H
