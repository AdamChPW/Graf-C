#include <stdio.h>
#include <stdlib.h>

typedef struct l{
    double wartosc;
    int nr_wierzcholka;
    lista_k* next;
}lista_k;

typedef struct m{
    int rozmiar;
    lista_k* lista;
}lista_sasiedztw;

// Wczytuje dane do tej smiesznej macierzy / listy sasiedztw
lista_sasiedztw* w_dane(char* f_name){
    return NULL;
}

// Sprawdza poprawnosc macierzy pod algorym
// Zwraca 0 jak macierz jest git
int spr_macierz(){
    return -1;
}

// Wypisuje dane do pliku f
// Zwraca 0 jak jest git (brakuje mi bool'a)
int wyp_dane( FILE *f ){
    return 1;
}

// Main. Plis zostawcie maina jak najbardziej czytelnym
void main(int argc, char** argv)
{
    if(argc < 2){
        fprintf(stderr, "Nie podano wejscie.\n");
        return 1;
    }

    lista_sasiedztw* macierz = w_dane(argv[1]);
    if( macierz == NULL ){
        fprintf(stderr, "Nie udalo sie utworzyc macierzy.\n");
        return 2;
    }

    if( spr_macierz(macierz) != 0 ){
        fprintf(stderr, "Macierz nie jest odpowiednia do algorytmu.\n");
        return 3;
    }

    //algo(macierz); Nie robimy

    FILE *out = argc > 2 ? fopen(argv[2], "w") : fopen("wyjscie", "w");

    if( wyp_dane( out ) == 0 )
        fprintf(stdout, "Wypisano odpowiedz do pliku.\n");
    else {
        fprintf(stderr, "Nie udalo sie zapisac odpowidzi");
        return 4;
    }

    return 0;
}