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
#include <string.h> // Potrzebne do strcmp

// Sprawdza poprawnosc macierzy pod algorytm
// Zwraca 0 jak macierz jest git, inna liczba to blad
int spr_macierz(lista_sasiedztw* l){
    if (l == NULL || l->rozmiar == 0) return 1;

    int V = l->rozmiar;
    int E = 0;

    // Liczenie krawędzi
    for (int i = 0; i < V; i++) {
        lista_k* temp = l->lista[i];
        while (temp) {
            if (i < temp->nr_wierzcholka - 1) { 
                E++;
            }
            temp = temp->next;
        }
    }

    // Sprawdzenie warunku Eulera dla V >= 3)
    if (V >= 3 && E > 3 * V - 6) {
        fprintf(stderr, "Blad: Graf ma za duzo krawedzi, byc moze nie jest planarny!\n");
        return 2; 
    }

    // Sprawdzanie unikalnosci nazw krawędzi
    char** sprawdzone_nazwy = malloc(E * sizeof(char*));
    if (sprawdzone_nazwy == NULL) {
        fprintf(stderr, "Blad alokacji pamieci przy sprawdzaniu macierzy.\n");
        return -1; 
    }
    
    int licznik_nazw = 0;

    for (int i = 0; i < V; i++) {
        lista_k* temp = l->lista[i];
        while (temp) {
            if (i < temp->nr_wierzcholka - 1) {
                for (int j = 0; j < licznik_nazw; j++) {
                    if (strcmp(sprawdzone_nazwy[j], temp->nazwa) == 0) {
                        fprintf(stderr, "Blad: Nazwa krawedzi '%s' nie jest unikalna!\n", temp->nazwa);
                        free(sprawdzone_nazwy);
                        return 3;
                    }
                }
                
                sprawdzone_nazwy[licznik_nazw] = temp->nazwa;
                licznik_nazw++;
            }
            temp = temp->next;
        }
    }

    free(sprawdzone_nazwy);
    return 0;
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

    char *ext = strrchr(argv[1], '.');
    if (ext == NULL || (strcmp(ext, ".txt") != 0 && strcmp(ext, ".csv") != 0)) {
        fprintf(stderr, "Nieobslugiwany format pliku.\n");
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