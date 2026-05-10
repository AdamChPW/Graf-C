#ifndef _STRUCTS_H_

#define _STRUCTS_H_

typedef struct l{
    char* nazwa; 
    double wartosc;
    int nr_wierzcholka_start;  //changed that
    int nr_wierzcholka_cel; 
    struct l* next;
}lista_k;

typedef struct m{
    int rozmiar;
    lista_k **lista;
}lista_sasiedztw;


#endif