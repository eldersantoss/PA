#include <iostream>
#include <cstring>
#include <cmath>
#include "poligono.h"

using namespace std;

void Poligono::limpar()
{
    if(vertices!=nullptr)
    {
        delete[] vertices;
        qtdVertices = 0;
    }
    else
    {
        return;
    }
}

void Poligono::copiar(Poligono &p1)
{
    if(p1.vertices!=nullptr)
    {
        alocaPol(p1.qtdVertices);
        for(int i=0; i<qtdVertices; i++) vertices[i] = p1.vertices[i];
    }
    else
    {
        Poligono();
    }
}

void Poligono::alocaPol(int _qtdVertices)
{
    qtdVertices = _qtdVertices;
    vertices = new Point[qtdVertices];
}

// Verifica se o vértice que se quer adicionar está no sentido anti-horário e forma polígono convexo:
bool Poligono::verificaVertice(Point &v1)
{
    bool ret;
    float x[3], y[3];
    float alpha, beta, gama;

    if(qtdVertices < 3)
    {
        ret = true;
    }
    else
    {
        // Passando valor dos vertices a serem analisados para os vetores auxiliares:
        // Primeiro vertice:
        x[0] = vertices[qtdVertices-2].getX();
        y[0] = vertices[qtdVertices-2].getY();
        // Segundo vertice (central):
        x[1] = vertices[qtdVertices-1].getX();
        y[1] = vertices[qtdVertices-1].getY();
        // Terceiro vertice (A ser validado):
        x[2] = v1.getX();
        y[2] = v1.getY();

        // Centralizando vertices no ponto (0, 0):
        // Primeiro:
        x[0] -= x[1];
        y[0] -= y[1];
        // Terceiro:
        x[2] -= x[1];
        y[2] -= y[1];
        // Segundo:
        x[1] -= x[1];
        y[1] -= y[1];

        // Calculando angulos entre as retas que passam pelos vertices das extremidades e da origem:
        // Primeiro:
        alpha = atan2(y[0],x[0]);
        if(alpha<0) alpha += 2*M_PI;
        // Segundo:
        beta = atan2(y[2], x[2]);
        if(beta<0) beta += 2*M_PI;

        // Encontrando angulo interno formado pelos 3 vertices do polígono:
        gama = alpha - beta;

        // Verificando se o angulo interno do poligono esta de acordo:
        if(gama>0 && gama<M_PI) ret = true;
        else ret = false;
    }
    return ret;
}

Poligono::Poligono()
{
    vertices = nullptr;
    qtdVertices = 0;
}

Poligono::Poligono(Poligono &p1)
{
    copiar(p1);
}

Poligono::~Poligono()
{
    limpar();
}

void Poligono::operator=(Poligono &p1)
{
    limpar();
    copiar(p1);
}

void Poligono::addVertice(Point &v1)
{
    if(verificaVertice(v1) && qtdVertices<100)
    {
        if(vertices == nullptr)
        {
            alocaPol(1) ;
            vertices[0] = v1;
        }
        else
        {
            Poligono temp(*this); // Cria um poligono temporario utilizando o construtor de copia e o ponteiro this.
            int newQtdVertices = temp.qtdVertices+1; // Incrementa o numero de vertices do novo poligono.

            limpar(); // Desaloca a memoria do poligono antigo.
            alocaPol(newQtdVertices); // Realoca a nova memoria com um espaco a mais para adicionar o novo vertice.

            for(int i=0; i<temp.qtdVertices; i++) vertices[i] = temp.vertices[i]; // Copia todos os vertices antigos
            // de volta.
            vertices[qtdVertices-1] = v1; // Adiciona o novo vertice na ultima posicao do array vertices.
        }
    }
    else
    {
        cerr << "Vertice invalido! Sao aceitos apenas vertices adicionados em sentido anti-horario"
                "e o polígono deve ser convexo." << endl;
        exit(0);
    }
}

int Poligono::getQtdVertices()
{
    return qtdVertices;
}

float Poligono::area()
{
    float ret;
    float soma_dir=0, soma_esq=0;
    int tamAux = qtdVertices+1;
    Point *aux = new Point[tamAux];

    for(int i=0; i<qtdVertices; i++) aux[i] = vertices[i];
    aux[tamAux-1] = vertices[0];

    for(int i=0; i<tamAux-1; i++)
    {
        soma_dir += aux[i].getX()*aux[i+1].getY();
        soma_esq += aux[i+1].getX()*aux[i].getY();
    }

    ret = abs(soma_dir-soma_esq)/2;
    return ret;
}

void Poligono::transladaPoli(float a, float b)
{
    float x_old, y_old;
    for(int i=0; i<qtdVertices; i++)
    {
        x_old = vertices[i].getX();
        y_old = vertices[i].getY();
        vertices[i].setX(x_old+a);
        vertices[i].setY(y_old+b);
    }
}

void Poligono::rotacionaPoli(float teta)
{
    float x_old, y_old, x_rot, y_rot;
    // Aplicando rotacao de teta graus em duas dimensoes.
    for(int i=0; i<qtdVertices; i++)
    {
        x_old = vertices[i].getX(); y_old = vertices[i].getY();
        x_rot = cos(teta)*x_old + sin(teta)*y_old;
        y_rot = -sin(teta)*x_old + cos(teta)*y_old;
        vertices[i].setXY(x_rot, y_rot);
    }
}

void Poligono::imprimePoli()
{
    for(int i=0; i<qtdVertices-1; i++)
    {
        vertices[i].imprime();
        cout << "=>";
    }
    vertices[qtdVertices-1].imprime();
    cout << "=>";
    vertices[0].imprime();
}
