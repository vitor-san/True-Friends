#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

typedef struct edgeNode {
    struct edgeNode *next;
    int vertex;
    double weight;
} EdgeNode;

typedef struct vertexNode {
    EdgeNode *beg, *end;
    void *data;
    int numConnec;
} VertexNode;

struct graph {
    VertexNode *list;
    int numVt;
    int numEd;
    int isDigraph;
    void (*freeFunction)(void *);
};

// n = number of vertices, d = is digraph
Graph newGraph(int n, int d, void (*freeFunction)(void *)) {
    Graph g = malloc(sizeof(GRAPH));  // creates the graph
    if (g == NULL) {
        fprintf(stderr, "Could not allocate memory to graph struct.\n");
        return NULL; // error
    }

    if (n <= 0) {
        fprintf(stderr, "Number of vertices should be a natural number.\n");
        return NULL;  // error
    }

    if (freeFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.\n");
        return NULL;  // error
    }

    g->list = calloc(n, sizeof(VertexNode)); // instantializes the list (no edges included)
    if (g->list == NULL) {
        fprintf(stderr, "Could not allocate memory to adjacency list.\n");
        return NULL; // error
    }

    g->numVt = n;
    g->numEd = 0;
    if (d < 0 || d > 1) d = 1;
    g->isDigraph = d;
    g->freeFunction = freeFunction;

    return g;
}

void freeList(Graph g, int freeData) {
    EdgeNode *aux;
    if (g->list != NULL) {
        for (int i = 0; i < g->numVt; i++) {
            if (g->list[i].beg != NULL) {
                aux = g->list[i].beg;
                while (g->list[i].beg->next != NULL) {
                    g->list[i].beg = g->list[i].beg->next;
                    free(aux);
                    aux = g->list[i].beg;
                }
                free(aux);
            }
            if (freeData) g->freeFunction(g->list[i].data);
        }
        free(g->list);
    }
}

int isAdjacent(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0; // error
    }

    if (g->list[x].beg != NULL) {
        EdgeNode *aux = g->list[x].beg;
        while (aux != NULL) {
            if (aux->vertex == y) return 1;
            aux = aux->next;
        }
    }
    else return 0;
}

// creates x -> y
int addEdge(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0;
    }

    if (isAdjacent(g, x, y)) {
        fprintf(stderr, "Edge (%d, %d) already exists.\n", x, y);
        return 0;
    }

    EdgeNode *n = malloc(sizeof(EdgeNode)); // new edge
    if (n == NULL) return 0;
    n->next = NULL;
    n->vertex = y;
    n->weight = 1;

    if (g->list[x].beg == NULL) {
        g->list[x].beg = n;
        g->list[x].end = n;
    }
    else {
        g->list[x].end->next = n;
        g->list[x].end = n;
    }

    int scndRtrn = 0;   // second return (in case it's a digraph)
    if (!g->isDigraph && !isAdjacent(g, y, x)) scndRtrn = addEdge(g, y, x);

    g->list[x].numConnec++;
    g->numEd++;

    if (!g->isDigraph) return scndRtrn;
    else return 1;
}

// removes x -> y
int removeEdge(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0;
    }

    if (g->list[x].beg != NULL) {
        // tries to find y node
        EdgeNode *cur = g->list[x].beg;
        EdgeNode *prev = NULL;

        while (cur != NULL && cur->vertex != y) {
            prev = cur;
            cur = cur->next;
        }

        if (cur == NULL) return 0;  // there's no edge from x to y
        else {  // the edge exists!
            if (prev == NULL) {   // cur points to the beginning of the list
                g->list[x].beg = cur->next;  // updates value of begin
                if (cur->next == NULL) g->list[x].end = NULL;  // updates value of end
                free(cur);  // deletes node
            }
            else {   // cur is not on the beginning of the list
                if (cur == g->list[x].end) {  // y is in the last node of the list
                    prev->next = NULL;  // cuts reference to cur
                    g->list[x].end = prev;  // updates value of end
                    free(cur);  // deletes node
                }
                else {  // y is in the "middle" of the list
                    prev->next = cur->next;   // cuts reference to cur
                    free(cur);  // deletes node
                }
            }
        }
    }
    else return 0;  // there's no edge from x to y

    if (!g->isDigraph && isAdjacent(g, y, x)) removeEdge(g, y, x);

    g->list[x].numConnec--;
    g->numEd--;

    return 1;
}

