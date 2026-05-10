#include "wczytaj.h"
#include "tutte.h"
#include "struct.h"
#include "utils.h"



// Main. Plis zostawcie maina jak najbardziej czytelnym
int main(int argc, char** argv)
{
    if(argc < 2){
        fprintf(stderr, "Nie podano wejscie.\n");
        return 1;
    }
    
    char *ext = strrchr(argv[1], '.');
    if (ext == NULL || (strcmp(ext, ".txt") != 0 && strcmp(ext, ".csv") != 0)) {
        fprintf(stderr, "Nieobslugiwany format pliku.\n");
        return 1;
    }

    lista_sasiedztw* macierz = w_dane(argv[1]);
    if( macierz == NULL ){
        fprintf(stderr, "Nie udalo sie utworzyc macierzy.\n");
        return 2;
    }
    //test(macierz);

    if( spr_macierz(macierz) != 0 ){
        fprintf(stderr, "Macierz nie jest odpowiednia do algorytmu.\n");
        free_m(macierz);
        return 3;
    }

    Lista_W* lv = algo(macierz);

    if(lv == NULL)
    {
        fprintf(stderr, "Konczenie dzialanie programu\n");
        free_m(macierz);
        return 4;
    }

    wypisz_dla_desmosa(lv);

    FILE *out = argc > 2 ? fopen(argv[2], "w") : fopen("wyjscie", "w");

    if( wyp_dane( out, lv ) == 0 )
        fprintf(stdout, "Wypisano odpowiedz do pliku.\n");
    else {
        fprintf(stderr, "Nie udalo sie zapisac odpowidzi\n");
        free_m(macierz);
        free_lv(lv);
        fclose(out);
        return 5;
    }

    free_m(macierz);
    free_lv(lv);
    fclose(out);
    return 0;
}