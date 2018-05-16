#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>

#include "figurageometrica.h"
#include "point.h"
#include "reta.h"
#include "retangulo.h"
#include "circulo.h"

using namespace std;

vector<FiguraGeometrica*> FiguraGeometrica::read(Screen &t)
{
    ifstream fin;
    string buffer;
    vector<FiguraGeometrica*> l;

    fin.open("/home/elder/Documentos/UFRN/7º SEMESTRE/pa/unidade2/projetos-20181/paint/teste-arquivo.txt");
    if(!fin.is_open())
    {
        cout << "Erro ao abrir arquivo!" << endl;
        exit(0);
    }
    else
    {
        cout << "Arquivo aberto com sucesso!" << endl;
    }

    // iniciando leitura do arquivo
    fin >> buffer;

    if(buffer == "dim")
    {
        // criando nova screen vazia
        t = Screen();

        // criando variaveis para receber novas dimensoes da screen
        int _nlin, _ncol;

        // capturando numero de colunas (largura) da screen
        fin >> buffer;
        if(isdigit(buffer[0]))
        {
            _ncol = atoi(buffer.c_str());
        }

        // capturando numero de linhas (altura) da screen
        fin >> buffer;
        if(isdigit(buffer[0]))
        {
            _nlin = atoi(buffer.c_str());
        }

        // definindo novo tamanho para antiga screen vazia
        t = Screen(_nlin, _ncol);
        cout << endl << "Criando nova screen de dimensoes " << _ncol << " x " << _nlin << "..." << endl;

        // continuando leitura do arquivo no laço infinito ate encontrar a palavra 'exit'
        for(;;)
        {
            fin >> buffer;
            while(buffer == "brush")
            {
                fin.ignore(numeric_limits<streamsize>::max(), ' ');
                getline(fin, buffer);
                if(buffer.empty())
                {
                    t.setBrush(' ');
                    cout << endl << "Definindo brush para '" << buffer << "'..." << endl;
                    fin >> buffer;
                }
                if(buffer.length() == 1)
                {
                    t.setBrush(buffer[0]);
                    cout << endl << "Definindo brush para '" << buffer << "'..." << endl;
                    fin >> buffer;
                }
                else
                {
                    cout << "Erro de leitura! Comando incorreto." << endl;
                    exit(0);
                }
            }
            while(buffer == "line")
            {
                int x0, y0, x1, y1;
                Point p0, p1;
                fin >> x0 >> y0 >> x1 >> y1;
                p0.setXY(x0, y0);
                p1.setXY(x1, y1);
                //ptr_fig.push_back(new Reta(p0, p1));
                cout << endl << "Criando nova reta dos pontos p0 = (" << x0 << ", " << y0 <<
                        ") e p1 = (" << x1 << ", " << y1 << ")..." << endl;
                l.push_back(new Reta(p0, p1));
                fin >> buffer;
            }
            while(buffer == "rectangle")
            {
                int x0, y0, largura, altura, fillmode;
                fin >> x0 >> y0 >> largura >> altura >> fillmode;
                Point p0(x0, y0);
                cout << endl << "Criando novo retangulo com ponto superior esquerdo p0 = (" << x0 << ", " << y0 <<
                        "), largura = " << largura << ", altura = " << altura << " e ";
                if(fillmode > 0)
                    cout << "totalmente preenchido..." << endl;
                else
                    cout << "apenas com contorno..." << endl;
                l.push_back(new Retangulo(p0, altura, largura, fillmode));
                fin >> buffer;
            }
            while(buffer == "circle")
            {
                int x0, y0, raio, fillmode;
                fin >> x0 >> y0 >> raio >> fillmode;
                Point p0(x0, y0);
                cout << endl << "Criando novo circulo de centro p0 = (" << x0 << ", " << y0 <<
                        "), raio = " << raio << " e ";
                if(fillmode > 0)
                    cout << "totalmente preenchido..." << endl;
                else
                    cout << "apenas com contorno..." << endl;
                l.push_back(new Circulo(p0, raio, fillmode));
                fin >> buffer;
            }
            if(buffer == "exit")
            {
                break;
            }
            else
            {
                cout << "Erro de leitura! Comando desconhecido ou incorreto." << endl;
                exit(0);
            }
        }
    }
    else
    {
        cout << "Erro de leitura! Arquivo invalido ou corrompido corrompido." << endl;
        exit(0);
    }
    return l;
}
