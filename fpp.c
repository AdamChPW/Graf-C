#include "fpp.h"
#include <stdio.h>

Lista_W* algo( lista_sasiedztw* m )
{
    Lista_W* lv = lv_init( m );

<<<<<<< HEAD
    Sciany* s = sciany_init( lv );    //Dla Cezarego
=======
    //struktura_scian* s = sciany_init( lv );    //Dla Cezarego
    struktura_scian* s = demoucron( m );
    if( s == NULL)
    {
        fprintf(stderr, "Graf nie planarny lub bez cykli\n");
        return lv;
    }
>>>>>>> 8967c16 (demoucron)
    triang( lv, s );
    fpp_zewn( lv );

    for(int i = 0; i < MAXITER; i++)
        tpp_wewn( lv );

    free_struktura_scian(s);  
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

// Tworzy sztuczne polaczenie do triangulacji
// Zastosowanie: add_sk(baza, 1, 3, 4) -> Dodaje krawedz od 1 do 4 po wystapieniu krawedzi 3.
void add_sk(Lista_W* lv, int from, int after, int to)
{
    if(from == to){
        //fprintf(stderr, "Proba dodania krawedzi od %d do %d\n", from, to);
        return;
    }

    lista_k* new = malloc(sizeof(lista_k));
    new->nazwa = NULL;
    new->wartosc = 0;
    new->nr_wierzcholka = to;

    lista_k* temp = lv->lista[from-1]->krawedzie;

    while(1 == 1)
    {
        if(temp->nr_wierzcholka == after)
        {
            if(temp->next != NULL && temp->next->nr_wierzcholka == to){
                //fprintf(stderr, "Istnieje juz krawedz od v%d do v%d\n", from, to);
                break;
            }

            //fprintf(stdout, "Dodano szt. krawedz (v%d, v%d)\n", from, to);
            new->next = temp->next;
            temp->next = new;
            break;
        }

        temp = temp->next;
        if(temp == NULL)
        {
            fprintf(stderr, "Nieznaleziono v%d w krawedziach v%d, dodano v%d na koncu listy.\n", after, from, to);
            new->next = NULL;
            temp = new;
            break;
        }
    }
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

    s->len[0] = 3;
    s->sciany[0] = malloc(3*sizeof(int));
    s->sciany[0][0] = 1;
    s->sciany[0][1] = 2;
    s->sciany[0][2] = 4;


    s->len[1] = 3;
    s->sciany[1] = malloc(3*sizeof(int));
    s->sciany[1][0] = 1;
    s->sciany[1][1] = 4;
    s->sciany[1][2] = 5;

    s->len[2] = 4;
    s->sciany[2] = malloc(4*sizeof(int));
    s->sciany[2][0] = 1;
    s->sciany[2][1] = 5;
    s->sciany[2][2] = 6;
    s->sciany[2][3] = 2;

    s->len[3] = 6;
    s->sciany[3] = malloc(6*sizeof(int));
    s->sciany[3][0] = 2;
    s->sciany[3][1] = 3;
    s->sciany[3][2] = 2;
    s->sciany[3][3] = 4;
    s->sciany[3][4] = 5;
    s->sciany[3][5] = 6;

    return s;
}

void triang( Lista_W* lv, Sciany* s )
{
    int pivot = 0;
    int previous = 0;
    int target = 0;

    for(int i = 0; i < s->rozmiar; i++)
    {
        if(s->s_zewn == i)
            continue;

        // Dziala tylko dla scian z wiecej niz 3 wierzcholki
        // Dodaje szt. krawedzie od 3 do ostatniego punktu w scianie np. [1, 5, (6), 2] 
        // Nie wiem czy to problem jak w liscie wystepujo ogony np. [(2,3,2),4,5,6] zamiast [2,4,5,6]
        for(int kr = 2; kr < s->len[i]-1; kr++)     
        {
            pivot = s->sciany[i][0];
            target = s->sciany[i][kr];
        
            previous = s->sciany[i][kr-1];  
            add_sk(lv, pivot, previous, target);
    
            previous = s->sciany[i][s->len[i]-1];  //Poprzedni dla pierwszego wierz to ostatni.
            add_sk(lv, target, previous, pivot);
        }
    }

    for(int i = 0; i < lv->rozmiar; i++)
    {
        fprintf(stdout, "v%d [ ", i+1);
        lista_k* temp = lv->lista[i]->krawedzie;
        while(temp != NULL)
        {
            fprintf(stdout, "%d ", temp->nr_wierzcholka);
            temp = temp->next;
        }
        fprintf(stdout, "]\n");
    }
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
    double poprzednia_waga = 0;
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

                if(temp->nazwa == NULL)
                    obecna_waga = poprzednia_waga;
                else
                    obecna_waga = temp->wartosc == 0 ? 0.00000001 : temp -> wartosc;
                    
                suma_odw_wag += 1/obecna_waga;
                poprzednia_waga = obecna_waga;

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