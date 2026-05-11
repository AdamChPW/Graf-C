#include "struct.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct c{
    int* wierzcholki; 
    int rozmiar;
    int poczatek;
    int is_looped;
} cycle;

void print_cycle_ids(cycle *to_print);
void print_cycle_names(cycle *to_print,lista_sasiedztw *list);
int find_id_in_list(lista_sasiedztw *list,int name);
/*int is_in_cycle(cycle *test,int id);*/
int is_connected_without_cycle(lista_sasiedztw *list,cycle *pretender);
void dfs_without_cycle(lista_sasiedztw *list,cycle *pretender,int *color,int *is_in_cycle, int tmp_id);
cycle *split_if_shortcut(lista_sasiedztw *list, cycle *pretender);