/* REMEMBER TO FREE() THE ARRAY RETURNED BY THIS FUNCTION!!! */
int *neighbors(Graph g, int x) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return NULL;
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return NULL;
    }

    int connections = g->list[x].numConnec;
    int *neigh = malloc((connections+1)*sizeof(int));   // allocates space for all the neighbors + a '0' that indicates the end of the list
    if (neigh == NULL) {
        fprintf(stderr, "Could not allocate memory to neighbors array.\n");
        return NULL;
    }

    EdgeNode *aux = g->list[x].beg;
    for (int i = 0; i < connections; i++) {
        neigh[i] = aux->vertex;
        aux = aux->next;
    }
    neigh[connections] = 0;   // terminator

    return neigh;
}

int addVertex(Graph g, int x) {     // adds the vertex x, if it is not there
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;
    }

    if (x < 0) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0;  // number of vertex shall be at least zero
    }

    if (x >= g->numVt) {
        VertexNode *aux = calloc(2*(g->numVt), sizeof(VertexNode));
        if (aux == NULL) {
            fprintf(stderr, "Could not expand graph's size.\n");
            return -1;
        }
        memcpy(aux, g->list, g->numVt*sizeof(VertexNode));  //makes a copy of the adjacency list
        freeList(g, 0);    //destroys the old one
        g->list = aux;  //g->list is going to point to the bigger block of data that was allocated

        g->numVt++;
        return 1;   // success
    }
    else {
        if (g->list[x].beg != NULL || g->list[x].data != NULL) return 0;
        else return 1;
    }

}

int removeVertex(Graph g, int x) {     // removes the vertex x, if it is there
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0;  // you shall remove a vertex that already exists
    }

    // first, I will remove all edges that ends on x
    for (int i = 0; i < g->numVt; i++) {
        if (isAdjacent(g, i, x)) removeEdge(g, i, x);
    }
    // now, I will remove all edges that starts on x (if g is a digraph; otherwise, they are already removed)
    if (g->isDigraph) {
        int *neigh = neighbors(g, x);
        int i = 0;
        while (neigh[i] != 0) removeEdge(g, x, neigh[i++]);
        free(neigh);
    }
    // erases the data in vertex x
    g->list[x].data = 0;

    return 1;   // success
}

void *searchVertexReturnData(Graph g, int (*compareFunction)(void *, void *), void *data) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return NULL;
    }

    if (compareFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.\n");
        return NULL;
    }

    if (data == NULL) {
        fprintf(stderr, "Invalid data passed as argument.\n");
        return NULL;
    }

    //search begins
    for (int i = 0; i < g->numVt; i++) {
        if (compareFunction(g->list[i].data, data) == 0) return g->list[i].data;
    }
    return NULL;
}

int searchVertexReturnPos(Graph g, int (*compareFunction)(void *, void *), void *data) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return -1;
    }

    if (compareFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.\n");
        return -1;
    }

    if (data == NULL) {
        fprintf(stderr, "Invalid data passed as argument.\n");
        return -1;
    }

    //search begins
    for (int i = 0; i < g->numVt; i++) {
        if (compareFunction(g->list[i].data, data) == 0) return i;
    }
    return -1;
}

int numVertices(Graph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph doesn't exist.\n");
        return -1;  // error
    }
    return g->numVt;
}

int numEdges(Graph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph doesn't exist.\n");
        return -1;  // error
    }
    return g->numEd;
}

