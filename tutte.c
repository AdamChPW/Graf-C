#include "tutte.h"
#include <stdio.h>
#include <math.h>

// Główna logika algorytmu
Lista_W* algo(lista_sasiedztw* m) {
    Lista_W* lv = lv_init(m);

    cycle* rama = get_guaranteed_frame(m);

    if (rama == NULL) {
        fprintf(stderr, "Blad: Nie znaleziono poprawnej ramy w grafie\n");
        free_lv(lv);
        return NULL;
    }

    // Konfiguracja ramy zewnętrznej
    lv->liczba_zewn = rama->rozmiar;
    for (int i = 0; i < rama->rozmiar; i++) {
        int id = rama->wierzcholki[i]; 
        lv->lista[id]->czy_zewn = 1;
    }

    // Ustawienie ramy na okręgu
    tutte_zewn_z_cyklu(lv, rama);

    // Relaksacja 
    for(int i = 0; i < MAXITER; i++) {
        tutte_wewn(lv);
    }

    free(rama->wierzcholki);
    free(rama);

    return lv;
}

// Przeszukiwanie z nawrotami w celu znalezienia jakiejkolwiek pętli,
// która po docięciu da poprawną ramę bez odcinania reszty grafu.
void dfs_szukaj_ramy(lista_sasiedztw *list, int u, int start, int *odwiedzone, int *sciezka, int dlugosc, cycle **znaleziona_rama) {
    if (*znaleziona_rama != NULL) return; 

    odwiedzone[u] = 1;
    sciezka[dlugosc] = u;
    dlugosc++;

    lista_k *edge = list->lista[u];
    while (edge != NULL) {
        int v = find_id_in_list(list, edge->nr_wierzcholka_cel);
        if (v != -1) {
            if (v == start && dlugosc >= 3) {
                
                cycle *kandydat = calloc(1, sizeof(cycle));
                kandydat->rozmiar = dlugosc;
                kandydat->wierzcholki = calloc(dlugosc, sizeof(int));
                for (int i = 0; i < dlugosc; i++) kandydat->wierzcholki[i] = sciezka[i];

                // docinanie (pozbycie się cięciw)
                cycle *dociety = kandydat;
                while (1) {
                    cycle *nowy = split_if_shortcut(list, dociety);
                    if (nowy == dociety) break;
                    dociety = nowy;
                }

                
                if (is_connected_without_cycle(list, dociety)) {
                    *znaleziona_rama = dociety; 
                    odwiedzone[u] = 0;
                    return;
                } 
                else {
                    free(dociety->wierzcholki);
                    free(dociety);
                }
            } 
            else if (!odwiedzone[v]) {
                dfs_szukaj_ramy(list, v, start, odwiedzone, sciezka, dlugosc, znaleziona_rama);
                if (*znaleziona_rama != NULL) {
                    odwiedzone[u] = 0;
                    return;
                }
            }
        }
        edge = edge->next;
    }
    // Cofamy się - odznaczamy węzeł
    odwiedzone[u] = 0; 
}

// wrapper
cycle* get_guaranteed_frame(lista_sasiedztw *list) {
    int *odwiedzone = calloc(list->rozmiar, sizeof(int));
    int *sciezka = calloc(list->rozmiar, sizeof(int));
    cycle *best = NULL;

    for (int i = 0; i < list->rozmiar && best == NULL; i++) {
        dfs_szukaj_ramy(list, i, i, odwiedzone, sciezka, 0, &best);
    }

    free(odwiedzone);
    free(sciezka);
    return best;
}


// Inicjalizacja listy wierzchołków
Lista_W* lv_init(lista_sasiedztw* m) {
    Lista_W* lv = malloc(sizeof(Lista_W));
    lv->rozmiar = m->rozmiar;
    lv->liczba_zewn = 0;
    lv->lista = malloc(lv->rozmiar * sizeof(Wierzch*));

    for(int i = 0; i < lv->rozmiar; i++) {
        Wierzch* v = malloc(sizeof(Wierzch));
        v->poz[0] = 0;
        v->poz[1] = 0;
        v->czy_zewn = 0;
        v->krawedzie = m->lista[i];
        
        v->nazwa = m->lista[i]->nr_wierzcholka_start; 
        lv->lista[i] = v;
    }

    return lv;
}


int znajdz_id_po_nazwie(Lista_W* lv, int szukana_nazwa) {
    for (int i = 0; i < lv->rozmiar; i++) {
        if (lv->lista[i]->nazwa == szukana_nazwa) {
            return i;
        }
    }
    return -1;
}

// Rozmieszczanie ramy na okręgu
void tutte_zewn_z_cyklu(Lista_W* lv, cycle* rama) {
    int n = rama->rozmiar;
    double angle = 2.0 * M_PI / n;
    
    for(int i = 0; i < n; i++) {
        int id = rama->wierzcholki[i]; 
        lv->lista[id]->poz[0] = SKALA * cos(i * angle);
        lv->lista[id]->poz[1] = SKALA * sin(i * angle);
    }
}

// Obliczanie średniej pozycji sąsiadów
void tutte_wewn(Lista_W* lv) {
    for(int i = 0; i < lv->rozmiar; i++) {
        // Pominięcie wierzchołków na kole
        if(lv->lista[i]->czy_zewn == 0) {
            double suma_x = 0;
            double suma_y = 0;
            double suma_wag = 0;

            lista_k* temp = lv->lista[i]->krawedzie;
            while(temp != NULL) {
                int target_id = znajdz_id_po_nazwie(lv, temp->nr_wierzcholka_cel);

                if (target_id != -1) {
                    // Obsługa wag (jeśli waga = 0, traktujemy jako małą wartość)
                    double waga = (temp->wartosc <= 0) ? 1.0 : 1.0 / temp->wartosc;
                    
                    suma_x += waga * lv->lista[target_id]->poz[0];
                    suma_y += waga * lv->lista[target_id]->poz[1];
                    suma_wag += waga;
                }
                temp = temp->next;
            }

            if(suma_wag > 0) {
                lv->lista[i]->poz[0] = suma_x / suma_wag;
                lv->lista[i]->poz[1] = suma_y / suma_wag;
            }
        }
    }
}

void free_lv(Lista_W* lv) {
    if (lv == NULL) return;
    for(int i = 0; i < lv->rozmiar; i++) {
        free(lv->lista[i]);
    } 
    free(lv->lista);
    free(lv);
}