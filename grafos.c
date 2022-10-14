/* Inclusões */
#include "grafos.h"
#include "filas.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Funções de Lista */
void listaAdd(lista ** l, vertice * info){
    lista * new = (lista*)malloc(sizeof(lista));
    lista * aux;
    
    new->info = info;
    new->prox = NULL;
    

    if((*l) != NULL){
        aux = (*l);
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = new;
    }else{
        (*l) = new;
    }
}

void listaLibera(lista ** l){
    lista * aux = (*l);

    while(aux != NULL){
        aux = (*l)->prox;
        free(*l);
        (*l) = aux;
    }
}

void listaRemove(lista **l, int remove){
    lista * aux;
    lista * ant = NULL;
    if((*l) != NULL){
        aux = (*l);
        while(aux != NULL){
            if(aux->info->info == remove){
                break;
            }else{
                ant = aux;
                aux = aux->prox;
            }
        }

        if(ant == NULL){
            (*l) = (*l)->prox;
        }else{
            ant->prox = aux->prox;
        }
        free(aux);
    }
}


/* Funções de Grafo */
//Função para criação de grafo
void criaGrafo(grafo** graf, int numVertices, tipoGrafo type){
    *graf = (grafo*)malloc(sizeof(grafo));
    (*graf)->numVertices = numVertices;
    (*graf)->type = type;
    
    //Criando os vertices e setando os valores padrões
    (*graf)->vertices = (vertice*)malloc(sizeof(vertice) * numVertices);
    for(int i = 0; i < (*graf)->numVertices; i++){
        (*graf)->vertices[i].info = i;
        (*graf)->vertices[i].arestas = NULL;
        (*graf)->vertices[i].stats = white;
    }
}

//Função auxiliar para saber se uma aresta existe ou nao
static int arestaExiste(aresta * actual, int origin, int destiny){
    if(actual != NULL){
        if(actual->origin == origin && actual->destiny == destiny){
            return 1;
        }else{
            arestaExiste(actual->prox, origin, destiny);
        }
    }
    return 0;
}

// Função para criar arestas
static aresta* arestaCria(aresta* actual, int peso, int origin, int destiny){
    aresta * new = (aresta*)malloc(sizeof(aresta));
    new->origin = origin;
    new->destiny = destiny;
    new->peso = peso;
    new->prox = actual;
    return new;
}

// Função auxiliar para liberar arestas
static void arestaLibera(aresta* start){
    if(start != NULL){
        arestaLibera(start->prox);
        free(start);
    }
}

void liberaGrafo(grafo** grafo){
    for(int i = 0; i < (*grafo)->numVertices; i++){
        arestaLibera((*grafo)->vertices[i].arestas);
    }
    free((*grafo)->vertices);
}


// Função para adicionar arestas entre dois vertices
void verticeAddAresta(grafo** grafo, int origin, int destiny, int peso){
    if(origin >= (*grafo)->numVertices){
        return;
    }else if(destiny >= (*grafo)->numVertices){
        return;
    }
    if(!arestaExiste((*grafo)->vertices[origin].arestas, origin, destiny)){
        (*grafo)->vertices[origin].arestas = arestaCria((*grafo)->vertices[origin].arestas, peso, origin, destiny);
    }
    if((*grafo)->type == grafoNaoDirecionado){
        if(!arestaExiste((*grafo)->vertices[destiny].arestas, destiny, origin)){
            (*grafo)->vertices[destiny].arestas = arestaCria((*grafo)->vertices[destiny].arestas, peso, destiny, origin);
        }
    }

}

void grafoImprime(grafo ** grafo){
    aresta* aux;

    // Informações do grafo
    (void)printf("Grafo\n");
    (void)printf("Tipo: ");
    (*grafo)->type == grafoDirecionado ? (void)printf("Direcionado\n") : (void)printf("Nao direcionado\n");
    (void)printf("Numero de vertices: %d\n", (*grafo)->numVertices);
    //Imprimindo cada vertice
    for(int i = 0; i < (*grafo)->numVertices; i++){
        (void)printf("Vertice #%02d\n", (*grafo)->vertices[i].info);
        if((*grafo)->vertices[i].arestas != NULL){
            aux = (*grafo)->vertices[i].arestas;
            while(aux!= NULL){
                (void)printf("%2d --> %-2d (%3d)\n", aux->origin, aux->destiny, aux->peso);
                aux = aux->prox;
            }
        }else{
            (void)printf("Sem arestas\n");
        }
    }
}


// Função para criar grafos aleatórios
void grafoCriaRandom(grafo **grafo, int minV, int maxV, tipoGrafo tipo){
    srand(time(NULL));
    int numVertices = (minV + (rand() % (maxV - minV)));
    int type = tipo;
    int random;

    criaGrafo(grafo, numVertices, type);
    
    for(int i = 0; i < numVertices; i++){
        random = (rand() % numVertices) + 1;
        for(int j = 0; j < random; j++){
            verticeAddAresta(grafo, i, rand() % numVertices, rand() % 100);
        }
    }
}

