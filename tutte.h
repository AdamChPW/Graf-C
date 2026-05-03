#ifndef _TUTTE_H_
#define _TUTTE_H_
    
#include <stdlib.h>
#include <math.h>
#include "wczytaj.h"

#define SKALA 5
#define MAXITER 1000
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


typedef struct s {
    int rozmiar;
    int s_zewn;   // index sciany zewn
    int* len;     // dlugosci scian
    int** sciany; // wierzcholki scian
} struktura_scian;

Lista_W* algo( lista_sasiedztw* m );

Lista_W* lv_init( lista_sasiedztw* m);

void add_sk(Lista_W* lv, int target, int after, int to);

struktura_scian* demoucron(lista_sasiedztw* graf);

void triang( Lista_W* lv, struktura_scian* s );

void tutte_zewn( Lista_W* lv, struktura_scian* s );

void tutte_wewn( Lista_W* lv );

void free_lv( Lista_W* lv );

void free_struktura_scian(struktura_scian* s);

void wypisz_system_rotacyjny(struktura_scian* s, int V);


#endif