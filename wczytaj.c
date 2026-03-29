#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wczytaj.h"

#define BUFSIZE 1024

int add_k(lista_sasiedztw* m, int from, int to, char* nazwa, double waga)
{
    lista_k *k = malloc(sizeof(lista_k));
    if( k == NULL )
        return 1;
    
    k->nr_wierzcholka = to;
    k->wartosc = waga;
    k->nazwa = malloc(strlen(nazwa) + 1);
    if(k->nazwa == NULL) {
        free(k);
        return 1;
    }
    if( strcpy(k->nazwa, nazwa) == NULL ) {
        free(k->nazwa);
        free(k);
        return 1;
    }
    k->next = NULL;

    lista_k* temp = m->lista[from-1];
    while(temp != NULL)
    {
        if(temp->nr_wierzcholka == to){
            fprintf(stderr, "Istmieje juz droga z v%d do v%d", from, to);
            free(temp);
            free(k->nazwa);
            free(k);
            return 1;
        }
        temp = temp->next;
    }

    k->next = m->lista[from-1];
    m->lista[from-1] = k;

    return 0;
}

lista_sasiedztw* w_dane(char* f_name)
{
    FILE* in = fopen(f_name, "r");
    if(in == NULL)
        return NULL;

    lista_sasiedztw* m = malloc(sizeof(lista_sasiedztw));
    if(m == NULL){
        fclose(in);
        return NULL;
    }
    m->rozmiar = 0;

    char buffer[BUFSIZE];
    while(fgets(buffer, BUFSIZE, in) != NULL){

        char nazwa[BUFSIZE];
        int v1, v2;
        double waga;

        sscanf(buffer, "%s %d %d %lf", nazwa, &v1, &v2, &waga);

        if(v1 < 1 || v2 < 1){
            fprintf(stderr, "Nr_Wierzcholka nie moze byc mniejszy od 1\n"); 
            free_m(m);
            fclose(in);
            return NULL;
        }
        if(waga < 0){
            fprintf(stderr, "Waga nie moze byc ujemna\n");
            free_m(m);
            fclose(in);
            return NULL;
        }

        int new_size =  v1 < v2 && m->rozmiar < v2 ? v2 :
                        m->rozmiar < v1 ? v1 : m->rozmiar;
        if(new_size != m->rozmiar){    
            m->lista = realloc(m->lista, new_size * sizeof(lista_k*));
            if(m->lista == NULL) {
                free_m(m);
                fclose(in);
                return NULL;
            }
            m->rozmiar = new_size;
        }
        
        if(add_k( m, v1, v2, nazwa, waga) || add_k( m, v2, v1, nazwa, waga)){
            fprintf(stderr, "Wystapil blad przy dodawaniu krawedzi.\n");
            free_m(m);
            fclose(in);
            return NULL;
        }
    }

    fclose(in);
    return m;
}

void free_m(lista_sasiedztw *m)
{
    for(int i = 0; i< m->rozmiar; i++){
        while(m->lista[i])
        {
            lista_k* temp = m->lista[i];
            free(temp->nazwa);
            m->lista[i] = temp->next;
            free(temp);
        }
    }
    free(m->lista);
    free(m);
}