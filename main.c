#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"

typedef struct person_s{
    int starvation;
    int posiAtual;
}person_t;

/* Contantes*/
#define ACTION_OFF_SET (0)
#define ACTION_STATUS (ACTION_OFF_SET + 1)
#define ACTION_SET_STATUS (ACTION_OFF_SET + 2)
#define ACTION_PRINT_MAP (ACTION_OFF_SET + 3)
#define ACTION_MAP_RECREATE (ACTION_OFF_SET + 4)
#define ACTION_LIST (ACTION_OFF_SET + 5)  
#define ACTION_EDIT_LIST (ACTION_OFF_SET + 6) 
#define ACTION_PRINT_LIST (ACTION_OFF_SET + 7)




int main(void){
    /* Variáveis */
    person_t fulaninho;
    grafo * gr = NULL;
    lista * restaurantes = NULL;
    retornoBusca_t * menorCaminho = NULL;

    int fome = 0;
    int escolha = 1;
    int minV, maxV;
    tipoGrafo tp;
    int posiAtual;

    lista * aux;
    lista * path;

    /* Criando um grafo base */
    criaGrafo(&gr, 8, grafoNaoDirecionado);
    verticeAddAresta(&gr, 0, 1, 3);
    verticeAddAresta(&gr, 0, 2, 2);
    verticeAddAresta(&gr, 0, 3, 1);
    verticeAddAresta(&gr, 1, 4, 2);
    verticeAddAresta(&gr, 4, 5, 3);
    verticeAddAresta(&gr, 5, 6, 1);
    verticeAddAresta(&gr, 6, 7, 1);
    verticeAddAresta(&gr, 2, 5, 1);
    verticeAddAresta(&gr, 3, 6, 6);

    /* Criando uma lista de restaurantes base */
    listaAdd(&restaurantes, &gr->vertices[7]);
    listaAdd(&restaurantes, &gr->vertices[4]);

    /* Criando o Moribundo*/
    fulaninho.starvation = 0;
    fulaninho.posiAtual = 0;

    do{
        (void)printf("\n\nProblema: \"Don't Starve\"\n");
        (void)printf("Estou cagado de fome\n");
        (void)printf("Escolha uma das acoes que deseja realizar:\n");
        (void)printf("[%2d] - Vizualizar a situacao atual\n", ACTION_STATUS);
        (void)printf("[%2d] - Atualizar situacao atual\n", ACTION_SET_STATUS);
        (void)printf("[%2d] - Imprimir o mapa\n", ACTION_PRINT_MAP);
        (void)printf("[%2d] - Gerar novo mapa\n", ACTION_MAP_RECREATE);
        (void)printf("[%2d] - Imprimir lista de restaurantes\n", ACTION_LIST);
        (void)printf("[%2d] - Atualizar lista de restaurantes\n", ACTION_EDIT_LIST);
        (void)printf("[%2d] - Imprimir o caminho para os restaurantes \n", ACTION_PRINT_LIST);
        (void)printf("[0] - Fechar o programa\n");

        (void)scanf("%d%*c", &escolha);

        switch(escolha){
            case ACTION_STATUS:
                (void)printf("Situacao do Moribundo:\n");
                (void)printf("Nivel de fome: %3d\n", fulaninho.starvation);
                (void)printf("Se encontra na posicao: %d\n", fulaninho.posiAtual);
                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_SET_STATUS:
                (void)printf("Insira o nivel de fome atual: ");
                (void)scanf("%d%*c", &fome);
                if(fome > 100 || fome < 0){
                    (void)printf("Entrada invalida.\n");
                }else{
                    fulaninho.starvation = fome;
                }

                (void)printf("Insira a posicao atual: ");
                (void)scanf("%d%*c", &posiAtual);
                if(posiAtual >= gr->numVertices || posiAtual < 0){
                    (void)printf("Entrada invalida.\n");
                }else{
                    fulaninho.posiAtual = posiAtual;
                }

                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_PRINT_MAP:
                if(gr != NULL){
                    grafoImprime(&gr);
                }else{
                    (void)printf("Grafo nao foi criado\n");
                }
                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_MAP_RECREATE:
                if(gr != NULL){
                    (void)printf("Ja existe um mapa.\nTem certeza que deseja criar outro?\n");
                    (void)printf("[1] - Sim\n[2] - Nao\n");
                    (void)scanf("%d%*c", &escolha);
                    if(escolha == 1){
                        liberaGrafo(&gr);
                    }else{
                        break;
                    }
                }
                (void)printf("Insira o tamanho min: ");
                (void)scanf("%d%*c", &minV);
                (void)printf("Insira o tamanho max: ");
                (void)scanf("%d%*c", &maxV);
                (void)printf("Insira o tipo das ruas:\n");
                (void)printf("[1] - Direcionando\n[2] - Nao direcionado\n");
                (void)scanf("%d%*c", &escolha);

                if(escolha == 1){
                    tp = grafoDirecionado;
                }else{
                    tp = grafoNaoDirecionado;
                }

                grafoCriaRandom(&gr, minV, maxV, tp);

                listaLibera(&restaurantes);
                fulaninho.posiAtual = 0;

                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_LIST:
                if(restaurantes != NULL){
                    (void)printf("Restaurantes na regiao:\n\n");
                    aux = restaurantes;
                    while(aux != NULL){
                        printf("Restaurante: %-4d\n", aux->info->info);
                        aux = aux->prox;
                    }
                }else{
                    (void)printf("Nao tem restaurantes na regiao\n");
                }


                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_EDIT_LIST:
                if(restaurantes != NULL){
                    (void)printf("Qual acao deseja executar?\n");
                    (void)printf("[ 1] - Remover um restaurante da lista\n");
                    (void)printf("[ 2] - Adicionar um restaurante a lista\n");
                    (void)scanf("%d%*c", &escolha);
                    if(escolha == 1){
                        (void)printf("Insira qual restaurante deseja remover: ");
                        (void)scanf("%d%*c", &escolha);
                        listaRemove(&restaurantes, escolha);
                    }else if(escolha == 2){
                        (void)printf("Insira qual restaurante deseja adicionar: ");
                        (void)scanf("%d%*c", &escolha);
                        if(escolha >= gr->numVertices || escolha < 0){
                            (void)printf("Restaurante nao foi encontrado no mapa\n");
                        }else{
                            listaAdd(&restaurantes, &gr->vertices[escolha]);
                        }
                    }
                }else{
                    (void)printf("Nao tem restaurantes na regiao.\n");
                    (void)printf("Deseja adicionar algum?\n");
                    (void)printf("[1] - Sim\n");
                    (void)printf("[2] - Nao\n");
                    (void)scanf("%d%*c", &escolha);
                    if(escolha == 1){
                        (void)printf("Insira qual restaurante deseja adicionar: ");
                        (void)scanf("%d%*c", &escolha);
                        if(escolha >= gr->numVertices || escolha < 0){
                            (void)printf("Restaurante nao foi encontrado no mapa\n");
                        }else{
                            listaAdd(&restaurantes, &gr->vertices[escolha]);
                        }
                    }
                }

                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            case ACTION_PRINT_LIST:
                if(gr != NULL && restaurantes != NULL){
                    aux = restaurantes;
                    while(aux != NULL){
                        menorCaminho = buscaLargura(&gr, fulaninho.posiAtual, aux->info->info);                        
                        (void)printf("Restaurante: %-4d\n\n", aux->info->info);
                        (void)printf("Fome ate chegar no restaurante: %3d\n", (menorCaminho->custo + fulaninho.starvation));
                        (void)printf("Caminho para chegar no restaurante:\n");
                        path = menorCaminho->path;
                        if(path == NULL){
                            (void)printf("Nao tem caminho ate o restaurante.\n");
                        }else{
                            while(path->prox != NULL){
                                (void)printf("%4c%2d --> %2d\n", ' ',path->info->info, path->prox->info->info);
                                path = path->prox;
                            }
                            (void)printf("%4cEnfim: %d\n\n\n", ' ', path->info->info);
                        }
                        if(menorCaminho != NULL){
                            listaLibera(&menorCaminho->path);
                            free(menorCaminho);
                        }

                        aux = aux->prox;
                    }
                }


                (void)printf("\nAperte enter para continuar...\n");
                (void)scanf("%*c");
                (void)printf("\n\n\n\n");
                break;

            default:

                break;
        }


    }while(escolha != 0);


    if(gr != NULL){
        liberaGrafo(&gr);
    }
    if(menorCaminho != NULL){
        listaLibera(&menorCaminho->path);
        free(menorCaminho);
    }

    if(restaurantes != NULL){
        listaLibera(&restaurantes);
    }

    return EXIT_SUCCESS;
}
