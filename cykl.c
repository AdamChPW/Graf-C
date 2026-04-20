#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "wczytaj.h"
#include "fpp.h"

typedef struct {
    int* wierzcholki; 
    int rozmiar;
} Sciana;

typedef struct {
    int* wierzcholki;
    int ile_wierzcholkow;
    int* punkty_styku;
    int ile_styku;
    int* pasujace_sciany;
    int ile_pasuje;
} Segment;

int start_cyklu = -1;
int koniec_cyklu = -1;

// Funkcje pomocnicze

int czy_w_tablicy(int* tablica, int rozmiar, int wartosc) {
    for (int i = 0; i < rozmiar; i++) if (tablica[i] == wartosc) return 1;
    return 0;
}

int czy_w_scianie(Sciana* sciana, int v) {
    for (int i = 0; i < sciana->rozmiar; i++) if (sciana->wierzcholki[i] == v) return 1;
    return 0;
}

int znajdz_indeks_w_scianie(Sciana* sciana, int v) {
    for (int i = 0; i < sciana->rozmiar; i++) if (sciana->wierzcholki[i] == v) return i;
    return -1;
}

void dfs_szukaj_cyklu(lista_sasiedztw* graf, int u, int p, int* kolor, int* rodzic) {
    kolor[u] = 1;
    rodzic[u] = p;
    lista_k* sasiad = graf->lista[u];
    
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka - 1;
        if (v == p) { sasiad = sasiad->next; continue; }
        if (kolor[v] == 1) {
            if (start_cyklu == -1) { start_cyklu = v; koniec_cyklu = u; }
            return;
        } else if (kolor[v] == 0) {
            dfs_szukaj_cyklu(graf, v, u, kolor, rodzic);
            if (start_cyklu != -1) return;
        }
        sasiad = sasiad->next;
    }
    kolor[u] = 2;
}

void eksploruj_segment(lista_sasiedztw* graf, int u, int* narysowane, int* odwiedzone, Segment* seg) {
    odwiedzone[u] = 1;
    seg->wierzcholki[seg->ile_wierzcholkow++] = u;
    lista_k* sasiad = graf->lista[u];
    
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka - 1;
        if (narysowane[v] == 1) {
            if (!czy_w_tablicy(seg->punkty_styku, seg->ile_styku, v)) {
                seg->punkty_styku[seg->ile_styku++] = v;
            }
        } else if (narysowane[v] == 0 && odwiedzone[v] == 0) {
            eksploruj_segment(graf, v, narysowane, odwiedzone, seg);
        }
        sasiad = sasiad->next;
    }
}

int dfs_sciezka(lista_sasiedztw* graf, int u, int cel_v, int* narysowane, int* odwiedzone, int* sciezka, int* dlugosc, Segment* seg) {
    odwiedzone[u] = 1;
    sciezka[(*dlugosc)++] = u;

    if (u == cel_v && (*dlugosc) > 1) {
        odwiedzone[u] = 0;
        return 1;
    }

    lista_k* sasiad = graf->lista[u];
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka - 1;

        int can_visit = 0;
        if (!odwiedzone[v]) {
            if (czy_w_tablicy(seg->wierzcholki, seg->ile_wierzcholkow, v)) can_visit = 1;
        }
        if (v == cel_v && (*dlugosc) > 1) {
            can_visit = 1;
        }

        if (can_visit) {
            if (dfs_sciezka(graf, v, cel_v, narysowane, odwiedzone, sciezka, dlugosc, seg)) return 1;
        }
        sasiad = sasiad->next;
    }

    (*dlugosc)--;
    odwiedzone[u] = 0;
    return 0;
}

void krok_d_rozdziel_sciane(Sciana* sciany, int* liczba_scian, int index_sciany, int* sciezka, int dl_sciezki) {
    Sciana stara = sciany[index_sciany];
    int idx_A = znajdz_indeks_w_scianie(&stara, sciezka[0]);
    int idx_B = znajdz_indeks_w_scianie(&stara, sciezka[dl_sciezki - 1]);

    Sciana nowa1, nowa2;
    nowa1.wierzcholki = malloc((stara.rozmiar + dl_sciezki) * sizeof(int));
    nowa2.wierzcholki = malloc((stara.rozmiar + dl_sciezki) * sizeof(int));
    nowa1.rozmiar = 0; nowa2.rozmiar = 0;

    int curr = idx_A;
    while (curr != idx_B) {
        nowa1.wierzcholki[nowa1.rozmiar++] = stara.wierzcholki[curr];
        curr = (curr + 1) % stara.rozmiar;
    }
    nowa1.wierzcholki[nowa1.rozmiar++] = stara.wierzcholki[idx_B];
    for (int i = dl_sciezki - 2; i >= 1; i--) nowa1.wierzcholki[nowa1.rozmiar++] = sciezka[i];

    curr = idx_B;
    while (curr != idx_A) {
        nowa2.wierzcholki[nowa2.rozmiar++] = stara.wierzcholki[curr];
        curr = (curr + 1) % stara.rozmiar;
    }
    nowa2.wierzcholki[nowa2.rozmiar++] = stara.wierzcholki[idx_A];
    for (int i = 1; i < dl_sciezki - 1; i++) nowa2.wierzcholki[nowa2.rozmiar++] = sciezka[i];

    free(sciany[index_sciany].wierzcholki);
    sciany[index_sciany] = nowa1;
    sciany[*liczba_scian] = nowa2;
    (*liczba_scian)++;
}

