#include "cykl.h"



cycle *dfs_find_cycle(lista_sasiedztw *list,int tmp_id,int *color,int last_id){ //0-white 1-grey 2-black

    //it returns IDs of vertex in lista sasiedztw (not names of the vertex)
    if(list->lista==NULL){
        return NULL;
    }

    
    lista_k *edge=list->lista[tmp_id];

    color[tmp_id]=1;
    while(edge!=NULL){
        int neighbor_id=find_id_in_list(list,edge->nr_wierzcholka_cel);
        if(neighbor_id==-1){
            printf("%d, %d",edge->nr_wierzcholka_cel,neighbor_id);
            edge=edge->next;
            continue;
        }
        if(neighbor_id!=last_id  && color[neighbor_id]==1){
            cycle *found=calloc(1,sizeof(cycle));
            found->rozmiar=1;
            found->wierzcholki=calloc(1,sizeof(int));
            found->wierzcholki[0]=tmp_id;
            found->poczatek=neighbor_id;
            found->is_looped=0;
            return found;
        }
        else if(color[neighbor_id]==0){
            cycle *test=dfs_find_cycle(list,neighbor_id,color,tmp_id);
            if(test!=NULL){
                if(!test->is_looped){
                    test->rozmiar++;
                    test->wierzcholki=realloc(test->wierzcholki,test->rozmiar *sizeof(int));
                    test->wierzcholki[test->rozmiar-1]=tmp_id;
                    if(tmp_id==test->poczatek){
                        test->is_looped=1;
                    }
                }
                
                return test;
            }
        }
        edge=edge->next;
    }
    color[tmp_id]=2;
    return NULL;
}

cycle *find_cycle(lista_sasiedztw *list,int start_id){
    
    if(list==NULL ||list->lista==NULL){
        return NULL;
    }

    int *color=calloc(list->rozmiar,sizeof(int));
    if(color==NULL){
        return NULL;
    }

    cycle *result=dfs_find_cycle(list,start_id,color,-1);

    free(color);
    return result;
}

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
        //printf("%d: %d, %d\n",j,pretender->wierzcholki[j],find_id_in_list(list,pretender->wierzcholki[j]));
    }
    while(start_id==-1){
        if(!is_in_cycle[i]){
            start_id=i;
            break;
        }
        i++;
    }
    //for(int x=0;x<list->rozmiar;x++){
      //  printf("%d = %d\n",x,is_in_cycle[x]);
    //}
    //printf("%d\n",start_id);
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





/*int is_in_cycle(cycle *test,int id){
    for(int i=0;i<test->rozmiar;i++){
        if(test->wierzcholki[i]==id){
            return 1;
        }
    }
    return 0;
}*/


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