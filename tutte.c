#include "tutte.h"
#include <stdio.h>

Lista_W* algo( lista_sasiedztw* m )
{
    Lista_W* lv = lv_init( m );

    struktura_scian* s = demoucron( m );
    if( s == NULL)
    {
        fprintf(stderr, "Graf nie planarny lub bez cykli\n");
        free_lv(lv);
        return NULL;
    }

    int zewn_idx = s->s_zewn;
    lv->liczba_zewn = s->len[zewn_idx];
    for (int i = 0; i < s->len[zewn_idx]; i++) 
    {
        int wierzcholek = s->sciany[zewn_idx][i];
        lv->lista[wierzcholek - 1]->czy_zewn = 1;
    }

    //wypisz_system_rotacyjny(s, m->rozmiar);
    
    /*=============================================================
    // Testy integracji demoucron-triangulacja
    fprintf(stderr, "\nTest\n");

    fprintf(stderr, "Znalezione sciany:\n");
    for(int i = 0; i<s->rozmiar; i++){
        fprintf(stderr, "%d: [ ",i);

        for(int l = 0; l < s->len[i]; l++){
            fprintf(stderr, "%d ",s->sciany[i][l]);
        }

        fprintf(stderr, "]\n");
    }
    fprintf(stderr, "Index Sciany glownej: %d\n",s->s_zewn);

    fprintf(stderr, "Przeszlo\n\n");
    =============================================================*/

    triang( lv, s );

    fpp_zewn( lv, s );

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


void triang( Lista_W* lv, struktura_scian* s )
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
    
    // Test
    /*
    fprintf(stdout, "Po triangualizacji: \n");
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
    */
}

void fpp_zewn( Lista_W* lv, struktura_scian* s ) 
{
    int zewn_idx = s->s_zewn;
    int dlugosc = s->len[zewn_idx];
    double angle = 2 * M_PI / dlugosc;
    
    for(int i = 0; i < dlugosc; i++){
        int wierzcholek = s->sciany[zewn_idx][i] - 1; 
        
        lv->lista[wierzcholek]->poz[0] = SKALA * cos( i * angle );
        lv->lista[wierzcholek]->poz[1] = SKALA * sin( i * angle );
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

//Przed nim powininen byc free_m, bo oba free korzystaja z tablicy lista_k
void free_lv(Lista_W* lv)
{
    for(int i = 0; i < lv->rozmiar; i++)
    {
        free(lv->lista[i]);
    } 
    free(lv->lista);
    free(lv);
}