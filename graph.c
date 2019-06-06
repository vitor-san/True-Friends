#include <stdio.h>
#include <stdlib.h>
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
        fprintf(stderr, "Could not allocate memory to graph struct.");
        return NULL; // error
    }

    if (n <= 0) {
        fprintf(stderr, "Number of vertices should be a natural number.");
        return NULL;  // error
    }

    if (freeFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.");
        return NULL;  // error
    }

    g->list = malloc(n*sizeof(VertexNode)); // instantializes the list (no edges included)
    if (g->list == NULL) {
        fprintf(stderr, "Could not allocate memory to adjacency list.");
        return NULL; // error
    }

    for (int i = 0; i < n; i++) {   // initializes all the begs and ends with NULL
        g->list[i].beg = NULL;
        g->list[i].end = NULL;
        g->list[i].data = 0;
        g->list[i].numConnec = 0;
    }

    g->numVt = n;
    g->numEd = 0;
    if (d < 0 || d > 1) d = 1;
    g->isDigraph = d;
    g->freeFunction = freeFunction;

    return g;
}

int isAdjacent(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return 0;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
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
        fprintf(stderr, "Invalid graph.");
        return 0;
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return 0;
    }

    if (isAdjacent(g, x, y)) {
        fprintf(stderr, "Edge (%d, %d) already exists.", x, y);
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
        fprintf(stderr, "Invalid graph.");
        return 0;
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
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
        fprintf(stderr, "Invalid graph.");
        return NULL;
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return NULL;
    }

    int connections = g->list[x].numConnec;
    int *neigh = malloc((connections+1)*sizeof(int));   // allocates space for all the neighbors + a '0' that indicates the end of the list
    if (neigh == NULL) {
        fprintf(stderr, "Could not allocate memory to neighbors array.");
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
        fprintf(stderr, "Invalid graph.");
        return 0;
    }

    if (x < 0) {
        fprintf(stderr, "Invalid vertex number.");
        return 0;  // number of vertex shall be at least zero
    }

    if (x >= g->numVt) {
        g->list = realloc(g->list, 2*(g->numVt)*sizeof(VertexNode));
        if (g->list == NULL) {
            fprintf(stderr, "Super error: you have lost the adjacency list you have previously.");
            return -1;   // super error: you lost the graph you had previously (probably will never happen)
        }

        g->list[x].data = 0;
        g->list[x].beg = NULL;
        g->list[x].end = NULL;
        g->numVt++;

        return 1;   // success
    }
    else {
        if (g->list[x].beg != NULL || g->list[x].data != 0) return 0;
        else {
            g->numVt++;
            return 1;
        }
    }

}

int removeVertex(Graph g, int x) {     // removes the vertex x, if it is there
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return 0;
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
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

void *searchVertex(Graph g, int (*compareFunction)(void *, void *), void *data) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return NULL;
    }

    if (compareFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.");
        return NULL;
    }

    if (data == NULL) {
        fprintf(stderr, "Invalid data passed as argument.");
        return NULL;
    }

    //search begins
    for (int i = 0; i < g->numVt; i++) {
        if (compareFunction(g->list[i].data, data) == 0) return g->list[i].data;
    }
    return NULL;
}

int numVertices(Graph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph doesn't exist.");
        return -1;  // error
    }
    return g->numVt;
}

int numEdges(Graph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph doesn't exist.");
        return -1;  // error
    }
    return g->numEd;
}

double getEdgeCost(Graph g, int x, int y) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return -1;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return -1;  // error
    }

    EdgeNode *aux = g->list[x].beg;
    while (aux != NULL && aux->vertex != y) aux = aux->next;

    if (aux == NULL) return -1;   // error: there is no edge from x to y
    else return aux->weight;
}

void setEdgeCost(Graph g, int x, int y, double val) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return;  // error
    }

    if (x < 0 || x >= g->numVt || y < 0 || y >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return;  // error
    }

    EdgeNode *aux = g->list[x].beg;
    while (aux != NULL && aux->vertex != y) aux = aux->next;

    if (aux == NULL) return;   // error: there is no edge from x to y
    else aux->weight = val;
}

void *getVertexData(Graph g, int x) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return 0;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return 0;  // error
    }

    return g->list[x].data;
}

int vertexDegree(Graph g, int x) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return -1;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return -1;  // error
    }

    return g->list[x].numConnec;
}

void setVertexData(Graph g, int x, void *val) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return;  // error
    }

    if (x < 0 || x >= g->numVt) {
        fprintf(stderr, "Invalid vertex number.");
        return;  // error
    }

    g->list[x].data = val;
}

void printGraph(Graph g, void (*printFunction)(void *), int verbose) {
    if (g == NULL || g->list == NULL) {
        fprintf(stderr, "Invalid graph.");
        return;
    }

    if (printFunction == NULL) {
        fprintf(stderr, "Invalid function passed as argument.");
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
        fprintf(stderr, "Graph doesn't exist.");
        return;
    }
    // free list:
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
            g->freeFunction(g->list[i].data);
        }
        free(g->list);
    }
    // free graph:
    free(g);
}
