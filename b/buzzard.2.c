#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define c m[m[0]++] =

char s[5000]; // String storage for the names of built-in and defined primitives
int t=64; // position of the next available space for a new string to be added
//  s[64..t] ... all the words

/*
 *  Main memory (random access), contains return stack and dictionary.
 *  Dictionary is a list of words (header, data field).
 *  header = (address of the previous word, index into string storage, code pointer)
 *
 *  m[0] ... pointer to the first empty slot (m[32..m[0]] is the dictionary)
 *  m[1] ... top of the return stack (m[?..m[1]])
 *  m[3], m[4], m[5] ... unused
 *  m[32..m[0]] ... dictionary
 */
int m[20000]={32};
int L=1; // m[L] is the last word added to main memory

int T[500]; // Stack
int S=0; // Top of the stack (TOS)
int f; // The value of T[S]

int I;
int w;

void error(char *message)
{
    printf("%s\n", message);
    exit(1);
}

/*
 * Add a new word (L, t, x) to a dictionary m[32..m[0]]
 *
 * 'L', 't' are global variables, 'x' is an argument.
 */
void a(int x)
{
    // Add a new word (L, t, x)
    c L; // m[L] is the place of the last word
    L = m[0]-1; // m[L] is now the place of this word
    c t; // s[t] is the name (string) of this word
    c x; // code pointer
    if (scanf("%s", &s[t]) != 1) error("Unexpected end of input.");
    t += strlen(&s[t])+1; // s[t] now points to the next available slot
}

void r(int x)
{
    switch(m[x++]) {
        case 0:
            T[++S] = f; f = m[I++]; break;
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
            m[f] = T[S--]; f = T[S--]; break;
        case 8: // -
            f = T[S--] - f; break;
        case 9: // *
            f *= T[S--]; break;
        case 10: // /
            f = T[S--] / f; break;
        case 11: // <0
            f = 0 > f; break;
        case 12: // exit
            I = m[m[1]--]; break;
        case 13: // echo
            putchar(f); f = T[S--]; break;
        case 14: // key
            T[++S] =f ; f = getchar(); break;
        case 15: // _pick
            f = T[S-f]; break;
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
