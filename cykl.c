#include "cykl.h"

int find_id_in_list(lista_sasiedztw *list,int name){
    for(int i=0;i<list->rozmiar;i++){
        if(list->lista[i]->nr_wierzcholka_start==name){
            return i;
        }
    }
    return -1;
}

void dfs_without_cycle(lista_sasiedztw *list,cycle *pretender,int *color,int *is_in_cycle, int tmp_id){
    if(list->lista==NULL){
        return;
    }
    
    lista_k *edge=list->lista[tmp_id];
    color[tmp_id]=1;

    while(edge!=NULL){
        int next_id=find_id_in_list(list,edge->nr_wierzcholka_cel);
        if (next_id == -1) { 
            edge = edge->next; continue;
        }
        if(color[next_id]==0  && !is_in_cycle[next_id]){
            dfs_without_cycle(list,pretender,color,is_in_cycle,next_id);
        }
        edge=edge->next;
    }
    color[tmp_id]=2;
}
int is_connected_without_cycle(lista_sasiedztw *list,cycle *pretender){
    if(pretender->rozmiar==list->rozmiar){
        return 1;
    }
    if(pretender->rozmiar>list->rozmiar){
        return -1;
    }
    int *color=calloc(list->rozmiar,sizeof(int));


    int start_id=-1;
    int i=0;
    int *is_in_cycle=calloc(list->rozmiar,sizeof(int));
    for(int j=0;j<pretender->rozmiar;j++){
        is_in_cycle[pretender->wierzcholki[j]]=1;
    }
    while(start_id==-1){
        if(!is_in_cycle[i]){
            start_id=i;
            break;
        }
        i++;
    }

    if(start_id!=-1){
        dfs_without_cycle(list,pretender,color,is_in_cycle,start_id);
    }
    
    int tmp=0;
    for(int j=0;j<list->rozmiar;j++){
        tmp+=color[j];
    }
    if(tmp==(list->rozmiar-pretender->rozmiar)*2)
    {
        free(color);
        free(is_in_cycle);
        return 1;
    }
    free(color);
    free(is_in_cycle);
    return 0;
}

cycle *split_if_shortcut(lista_sasiedztw *list, cycle *pretender){
    int *is_in_cycle=calloc(list->rozmiar,sizeof(int));
    for(int j=0;j<pretender->rozmiar;j++){
        is_in_cycle[pretender->wierzcholki[j]]=1;
    }

    for(int i=0;i<pretender->rozmiar;i++){
        lista_k *edge=list->lista[pretender->wierzcholki[i]];
        while(edge!=NULL){
            int tmp_id_in_list=find_id_in_list(list,edge->nr_wierzcholka_cel);
            if(is_in_cycle[tmp_id_in_list]){
                int id_in_cycle=-1;
                int n=0;
                while(id_in_cycle==-1){
                    if(pretender->wierzcholki[n]==tmp_id_in_list){
                        id_in_cycle=n;
                        break;
                    }
                    n++;
                }
                int distance=abs(i-id_in_cycle);
                if(distance>1 && distance<pretender->rozmiar-1 && i<id_in_cycle){

                    cycle *right=calloc(1,sizeof(cycle));
                    right->rozmiar=id_in_cycle-i+1;
                    right->wierzcholki=calloc(right->rozmiar,sizeof(int));

                    cycle *left=calloc(1,sizeof(cycle));
                    left->rozmiar=pretender->rozmiar-right->rozmiar+2;
                    left->wierzcholki=calloc(left->rozmiar,sizeof(int));

                    for(int j=0;j<pretender->rozmiar;j++){
                        if(j<i){
                            left->wierzcholki[j]=pretender->wierzcholki[j];
                        }
                        else if(j==i){
                            left->wierzcholki[j]=pretender->wierzcholki[j];
                            right->wierzcholki[j-i]=pretender->wierzcholki[j];
                        }
                        else if(j>i && j<id_in_cycle){
                            right->wierzcholki[j-i]=pretender->wierzcholki[j];
                        }
                        else if(j==id_in_cycle){
                            right->wierzcholki[j-i]=pretender->wierzcholki[j];
                            left->wierzcholki[j-right->rozmiar+2]=pretender->wierzcholki[j];
                        }
                        else{
                            left->wierzcholki[j-right->rozmiar+2]=pretender->wierzcholki[j];
                        }
                    }
                    
                    
                    if(is_connected_without_cycle(list,left)){
                        free(right->wierzcholki);
                        free(right);
                        free(is_in_cycle);
                        free(pretender->wierzcholki);
                        free(pretender);
                        return left;
                    }
                    else if(is_connected_without_cycle(list,right)){
                        free(left->wierzcholki);
                        free(left);
                        free(is_in_cycle);
                        free(pretender->wierzcholki);
                        free(pretender);
                        return right;
                    }
                    else{
                        free(left->wierzcholki); 
                        free(left);
                        free(right->wierzcholki); 
                        free(right);
                        free(is_in_cycle);
                        return pretender;
                    }
                }



            }
            edge=edge->next;
        }
    }
    free(is_in_cycle);
    return pretender;
}


void print_cycle_ids(cycle *to_print){
    for(int i = 0;i<to_print->rozmiar;i++){
        printf("%d ",to_print->wierzcholki[i]);
    }
    printf("\n");
    return;
}
void print_cycle_names(cycle *to_print,lista_sasiedztw *list){
    for(int i = 0;i<to_print->rozmiar;i++){
        printf("%d ",list->lista[to_print->wierzcholki[i]]->nr_wierzcholka_start);
    }
    printf("\n");
    return;
}