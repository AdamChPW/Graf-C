#include <stdio.h>
#include <stdlib.h>
#include "wczytaj.h"
#include "fpp.h"

//algorytm demoucrona

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

//funkcje

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

//
void dfs_szukaj_cyklu(lista_sasiedztw* graf, int u, int p, int* color, int* parent) {
    color[u] = 1;
    parent[u] = p;
    lista_k* sasiad = graf->lista[u];
    
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka;
        if (v == p) { sasiad = sasiad->next; continue; }
        if (color[v] == 1) {
            if (start_cyklu == -1) { start_cyklu = v; koniec_cyklu = u; }
            return;
        } else if (color[v] == 0) {
            dfs_szukaj_cyklu(graf, v, u, color, parent);
            if (start_cyklu != -1) return;
        }
        sasiad = sasiad->next;
    }
    color[u] = 2;
}

void eksploruj_segment(lista_sasiedztw* graf, int u, int* in_drawing, int* visited, Segment* seg) {
    visited[u] = 1;
    seg->wierzcholki[seg->ile_wierzcholkow++] = u;
    lista_k* sasiad = graf->lista[u];
    
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka;
        if (in_drawing[v] == 1) {
            if (!czy_w_tablicy(seg->punkty_styku, seg->ile_styku, v)) {
                seg->punkty_styku[seg->ile_styku++] = v;
            }
        } else if (in_drawing[v] == 0 && visited[v] == 0) {
            eksploruj_segment(graf, v, in_drawing, visited, seg);
        }
        sasiad = sasiad->next;
    }
}

// ==========================================
// 5. WYCIĄGANIE ŚCIEŻKI Z SEGMENTU
// ==========================================
int dfs_sciezka(lista_sasiedztw* graf, int u, int cel_v, int* in_drawing, int* visited, int* sciezka, int* dlugosc) {
    visited[u] = 1;
    sciezka[(*dlugosc)++] = u;
    
    if (u == cel_v) return 1;

    lista_k* sasiad = graf->lista[u];
    while (sasiad != NULL) {
        int v = sasiad->nr_wierzcholka;
        if (!visited[v] && (in_drawing[v] == 0 || v == cel_v)) {
            if (dfs_sciezka(graf, v, cel_v, in_drawing, visited, sciezka, dlugosc)) return 1;
        }
        sasiad = sasiad->next;
    }
    
    (*dlugosc)--; // Backtracking
    return 0;
}

// ==========================================
// 6. ROZDZIELANIE ŚCIANY
// ==========================================
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

// ==========================================
// 7. GŁÓWNY ALGORYTM DEMOUCRONA
// ==========================================
struktura_scian* demoucron(lista_sasiedztw* graf) {
    int V = graf->rozmiar;
    int* in_drawing = calloc(V, sizeof(int));
    
    int* color = calloc(V, sizeof(int));
    int* parent = malloc(V * sizeof(int));
    for(int i=0; i<V; i++) parent[i] = -1;

    start_cyklu = -1;
    koniec_cyklu = -1;
    
    // Szukanie pierwszego cyklu
    for (int i = 0; i < V; i++) {
        if (color[i] == 0 && start_cyklu == -1) dfs_szukaj_cyklu(graf, i, -1, color, parent);
    }

    if (start_cyklu == -1) { 
        printf("BLAD: Brak cykli w grafie (drzewo/las).\n"); 
        free(color); free(parent); free(in_drawing);
        return NULL; 
    }

    int* cykl = malloc(V * sizeof(int));
    int dl_cyklu = 0;
    int curr = koniec_cyklu;
    while (curr != start_cyklu) { cykl[dl_cyklu++] = curr; curr = parent[curr]; }
    cykl[dl_cyklu++] = start_cyklu;

    for (int i = 0; i < dl_cyklu; i++) in_drawing[cykl[i]] = 1;

    Sciana* sciany = malloc(200 * sizeof(Sciana)); // 200 dla bezpieczeństwa z Eulerem
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
        int* visited = calloc(V, sizeof(int));

        for (int i = 0; i < V; i++) {
            if (in_drawing[i] == 0 && visited[i] == 0) {
                segmenty[liczba_segmentow].wierzcholki = malloc(V * sizeof(int));
                segmenty[liczba_segmentow].punkty_styku = malloc(V * sizeof(int));
                segmenty[liczba_segmentow].pasujace_sciany = malloc(liczba_scian * sizeof(int));
                segmenty[liczba_segmentow].ile_wierzcholkow = 0;
                segmenty[liczba_segmentow].ile_styku = 0;
                segmenty[liczba_segmentow].ile_pasuje = 0;
                eksploruj_segment(graf, i, in_drawing, visited, &segmenty[liczba_segmentow]);
                liczba_segmentow++;
            }
        }
        
        if (liczba_segmentow == 0) { 
            free(segmenty); 
            free(visited); 
            break; // Cały graf narysowany
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

        int min_pasuje = 999999, wybrany_idx = -1;
        for (int i = 0; i < liczba_segmentow; i++) {
            if (segmenty[i].ile_pasuje < min_pasuje) { 
                min_pasuje = segmenty[i].ile_pasuje; 
                wybrany_idx = i; 
            }
        }

        if (min_pasuje == 0) { 
            printf("BLAD: Graf nie jest planarny!\n"); 
            free(color); free(parent); free(cykl); free(in_drawing);
            for(int i = 0; i < liczba_scian; i++) free(sciany[i].wierzcholki);
            free(sciany);
            return NULL; 
        }

        Segment* wybrany = &segmenty[wybrany_idx];
        int cel_sciana = wybrany->pasujace_sciany[0]; 

        int* sciezka = malloc(V * sizeof(int));
        int dl_sciezki = 0;
        int* visited_sciezka = calloc(V, sizeof(int));
        
        int start_styku = wybrany->punkty_styku[0];
        int cel_styku = wybrany->punkty_styku[1]; 
        
        dfs_sciezka(graf, start_styku, cel_styku, in_drawing, visited_sciezka, sciezka, &dl_sciezki);

        for(int i=1; i < dl_sciezki-1; i++) in_drawing[sciezka[i]] = 1;

        krok_d_rozdziel_sciane(sciany, &liczba_scian, cel_sciana, sciezka, dl_sciezki);

        for(int i=0; i<liczba_segmentow; i++) {
            free(segmenty[i].wierzcholki); 
            free(segmenty[i].punkty_styku); 
            free(segmenty[i].pasujace_sciany);
        }
        free(segmenty); free(visited); free(sciezka); free(visited_sciezka);
    }

    // ==========================================
    // 8. PAKOWANIE DO DOCELOWEJ STRUKTURY
    // ==========================================
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
            wynik->sciany[i][j] = sciany[i].wierzcholki[j];
        }

        if (wynik->len[i] > max_len) {
            max_len = wynik->len[i];
            index_zewn = i;
        }
    }
    
    wynik->s_zewn = index_zewn;

    // Sprzątanie po strukturach wewnętrznych
    free(color); 
    free(parent); 
    free(cykl); 
    free(in_drawing);
    for(int i = 0; i < liczba_scian; i++) free(sciany[i].wierzcholki);
    free(sciany);

    return wynik;
}