double getEdgeCost(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return -1;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return -1;  // error
    }

    EdgeNode *aux = g->list[x].beg;
    while (aux != NULL && aux->vertex != y) aux = aux->next;

    if (aux == NULL) return -1;   // error: there is no edge from x to y
    else return aux->weight;
}

void setEdgeCost(Graph g, int x, int y, double val) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return;  // error
    }

    EdgeNode *aux = g->list[x].beg;
    while (aux != NULL && aux->vertex != y) aux = aux->next;

    if (aux == NULL) return;   // error: there is no edge from x to y
    else {
        if (aux->weight == val) return;
        aux->weight = val;
    }
    if (!g->isDigraph) setEdgeCost(g, y, x, val);
}

void *getVertexData(Graph g, int x) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return 0;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return 0;  // error
    }

    return g->list[x].data;
}

int vertexDegree(Graph g, int x) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return -1;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return -1;  // error
    }

    return g->list[x].numConnec;
}


int find(int x,int* pai) {
    if (pai[x] == x) return x;
    pai[x] = find(pai[x],pai);
}

void join(int a, int b,int* pai) {

    a = find(a,pai);
    b = find(b,pai);

    pai[b] = a;

}

Tuple* kruskal(Graph g,int* size) {

    Tuple* allEdges = malloc(sizeof(Tuple) * g->numEd);

    int pos = 0;

    for (int i = 0; i < g->numVt; i++) {
        int j = 0;
        int* neig = neighbors(g,i);
        while(neig[j] != 0) {
                Set(&(allEdges[pos]),i,neig[j],getEdgeCost(g,i,neig[j]));
                pos++;
                j++;
        }

        if(neig != NULL) free(neig);
    }

    qsort(allEdges,g->numEd,sizeof(Tuple),compTuple);

    int* pai = malloc(g->numVt*sizeof(int));
    for (int i = 0; i < g->numVt; i++) pai[i] = i;

    Tuple* mst = malloc(g->numVt*sizeof(Tuple));
    pos = 0;

    for (int i = 0; i < g->numEd; i++) {

        if(find(First(allEdges[i]),pai) != find(Second(allEdges[i]),pai)) {
            join(First(allEdges[i]),Second(allEdges[i]),pai);
            Set(&(mst[pos]),First(allEdges[i]),Second(allEdges[i]),Third(allEdges[i]));
            pos++;
        }

    }

    free(allEdges);
    free(pai);

    *size = pos;

    return mst;

}



void setVertexData(Graph g, int x, void *val) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.\n");
        return;  // error
    }

    g->list[x].data = val;
}

void printGraph(Graph g, void (*printFunction)(void *), int verbose) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.\n");
        return;
    }

    if (printFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.\n");
        return;  // error
    }

    EdgeNode *aux;
    for (int i = 0; i < g->numVt; i++) {
        if (g->list[i].beg != NULL) {
            aux = g->list[i].beg;
            if (verbose) {
                printf("Vertex %d (", i);
                printFunction(g->list[i].data);
                printf(") -> ");
            }
            else printf("%d -> ", i);

            while (aux->next != NULL) {
                if (verbose) printf("(%d, %.2lf), ", aux->vertex, aux->weight);
                else printf("%d, ", aux->vertex);
                aux = aux->next;
            }

            if (verbose) printf("(%d, %.2lf)\n", aux->vertex, aux->weight);
            else printf("%d\n", aux->vertex);
        }
        else {
            if (verbose) {
                printf("Vertex %d (", i);
                printFunction(g->list[i].data);
                printf(") -> NONE\n");
            }
            else printf("%d -> NONE\n", i);
        }
    }
}

void printNeighbors(Graph g, int x) {
    int *vet = neighbors(g, x), i = 0;
    if (vet == NULL) return;

    printf("Neighbors of %d: ", x);
    while (vet[i] != 0) printf("%d ", vet[i++]);
    printf("\n");

    free(vet);
}

void freeGraph(Graph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph doesn't exist.\n");
        return;
    }
    freeList(g, 1);
    free(g);
}
