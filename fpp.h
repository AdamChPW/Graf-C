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

Lista_W* algo( lista_sasiedztw* m );

Lista_W* lv_init( lista_sasiedztw* m);

lista_k* add_sk(lista_k* lk, int to, double waga);

void triang( Lista_W* lv );

void fpp_zewn( Lista_W* lv );

void tpp_wewn( Lista_W* lv );

#endif