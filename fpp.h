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


typedef struct s {
    int rozmiar;
    int s_zewn;   // index sciany zewn
    int* len;     // dlugosci scian
    int** sciany; // wierzcholki scian
} struktura_scian;

Lista_W* algo( lista_sasiedztw* m );

Lista_W* lv_init( lista_sasiedztw* m);

void add_sk(Lista_W* lv, int target, int after, int to);

struktura_scian* sciany_init(Lista_W* lv);

void triang( Lista_W* lv, struktura_scian* s );

void fpp_zewn( Lista_W* lv );

void tpp_wewn( Lista_W* lv );

#endif