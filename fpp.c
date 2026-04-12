#include "fpp.h"
#include <stdio.h>

Lista_W* algo( lista_sasiedztw* m )
{
    Lista_W* lv = lv_init( m );

    Sciany* s = sciany_init( lv );    //Dla Cezarego
    triang( lv, s );

    fpp_zewn( lv );

    for(int i = 0; i < MAXITER; i++)
        tpp_wewn( lv );

    return lv;
}

Lista_W* lv_init( lista_sasiedztw* m )
{
    Lista_W* lv = malloc(sizeof(Lista_W));
    lv->rozmiar = m->rozmiar;
    lv->liczba_zewn = 0;
    lv->lista = malloc(lv->rozmiar * sizeof(Wierzch*));

    for(int i = 0; i< lv->rozmiar; i++){
        Wierzch* v = malloc(sizeof(Wierzch));
        v->poz[0] = 0;
        v->poz[1] = 0;
        v->czy_zewn = 0;
        v->krawedzie = m->lista[i];
        
        lv->lista[i] = v;
    }

    return lv;
}

lista_k* add_sk(lista_k* lk, int to, double waga)
{
    lista_k* new = malloc(sizeof(lista_k));
    new->nazwa = NULL;
    new->next = lk;
    new->nr_wierzcholka = to;
    new->wartosc = waga;
    return new;
}

// Dla cezarego
Sciany* sciany_init(Lista_W* lv)
{
    // Brute force poki co
    lv->lista[1]->czy_zewn = 1;
    lv->lista[2]->czy_zewn = 1;
    lv->lista[3]->czy_zewn = 1;
    lv->lista[4]->czy_zewn = 1;
    lv->lista[5]->czy_zewn = 1;
    lv->liczba_zewn = 5;

    Sciany* s = malloc(sizeof(Sciany));
    s->rozmiar = 4;
    s->s_zewn = 3;
    s->len = malloc(4*sizeof(int));
    s->sciany = malloc(4*sizeof(int*));

    int s1[3] = {1,2,4};
    s->len[0] = 3;
    s->sciany[0] = s1;
    int s2[3] = {1,4,5};
    s->len[1] = 3;
    s->sciany[1] = s2;
    int s3[4] = {1,5,6,2};
    s->len[2] = 4;
    s->sciany[2] = s3;
    int s4[7] = {1,2,3,2,4,5,6};
    s->len[3] = 7;
    s->sciany[3] = s4;

    return s;
}

void triang( Lista_W* lv, Sciany* s )
{
    // To ja zrobie
}

void fpp_zewn( Lista_W* lv ) 
{
    double angle = 2 * M_PI / lv->liczba_zewn;
    
    int k = 0;
    for(int i = 0; i<lv->rozmiar; i++){
        if(lv->lista[i]->czy_zewn == 1){
            lv->lista[i]->poz[0] = SKALA * cos( k * angle );
            lv->lista[i]->poz[1] = SKALA * sin( k * angle );
            k++;
        }
    }
}

void tpp_wewn( Lista_W* lv )
{
    double obecna_waga;
    double suma_odw_wag;
    double suma_x;
    double suma_y;

    for(int i = 0; i<lv->rozmiar; i++){
        if(lv->lista[i]->czy_zewn == 0)
        {
            suma_odw_wag = 0;
            suma_x = 0;
            suma_y = 0;

            lista_k* temp = lv->lista[i]->krawedzie;
            while(temp != NULL){
                obecna_waga = temp->wartosc == 0 ? 0.00000001 : temp -> wartosc;
                suma_odw_wag += 1/obecna_waga;
                int target = temp->nr_wierzcholka - 1;
                suma_x += (1/obecna_waga) * lv->lista[target]->poz[0];
                suma_y += (1/obecna_waga) * lv->lista[target]->poz[1];
                temp = temp->next;
            }
            lv->lista[i]->poz[0] = suma_x / suma_odw_wag;
            lv->lista[i]->poz[1] = suma_y / suma_odw_wag;
        }
    }
}