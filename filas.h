typedef struct fila_s{
    vertice * prim;
    vertice * info;
    struct fila_s *prox;
}fila;


fila* filaCria();
void filaAdd(fila** actual, vertice* joined);
vertice* filaRemove(fila** f);
void filaLibera(fila** f);