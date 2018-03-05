#include <stdio.h>

int main()
{
    int i=5, j=3, *p, *q;
    i = *&*&j;
    q = &i;
    p = &j;
    i = ((*p)++) + *q;

    printf("%d", i);

    return 0;
}
