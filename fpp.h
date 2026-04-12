#ifndef _FPP_H_
#define _FPP_H_
    
#include <stdlib.h>
#include <math.h>
#include "wczytaj.h"

#define SKALA 5
#define MAXITER 100
#define MAXDEG 16

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

lista_k* add_sk(lista_k* lk, int to, double waga);

Sciany* sciany_init(Lista_W* lv);

void triang( Lista_W* lv, Sciany* s );

void fpp_zewn( Lista_W* lv );

void tpp_wewn( Lista_W* lv );

#endif