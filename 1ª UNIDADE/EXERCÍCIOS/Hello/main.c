#include <stdio.h>

int soma(int a, int b);

int main()
{
    int x=2,y=3, z;
    z = soma(x,y);
    printf("A soma dos valores é: %d", z,"endl");
    return 0;
}

int soma(int a, int b)
{
    return a+b;
}
