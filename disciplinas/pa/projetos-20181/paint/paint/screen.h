#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Screen
{
private:
    int nlin, ncol;
    char brush;
    vector< vector<char> > mat;
    vector< vector<char> >::iterator imat;

public:
    // construtor da classe
    Screen();
    Screen(int _nlin, int _ncol);

    // desenha um pixel da matriz usando o caratere
    // guardado em 'brush'
    void setPixel(int x, int y);

    // limpa a tela
    void clear();

    // muda o caractere de desenho
    void setBrush(char _brush);

    // envia a tela para um stream de saida
    friend ostream& operator<<(ostream &os, Screen &t);
};

ostream& operator<<(ostream &os, Screen &t);

#endif // SCREEN_H
