#ifndef RETANGULO_H
#define RETANGULO_H

#include "poligono.h"

class Retangulo : public Poligono
{
private:
    float largura, altura;
public:
    Retangulo();
    Retangulo(float x, float y, float _largura, float _altura);
    ~Retangulo();
    void setLargura(float _largura);
    void setAltura(float _altura);
    float getLargura();
    float getAltura ();
};

#endif // RETANGULO_H
