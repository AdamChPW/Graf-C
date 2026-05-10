#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Potrzebne do strcmp

//#include "wczytaj.h"
#include "tutte.h"
#include "struct.h"

void test(lista_sasiedztw* m);
int spr_macierz(lista_sasiedztw* l);
int wyp_dane( FILE *f, Lista_W* lv );
void wypisz_dla_desmosa(Lista_W* lv);