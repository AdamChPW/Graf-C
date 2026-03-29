#ifndef _wczytaj_h_
#define _wczytaj_h_

typedef struct l{
    char* nazwa;    //Dodalem
    double wartosc;
    int nr_wierzcholka;
    struct l* next; // Kompilator errora walil gdy byl alias*
}lista_k;

typedef struct m{
    int rozmiar;
    lista_k** lista;    //Zmienilem z lista* na lista**. Jak jest ktos modzejszy to niech to zmieni by dzialo z lista_k*
}lista_sasiedztw;

// Wczytuje dane do tej smiesznej macierzy / listy sasiedztw
// Ja to zrobie (Adam)
int add_k(lista_sasiedztw* m, int from, int to, char* nazwa, double waga);

lista_sasiedztw* w_dane(char* f_name);

void free_m(lista_sasiedztw *m);

#endif