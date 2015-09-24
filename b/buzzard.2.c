#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define c m[m[0]++] =
#define z ;break;case

char s[5000];
int m[20000]={32}, L=1, I, T[500], *S=T, t=64, w, f;

a(x)
{
   c L;
   L = *m-1;
   c t;
   c x;
   scanf("%s", s+t);
   t += strlen(s+t)+1;
}

r(x)
{
   switch(m[x++]) {
	z 0:	*++S = f; f = m[I++]
	z 1:	c x
	z 2:	m[++m[1]] = I; I = x

    // :
	z 3:	a(1); c 2

    // immediate
	z 4:	*m -= 2; c 2

    // _read
	z 5:	for(w=scanf("%s",s)<1?exit(0),0:L;strcmp(s,&s[m[w+1]]);w=m[w]);
            w-1 ? r(w+2) : (c 2, c atoi(s))
    // @
	z 6:	f = m[f]

    // !
	z 7:	m[f] = *S--; f = *S--

    // -
	z 8:	f = (*S--) - f

    // *
	z 9:	f *= *S--

    // /
	z 10:	f = (*S--) / f

    // <0
	z 11:	f = 0 > f

    // exit
	z 12:	I = m[m[1]--]

    // echo
	z 13:	putchar(f); f = *S--

    // key
	z 14:	*++S =f ; f = getchar();

    // _pick
	z 15:	f = S[-f];
   }
}

main()
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
   for (w=6; w<16; )
      a(1), c w++;
   m[1] = *m;
   for (*m+=512; ; r(m[I++]));
}