// Funkcja zwalniająca strukturę ścian
void free_struktura_scian(struktura_scian* s) {
    if (s == NULL) return;
    for (int i = 0; i < s->rozmiar; i++) {
        free(s->sciany[i]);
    }
    free(s->len);
    free(s->sciany);
    free(s);
}


void wypisz_system_rotacyjny(struktura_scian* s, int V) {
    printf("\nPlanar Embedding posortowani sasiedzi anticlockwise\n");
    
    //dla każdego wierzchołka w grafie
    for (int v = 1; v <= V; v++) {
        printf("v%d [ ", v);
        
        int* nastepny = calloc(V + 1, sizeof(int));
        int pierwszy_sasiad = 0;

        //szukamy wierzchołka v w każdej ścianie
        for (int i = 0; i < s->rozmiar; i++) {
            int len = s->len[i];
            for (int j = 0; j < len; j++) {
                if (s->sciany[i][j] == v) {
                    int u = s->sciany[i][(j - 1 + len) % len];
                    int w = s->sciany[i][(j + 1) % len];
                    
                    nastepny[w] = u;
                    pierwszy_sasiad = w;
                }
            }
        }
        //wypisujemy sąsiadów w kółko
        if (pierwszy_sasiad != 0) {
            int curr = pierwszy_sasiad;
            do {
                printf("%d ", curr);
                curr = nastepny[curr];
            } while (curr != pierwszy_sasiad && curr != 0);
        }
        printf("]\n");
        free(nastepny);
    }
    printf("\n\n");
}

