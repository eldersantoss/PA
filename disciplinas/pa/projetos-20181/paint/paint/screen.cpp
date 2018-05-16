#include "screen.h"

Screen::Screen()
{
    nlin = 0; ncol=0;
}

Screen::Screen(int _nlin, int _ncol)
{
    nlin = _nlin;
    ncol = _ncol;
    mat = vector< vector<char> >(nlin, vector<char>(ncol, ' '));
}

void Screen::setPixel(int x, int y)
{
    mat[x][y] = brush;
}

void Screen::clear()
{
    if(nlin==0 || ncol==0)
        return;
    else
        for(int i=0; i<nlin; i++)
            for(int j=0; j<ncol; j++)
                mat[i][j] = ' ';
}

void Screen::setBrush(char _brush)
{
    brush = _brush;
}

ostream& operator<<(ostream &os, Screen &t)
{
    if(t.nlin==0 || t.ncol==0)
        return os;
    else
        for(int i=0; i<t.nlin; i++)
        {
            for(int j=0; j<t.ncol; j++)
            {
                os << t.mat[i][j];
            }
            os << endl;
        }
    return os;
}
