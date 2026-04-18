#ifndef _FPP_H_
#define _FPP_H_
    
#include <stdlib.h>
#include <math.h>
#include "wczytaj.h"

#define SKALA 5
#define MAXITER 100
#define MAXDEG 16
#ifndef M_PI
#define M_PI 3.14159265358979323846 //--pedantic
#endif

typedef struct v
{
    double poz[2];  // [x, y]
    int czy_zewn;   // bool
    lista_k* krawedzie;
} Wierzch;

typedef struct lv
{
    int rozmiar;
    int liczba_zewn;
    Wierzch **lista;
} Lista_W;

typedef struct s{
       int rozmiar;
       int s_zewn;   // index sciany zewn = 3
       int* len;
       int** sciany;     // { [1,2,4], [1,4,5], [1,5,6,2], [2,3,2,4,5,6] }
}Sciany;

Lista_W* algo( lista_sasiedztw* m );

Lista_W* lv_init( lista_sasiedztw* m);

void add_sk(Lista_W* lv, int target, int after, int to);

Sciany* sciany_init(Lista_W* lv);

<<<<<<< HEAD
void triang( Lista_W* lv, Sciany* s );
=======
struktura_scian* demoucron(lista_sasiedztw* graf);

struktura_scian* demoucron(lista_sasiedztw* graf);

void triang( Lista_W* lv, struktura_scian* s );
>>>>>>> 8967c16 (demoucron)

void fpp_zewn( Lista_W* lv );

void tpp_wewn( Lista_W* lv );

void free_struktura_scian(struktura_scian* s);
#endif