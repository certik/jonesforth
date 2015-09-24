#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define c m[m[0]++] =

char s[5000];
int m[20000]={32}, L=1, I, T[500], *S=T, t=64, w, f;

void error(char *message)
{
    printf("%s\n", message);
    exit(1);
}

void a(int x)
{
    int stat;
    c L;
    L = *m-1;
    c t;
    c x;
    stat = scanf("%s", s+t);
    if (stat != 1) error("Unexpected end of input.");
    t += strlen(s+t)+1;
}

void r(int x)
{
    switch(m[x++]) {
        case 0:
            *++S = f; f = m[I++]; break;
        case 1:
            c x; break;
        case 2:
            m[++m[1]] = I; I = x; break;
        case 3: // :
            a(1); c 2; break;
        case 4: // immediate
            *m -= 2; c 2; break;
        case 5: // _read
            for(w=scanf("%s",s)<1?exit(0),0:L;strcmp(s,&s[m[w+1]]);w=m[w]);
            w-1 ? r(w+2) : (c 2, c atoi(s));
            break;
        case 6: // @
            f = m[f]; break;
        case 7: // !
            m[f] = *S--; f = *S--; break;
        case 8: // -
            f = (*S--) - f; break;
        case 9: // *
            f *= *S--; break;
        case 10: // /
            f = (*S--) / f; break;
        case 11: // <0
            f = 0 > f; break;
        case 12: // exit
            I = m[m[1]--]; break;
        case 13: // echo
            putchar(f); f = *S--; break;
        case 14: // key
            *++S =f ; f = getchar(); break;
        case 15: // _pick
            f = S[-f]; break;
    }
}

int main()
{
    a(3);
    a(4);
    a(1);
    w = *m;
    c 5;
    c 2;
    I = *m;
    c w;
    c I-1;
    for (w = 6; w < 16; c w++) a(1);
    m[1] = *m;
    for (*m += 512; ; r(m[I++]));
    return 0;
}