// Função para copiar o caminho ate um vertice para outro vertice
static void pathCopy(lista **pathList, int original, int copy){
    lista * aux;
    while(pathList[copy] != NULL){
        aux = pathList[copy]->prox;
        free(pathList[copy]);
        pathList[copy] = aux;
    }

    if(pathList[original] != NULL){
        aux = pathList[original];
        while(aux != NULL){
            listaAdd(&pathList[copy], aux->info);
            aux = aux->prox;
        }
    }
}

static void grafoAllWhite(grafo ** grafo){
    for(int i = 0; i < (*grafo)->numVertices; i++){
        (*grafo)->vertices[i].stats = white;
    }
}

//Função que implementa busca em largura para descobrir o menor caminho entre dois pontos e retorna esse caminho
retornoBusca_t * buscaLargura(grafo **grafo, int vOrigin, int vDestiny){
    //Fila q define a ordem de procura
    fila *queue = filaCria();
    
    //Vetores de controle
    int *distance; //Distância ate a origem
    lista **path;
    retornoBusca_t * retorno;

    //Variáveis auxiliares
    aresta * arestaAtual;
    vertice * posiActual;

    if(grafo != NULL){ // Verificando se o grafo é valido
        if(vOrigin > (*grafo)->numVertices || vDestiny > (*grafo)->numVertices){
            return NULL;
        }else if(vOrigin < 0 || vDestiny < 0){
            return NULL;
        }else{
            //Alocando um vetor que vai salvar as distâncias
            distance = (int*)malloc(sizeof(int) * (*grafo)->numVertices);
            for(int i = 0; i < (*grafo)->numVertices; i++){
                distance[i] = ARESTA_FORA_GRAFO;
            }
            //Inicializando a distâncio do vOrigem com ele msm
            distance[vOrigin] = 0;

            //Alocando espaço para mapear os caminhos existentes no grafo
            path = (lista**)malloc(sizeof(lista*) * (*grafo)->numVertices);
            for(int i = 0; i < (*grafo)->numVertices; i++){
                path[i] = NULL;
            }
            //Caminho do vetor origem ate ele msm é somente ele msm
            listaAdd(&path[vOrigin], &(*grafo)->vertices[vOrigin]);

            //Iniciando a busca em largura
            filaAdd(&queue, &((*grafo)->vertices[vOrigin]));
            (*grafo)->vertices[vOrigin].stats = gray;

            
            //Enquanto tiver vertices na fila
            while(queue != NULL){
                //Atualiza em qual vertice esta o algoritmo
                posiActual = filaRemove(&queue);

                //Atualiza a lista de arestas para procurar
                arestaAtual = posiActual->arestas;

                //Enquanto ainda tiver arestas para procurar
                while(arestaAtual != NULL){
                    //Se o vertice ainda n foi visitado
                    if((*grafo)->vertices[arestaAtual->destiny].stats == white){
                        (*grafo)->vertices[arestaAtual->destiny].stats = gray; //Define o vertice como cinza
                        filaAdd(&queue, &((*grafo)->vertices[arestaAtual->destiny])); //Add o vertice na fila
                        distance[arestaAtual->destiny] = distance[arestaAtual->origin] + arestaAtual->peso;
                        //Copio o caminho até o vertice anterior
                        pathCopy(path, arestaAtual->origin, arestaAtual->destiny);
                        //Salvo o vertice q esta sendo visitado agr no caminho
                        listaAdd(&path[arestaAtual->destiny], &(*grafo)->vertices[arestaAtual->destiny]);
                    }else if(distance[arestaAtual->destiny] > (distance[arestaAtual->origin] + arestaAtual->peso)){ //Se foi achado um novo caminho menor q o anterior
                        distance[arestaAtual->destiny] = (distance[arestaAtual->origin] + arestaAtual->peso); //Atualiza a distancia
                        if((*grafo)->vertices[arestaAtual->destiny].stats == black){
                            (*grafo)->vertices[arestaAtual->destiny].stats = gray;
                        }
                        filaAdd(&queue, &(*grafo)->vertices[arestaAtual->destiny]);
                        pathCopy(path, arestaAtual->origin, arestaAtual->destiny);
                        listaAdd(&path[arestaAtual->destiny], &(*grafo)->vertices[arestaAtual->destiny]);
                    }
                    //Avançando pro proximo vertice adjacente ao vertice atual
                    arestaAtual = arestaAtual->prox;
                }
                posiActual->stats = black;
            }
            //Salvando o menor caminho
            retorno = (retornoBusca_t*)malloc(sizeof(retornoBusca_t));
            retorno->path = path[vDestiny];
            retorno->custo = distance[vDestiny];


            /* Liberando a memória */
            filaLibera(&queue);
            free(distance);
            for(int i = 0; i < (*grafo)->numVertices; i++){
                if(i != vDestiny){
                    listaLibera(&path[i]);
                }
            }
            grafoAllWhite(grafo);
            //Com isso feito eu tenho as menor distancia possivel entre o meu vOrigem e o vDestino
            return retorno;
        }
    }
    return NULL;
}
