#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_m 20000  // Main memory size
#define N_s  5000  // String storage size
#define N_T   500  // Stack size
#define N_rs  512  // Return stack size

#define M_CHECK(x) { if (x < 0 || x >= N_m) error("Out of memory"); }
#define T_CHECK(x) { if (x < 0) error("Stack underflow"); if (x >= N_T) error("Stack overflow"); }
#define RS_CHECK(x) { if (x-RS0 < 0) error("Return stack underflow"); if (x-RS0 >= N_rs) error("Return stack overflow"); }

#define c M_CHECK(m[0]) m[m[0]++] =

// Note: All ranges like a..b are of the usual C form [a, b), i.e. including a,
// excluding b.

char s[N_s]; // String storage for the names of built-in and defined primitives
int t=64; // position of the next available space for a new string to be added
//  s[0..64] ... temporary storage for the current word being read
//  s[64..t] ... all the words

/*
 *  Main memory (random access), contains return stack and dictionary.
 *  Dictionary is a list of words (header, data field).
 *  header = (address of the previous word, index into string storage, instruction to execute)
 *
 *  m[0] ... dictionary pointer: pointer to the first empty slot in the
 *           dictionary (m[32..m[0]] is the dictionary)
 *  m[1] ... return stack pointer: top of the return stack (m[?..m[1]])
 *  m[2] ... should always be 0 -- fake dictionary entry that means "pushint"
 *  m[3..32] ... unused (user can use as variables)
 *  The memory m[32..N_m] is used for the dictionary as well as other data
 *  (mingled together). The last dictionary word being at m[L] (and also m[L]
 *  being the first item in the header points to the position of the previous
 *  word). One writes to the memory using `c num`.
 *  m[RS0..RS0+N_rs]  ... return stack, m[1] top of the return stack. The space
 *  for the stack is reserved using:
 *      m[1] = m[0];
 *      RS0 = m[1];
 *      m[0] += N_rs;
 *  So the return stack is N_rs elements long and it starts at RS0.
 */
int m[N_m]={32};
int L=1; // m[L] is the last word added to main memory

int T[N_T]; // Stack
int S=-1; // T[S] is the value at the top of the stack (TOS).

int RS0; // Bottom of the return stack

int I; // m[I] is the code pointer to the next instruction to execute, i.e.
// m[m[I]] is the actual instruction, an integer 0..16

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
    L = m[0]-1; // m[L] is now the place of this word, i.e. the header of the
    // new word being inserted is at m[L], m[L+1], m[L+2]
    c t; // s[t] is the name (string) of this word
    c x; // code instruction, between 0..16
    if (x < 0 || x > 15) error("'x' out of range");
    if (scanf("%s", &s[t]) != 1) error("Unexpected end of input.");
    t += strlen(&s[t])+1; // s[t] now points to the next available slot
    if (t + 64 > N_s) error("String storage too small.");
}

/*
 * Execute an instruction.
 *
 * x ... code pointer (to the instruction)
 *
 * The instruction given in m[x] will be executed.
 */
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
            m[1]++;
            M_CHECK(m[1])
            RS_CHECK(m[1])
            m[m[1]] = I; I = x; break;
        case 3: // :
            a(1); c 2; break;
        case 4: // immediate
            m[0] -= 2; c 2; break;
        case 5: // _read
            if (scanf("%s", s) != 1) exit(0);
            // See if 's' is in the dictionary. The 'w' points to the current
            // word, i.e. m[w], m[w+1], m[w+2] is the header, i.e. m[w] is the
            // address of the previous word, m[w+1] is the index into string
            // storage and m[w+2] is the code pointer.
            w = L;
            while (strcmp(s, &s[m[w+1]])) w = m[w];
            if (w == 1) {
                // Term 's' not found in the dictionary. If 's' is an integer,
                // add it to stack. Otherwise error out.
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
                // The term 's' is a known dictionary word, 'w' points to it.
                // We execute the corresponding instruction given in m[w+2].
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
            I = m[m[1]--];
            RS_CHECK(m[1])
            break;
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
        default:
            error("Unknown instruction.");
            break;
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
    RS0 = m[1];
    RS_CHECK(m[1])
    m[0] += N_rs;
    for (;;) {
        M_CHECK(I)
        r(m[I++]);
    };
    return 0;
}
