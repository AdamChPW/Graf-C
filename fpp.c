#include "fpp.h"
#include <stdio.h>

Lista_W* algo( lista_sasiedztw* m )
{
    Lista_W* lv = lv_init( m );
    triang( lv );
    fpp_zewn( lv );

    for(int i = 0; i < MAXDEG; i++)
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
    new->nazwa = malloc(sizeof(char));
    new->nazwa = "";
    new->next = lk;
    new->nr_wierzcholka = to;
    new->wartosc = waga;
    return new;
}

void triang( Lista_W* lv )
{
    // Poki co nic
    lv->lista[2]->czy_zewn = 1;
    lv->lista[3]->czy_zewn = 1;
    lv->lista[4]->czy_zewn = 1;
    lv->lista[5]->czy_zewn = 1;
    lv->liczba_zewn = 4;


    lv->lista[2]->krawedzie = add_sk(lv->lista[2]->krawedzie, 4, 18);
    lv->lista[3]->krawedzie = add_sk(lv->lista[3]->krawedzie, 3, 18);

    lv->lista[2]->krawedzie = add_sk(lv->lista[2]->krawedzie, 6, 10);
    lv->lista[5]->krawedzie = add_sk(lv->lista[5]->krawedzie, 3, 10);

    lv->lista[0]->krawedzie = add_sk(lv->lista[0]->krawedzie, 6, 4);
    lv->lista[5]->krawedzie = add_sk(lv->lista[5]->krawedzie, 1, 4);
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
            lv->lista[i]->poz[0] = suma_x * suma_odw_wag;
            lv->lista[i]->poz[1] = suma_y * suma_odw_wag;
        }
    }
}