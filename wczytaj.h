
#ifndef _wczytaj_h_
#define _wczytaj_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

// Wczytuje dane do tej smiesznej macierzy / listy sasiedztw
// Ja to zrobie (Adam)
int add_k(lista_sasiedztw* m, int from, int to, char* nazwa, double waga);

lista_sasiedztw* w_dane(char* f_name);

void free_m(lista_sasiedztw *m);

#endif