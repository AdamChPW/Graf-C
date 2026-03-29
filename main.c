#include <stdio.h>
#include <stdlib.h>

#include "wczytaj.h"


void test(lista_sasiedztw* m){
    for(int i = 0; i < m->rozmiar; i++){
        printf("v%d[ ", i+1);
        lista_k* temp  = m->lista[i];
        while(temp){
            printf("%s ",temp->nazwa);
            temp = temp->next;
        }
        printf("]\n");
    }
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
int main(int argc, char** argv)
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
    //test(macierz);

    if( spr_macierz(macierz) != 0 ){
        fprintf(stderr, "Macierz nie jest odpowiednia do algorytmu.\n");
        free_m(macierz);
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

    free_m(macierz);
    fclose(out);
    return 0;
}