#ifndef _TUTTE_H_
#define _TUTTE_H_

#include <stdlib.h>
#include <math.h>
#include "wczytaj.h"
#include "cykl.h" // Zawiera definicję struktury 'cycle'

#define SKALA 100
#define MAXITER 10000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Struktura wierzchołka przechowująca pozycję i metadane
typedef struct v {
    double poz[2];       // [x, y]
    int czy_zewn;        // Czy należy do ramy (stały punkt)
    int nazwa;           // Oryginalna nazwa z pliku (np. 60)
    lista_k* krawedzie;  // Lista sąsiadów
} Wierzch;

// Główna struktura zarządzająca punktami
typedef struct lv {
    int rozmiar;
    int liczba_zewn;
    Wierzch **lista;
} Lista_W;

// --- Prototypy funkcji ---

Lista_W* algo(lista_sasiedztw* m);
Lista_W* lv_init(lista_sasiedztw* m);

// Funkcja pomocnicza: mapuje nazwę wierzchołka na jego aktualny indeks w tablicy
int znajdz_id_po_nazwie(Lista_W* lv, int szukana_nazwa);
void dfs_szukaj_ramy(lista_sasiedztw *list, int u, int start, int *odwiedzone, int *sciezka, int dlugosc, cycle **znaleziona_rama);
cycle* get_guaranteed_frame(lista_sasiedztw *list);

void tutte_zewn_z_cyklu(Lista_W* lv, cycle* rama);
void tutte_wewn(Lista_W* lv);
void free_lv(Lista_W* lv);

#endif