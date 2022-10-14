#define ARESTA_FORA_GRAFO (-2)
#define ERRO_BUSCA_LARGURA (-1)
#define ERRO_BUSCA_PROFUNDIDADE (-10)

enum tipoGrafo_e{
    grafoDirecionado,
    grafoNaoDirecionado,
};

enum status_e{
    white,
    gray,
    black,
};

typedef enum tipoGrafo_e tipoGrafo;
typedef enum status_e status;

typedef struct aresta_t{
    int peso;
    int origin;
    int destiny;
    struct aresta_t *prox;
}aresta;

typedef struct vertice_s{
    int info;
    status stats;
    aresta *arestas;
}vertice;

typedef struct grafo_s{
    int numVertices;
    vertice * vertices;
    tipoGrafo type;
}grafo;

typedef struct lista_s{
    vertice * info;
    struct lista_s * prox;
}lista;

typedef struct retornoBusca_s{
    int custo;
    lista * path;
}retornoBusca_t;


void listaAdd(lista ** l, vertice * info);
void listaLibera(lista ** l);
void listaRemove(lista **l, int remove);

void criaGrafo(grafo** grafo, int numVertices, tipoGrafo type);
void liberaGrafo(grafo** grafo);
void verticeAddAresta(grafo** grafo, int origin, int destiny, int peso);
void grafoImprime(grafo ** grafo);
void grafoCriaRandom(grafo **grafo, int minV, int maxV, tipoGrafo tipo);
retornoBusca_t * buscaLargura(grafo **grafo, int vOrigin, int vDestiny);
