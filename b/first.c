#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_m 20000
#define N_s 5000
#define N_T 500

#define M_CHECK(x) { if (x < 0 || x >= N_m) error("Out of memory"); }
#define T_CHECK(x) { if (x < 0) error("Stack underflow"); if (x >= N_T) error("Stack overflow"); }

#define c M_CHECK(m[0]) m[m[0]++] =

char s[N_s]; // String storage for the names of built-in and defined primitives
int t=64; // position of the next available space for a new string to be added
//  s[0..64] ... temporary storage for the current word being read
//  s[64..t] ... all the words

/*
 *  Main memory (random access), contains return stack and dictionary.
 *  Dictionary is a list of words (header, data field).
 *  header = (address of the previous word, index into string storage, code pointer)
 *
 *  m[0] ... dictionary pointer: pointer to the first empty slot in the
 *           dictionary (m[32..m[0]] is the dictionary)
 *  m[1] ... return stack pointer: top of the return stack (m[?..m[1]])
 *  m[2] ... should always be 0 -- fake dictionary entry that means "pushint"
 *  m[3], m[4], m[5] ... unused (user can use as variables)
 *  m[32..m[0]] ... dictionary
 */
int m[N_m]={32};
int L=1; // m[L] is the last word added to main memory

int T[N_T]; // Stack
int S=0; // T[S] is the top of the stack (TOS)

int I; // m[I] is the next instruction in the instruction stream

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
    if (t + 64 > N_s) error("String storage too small.");
}

void r(int x)
{
    int w;
    M_CHECK(x)
    switch(m[x++]) {
        case 0: // pushint
            S++;
            M_CHECK(I)
            T_CHECK(S)
            T[S] = m[I++]; break;
        case 1: // compile me
            c x; break;
        case 2: // run me
            M_CHECK(m[1]+1)
            m[++m[1]] = I; I = x; break;
        case 3: // :
            a(1); c 2; break;
        case 4: // immediate
            m[0] -= 2; c 2; break;
        case 5: // _read
            if (scanf("%s", s) != 1) exit(0);
            w = L;
            while (strcmp(s, &s[m[w+1]])) w = m[w];
            if (w == 1) {
                int i;
                char *ptr;
                c 2;
                i = strtol(s, &ptr, 10);
                if (ptr == s) {
                    printf("word: %s\n", s);
                    error("Unknown word");
                }
                c i;
            } else {
                r(w+2);
            }
            break;
        case 6: // @
            T_CHECK(S)
            M_CHECK(T[S])
            T[S] = m[T[S]]; break;
        case 7: // !
            T_CHECK(S-1)
            T_CHECK(S)
            M_CHECK(T[S])
            m[T[S]] = T[S-1]; S--; S--; break;
        case 8: // -
            T_CHECK(S-1)
            T_CHECK(S)
            T[S-1] = T[S-1] - T[S]; S--; break;
        case 9: // *
            T_CHECK(S-1)
            T_CHECK(S)
            T[S-1] = T[S-1] * T[S]; S--; break;
        case 10: // /
            T_CHECK(S-1)
            T_CHECK(S)
            T[S-1] = T[S-1] / T[S]; S--; break;
        case 11: // <0
            T_CHECK(S)
            T[S] = 0 > T[S]; break;
        case 12: // exit
            M_CHECK(m[1])
            I = m[m[1]--]; break;
        case 13: // echo
            T_CHECK(S)
            putchar(T[S]); S--; break;
        case 14: // key
            S++;
            T_CHECK(S)
            T[S] = getchar(); break;
        case 15: // _pick
            T_CHECK(S)
            T_CHECK(S-T[S])
            T[S] = T[S-T[S]]; break;
    }
}

int main()
{
    int i;
    a(3);
    a(4);
    a(1);
    i = m[0];
    c 5;
    c 2;
    I = m[0];
    c i;
    c I-1;
    for (i = 6; i < 16; ) {
        a(1);
        c i++;
    }
    m[1] = m[0];
    m[0] += 512;
    for (;;) {
        M_CHECK(I)
        r(m[I++]);
    };
    return 0;
}
