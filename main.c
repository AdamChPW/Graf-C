#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

typedef struct l{
    char* nazwa;    //Dodalem
    double wartosc;
    int nr_wierzcholka;
    struct l* next;
}lista_k;

typedef struct m{
    int rozmiar;
    lista_k** lista;    //Zmienilem z lista* na lista **. Jak jest ktos modzejszy to niech to zmieni by dzialo z lista_k*
}lista_sasiedztw;

// Wczytuje dane do tej smiesznej macierzy / listy sasiedztw
// Ja to zrobie (Adam)
int add_k(lista_k *origin, int to, char* nazwa, double waga)
{
    lista_k *k = malloc(sizeof(lista_k));
    if( k == NULL ) return 1;
    
    k->nr_wierzcholka = to;
    k->wartosc = waga;
    k->nazwa = malloc(strlen(nazwa));
    if( strcpy(k->nazwa, nazwa) == NULL ) return 0;
 
    while(origin != NULL)
    {
        if(origin->nr_wierzcholka == to){
            fprintf(stderr, "Istmieje juz droga do v%d [%s i %s]", to, nazwa, origin->nazwa);
            return 1;
        }
        origin = origin->next;
    }

    origin = k;
    return 0;
}

lista_sasiedztw* w_dane(char* f_name)
{
    FILE* in = fopen(f_name, "r");
    if(in == NULL) return NULL;

    lista_sasiedztw* m = malloc(sizeof(lista_sasiedztw));
    if(m == NULL) return NULL;
    m->rozmiar = 0;
    m->lista = malloc(0);

    char buffer[BUFSIZE];
    while(fgets(buffer, BUFSIZE, in) != NULL){

        char nazwa[BUFSIZE];
        int v1, v2;
        double waga;

        sscanf(buffer, "%s %d %d %lf", nazwa, &v1, &v2, &waga);

        if(v1 < 1 || v2 < 1){
            fprintf(stderr, "Nr_Wierzcholka nie moze byc mniejszy od 1\n");
            return NULL;
        }
        if(waga < 0){
            fprintf(stderr, "Waga nie moze byc ujemna\n");
            return NULL;
        }

        int new_size =  v1 < v2 && m->rozmiar < v2 ? v2 :
                        m->rozmiar < v1 ? v1 : m->rozmiar;
        if(new_size != m->rozmiar){    
            m->lista = realloc(m->lista, new_size * sizeof(lista_k));
            if(m->lista == NULL) return NULL;
            m->rozmiar = new_size;
        }
        
        if(add_k( m->lista[v1-1], v2, nazwa, waga) || add_k( m->lista[v2-1], v1, nazwa, waga)){
            fprintf(stderr, "Wystapil blad przy dodawaniu krawedzi.\n");
            return NULL;
        }
    }
    return m;
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
// Jak sie komus chce to moze to rozlorzyc na kilka pliki.