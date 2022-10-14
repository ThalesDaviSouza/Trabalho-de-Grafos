#include "grafos.h"
#include "filas.h"
#include <stdio.h>
#include <stdlib.h>

fila* filaCria(){
    fila* f = NULL;
    return f;
}

void filaAdd(fila** actual, vertice* joined){
    fila* new = (fila*)malloc(sizeof(fila));
    fila * aux;
    if((*actual) != NULL){
        aux = (*actual);
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = new;
        new->prim = (*actual)->prim;
    }else{
        new->info = joined;
        new->prim = new->info;
        (*actual) = new;
    }
    new->prox = NULL;
    new->info = joined;
}

vertice* filaRemove(fila** f){
    fila *libera = NULL;
    fila *temp = NULL;
    vertice *retorno = NULL;
    fila *prim = NULL;

    if((*f) != NULL){
        libera = (*f);
        retorno = (*f)->info;
        temp = (*f)->prox;
        prim = (*f)->prox;
        while(temp != NULL){
            temp->prim = prim->info;
            temp = temp->prox;
        }
        (*f) = (*f)->prox;
        free(libera);
    } 
    return retorno;
}

void filaLibera(fila** f){
    fila * aux = (*f);
    fila * temp;
    if(f != NULL){
        while(aux != NULL){
            temp = aux;
            aux = aux->prox;
            free(temp);
        }
        free((*f));
    }
}