// GŁÓWNY ALGORYTM DEMOUCRONA
struktura_scian* demoucron(lista_sasiedztw* graf) {
    int V = graf->rozmiar;
    // ZMIANA: in_drawing -> narysowane
    int* narysowane = calloc(V, sizeof(int));
    int* kolor = calloc(V, sizeof(int));
    int* rodzic = malloc(V * sizeof(int));
    for(int i=0; i<V; i++) rodzic[i] = -1;

    start_cyklu = -1;
    koniec_cyklu = -1;
    
    for (int i = 0; i < V; i++) {
        if (kolor[i] == 0 && start_cyklu == -1) dfs_szukaj_cyklu(graf, i, -1, kolor, rodzic);
    }

    if (start_cyklu == -1) { 
        fprintf(stderr, "BLAD: Brak cykli w grafie (drzewo/las).\n"); 
        free(kolor); free(rodzic); free(narysowane);
        return NULL; 
    }

    int* cykl = malloc(V * sizeof(int));
    int dl_cyklu = 0;
    int curr = koniec_cyklu;
    while (curr != start_cyklu) { cykl[dl_cyklu++] = curr; curr = rodzic[curr]; }
    cykl[dl_cyklu++] = start_cyklu;

    for (int i = 0; i < dl_cyklu; i++) narysowane[cykl[i]] = 1;

    // 2*V (Euler)
    Sciana* sciany = malloc((2 * V) * sizeof(Sciana)); 
    int liczba_scian = 2;
    
    sciany[0].wierzcholki = malloc(dl_cyklu * sizeof(int));
    sciany[1].wierzcholki = malloc(dl_cyklu * sizeof(int));
    sciany[0].rozmiar = sciany[1].rozmiar = dl_cyklu;
    for(int i=0; i<dl_cyklu; i++) { 
        sciany[0].wierzcholki[i] = cykl[i]; 
        sciany[1].wierzcholki[i] = cykl[i]; 
    }

    // GŁÓWNA PĘTLA UKŁADANIA SEGMENTÓW
    while (1) {
        Segment* segmenty = malloc(V * sizeof(Segment));
        int liczba_segmentow = 0;
        int* odwiedzone = calloc(V, sizeof(int));

        for (int i = 0; i < V; i++) {
            if (narysowane[i] == 0 && odwiedzone[i] == 0) {
                segmenty[liczba_segmentow].wierzcholki = malloc(V * sizeof(int));
                segmenty[liczba_segmentow].punkty_styku = malloc(V * sizeof(int));
                segmenty[liczba_segmentow].pasujace_sciany = malloc((liczba_scian + 1) * sizeof(int));
                segmenty[liczba_segmentow].ile_wierzcholkow = 0;
                segmenty[liczba_segmentow].ile_styku = 0;
                segmenty[liczba_segmentow].ile_pasuje = 0;
                eksploruj_segment(graf, i, narysowane, odwiedzone, &segmenty[liczba_segmentow]);
                liczba_segmentow++;
            }
        }
        
        if (liczba_segmentow == 0) { 
            free(segmenty); free(odwiedzone); 
            break; 
        }

        for (int s = 0; s < liczba_segmentow; s++) {
            for (int i = 0; i < liczba_scian; i++) {
                int pasuje = 1;
                for (int k = 0; k < segmenty[s].ile_styku; k++) {
                    if (!czy_w_scianie(&sciany[i], segmenty[s].punkty_styku[k])) { pasuje = 0; break; }
                }
                if (pasuje) segmenty[s].pasujace_sciany[segmenty[s].ile_pasuje++] = i;
            }
        }

        int min_pasuje = INT_MAX, wybrany_idx = -1;
        for (int i = 0; i < liczba_segmentow; i++) {
            if (segmenty[i].ile_pasuje < min_pasuje) { 
                min_pasuje = segmenty[i].ile_pasuje; 
                wybrany_idx = i; 
            }
        }

        if (min_pasuje == 0) { 
            fprintf(stderr, "BLAD: Graf nie jest planarny!\n"); 
            free(kolor); free(rodzic); free(cykl); free(narysowane);
            for(int i = 0; i < liczba_scian; i++) free(sciany[i].wierzcholki);
            free(sciany); free(segmenty); free(odwiedzone);
            return NULL; 
        }

        Segment* wybrany = &segmenty[wybrany_idx];
        int cel_sciana = wybrany->pasujace_sciany[0]; 

        int* sciezka = malloc(V * sizeof(int));
        int dl_sciezki = 0;
        int* odwiedzone_sciezka = calloc(V, sizeof(int));
        
        int start_styku = wybrany->punkty_styku[0];
        int cel_styku = (wybrany->ile_styku > 1) ? wybrany->punkty_styku[1] : wybrany->punkty_styku[0]; 
        
        if (dfs_sciezka(graf, start_styku, cel_styku, narysowane, odwiedzone_sciezka, sciezka, &dl_sciezki, wybrany)) {
            for(int i=1; i < dl_sciezki-1; i++) narysowane[sciezka[i]] = 1;
            krok_d_rozdziel_sciane(sciany, &liczba_scian, cel_sciana, sciezka, dl_sciezki);
        } else {
            fprintf(stderr, "BLAD: Algorytm utknal, brak trasy przez segment!\n");
            free(sciezka); free(odwiedzone_sciezka);
            break; 
        }

        for(int i=0; i<liczba_segmentow; i++) {
            free(segmenty[i].wierzcholki); 
            free(segmenty[i].punkty_styku); 
            free(segmenty[i].pasujace_sciany);
        }
        free(segmenty); free(odwiedzone); free(sciezka); free(odwiedzone_sciezka);
    }

    struktura_scian* wynik = malloc(sizeof(struktura_scian));
    wynik->rozmiar = liczba_scian;
    wynik->len = malloc(liczba_scian * sizeof(int));
    wynik->sciany = malloc(liczba_scian * sizeof(int*));

    int max_len = 0;
    int index_zewn = 0;

    for (int i = 0; i < liczba_scian; i++) {
        wynik->len[i] = sciany[i].rozmiar;
        wynik->sciany[i] = malloc(sciany[i].rozmiar * sizeof(int));
        
        for (int j = 0; j < sciany[i].rozmiar; j++) {
            wynik->sciany[i][j] = sciany[i].wierzcholki[j] + 1; // Powrót do numeracji 1..V
        }

        if (wynik->len[i] > max_len) {
            max_len = wynik->len[i];
            index_zewn = i;
        }
    }
    
    wynik->s_zewn = index_zewn;

    free(kolor); free(rodzic); free(cykl); free(narysowane);
    for(int i = 0; i < liczba_scian; i++) free(sciany[i].wierzcholki);
    free(sciany);

    return wynik;
}