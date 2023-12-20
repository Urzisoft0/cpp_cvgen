#include <string.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <string>
#include "bfs.h"
std::queue<Node *> q;

int get_neighbors(const Grid *grid, Point p, Point neighb[]) {


    /*
     * dx[] = {}
     * dy[] = {}
     *
     * for(i: 0 -> 4)
     * {
     *      a = p->row + dy[i]
     *      b = p->col + dx[i]
     *      if(grid->mat[a][b] == 0)
     *          neighb[cont] = mat[a][b]
     *          cont += 1
     *
     * }
     * return cont;
     * */
    int i = 0;
    int nr_neigh = 0;
    if (grid->mat[p.row][p.col] == 0) {
        if (grid->mat[p.row - 1][p.col] == 0 && p.row - 1 >= 0) {
            neighb[i].row = p.row - 1;
            neighb[i++].col = p.col;
            nr_neigh++;
        }

        if (grid->mat[p.row][p.col + 1] == 0 && p.col + 1 < grid->cols) {
            neighb[i].row = p.row;
            neighb[i++].col = p.col + 1;
            nr_neigh++;
        }

        if (grid->mat[p.row + 1][p.col] == 0 && p.row + 1 < grid->rows) {
            neighb[i].row = p.row + 1;
            neighb[i++].col = p.col;
            nr_neigh++;
        }

        if (grid->mat[p.row][p.col - 1] == 0 && p.col - 1 >= 0) {
            neighb[i].row = p.row;
            neighb[i++].col = p.col - 1;
            nr_neigh++;
        }
    }
    return nr_neigh;
}

//pt cal

int get_neighbors2(const Grid *grid, Point p, Point neighb[]) {

    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {-2, 1, 1, 2, 2, 1, -1, -2};
    int nr_neigh{};
    int j = 0;
    if (!grid->mat[p.row][p.col]) {
        for (int i = 0; i < 8; i++) {
            int a = p.row + dx[i];
            int b = p.col + dy[i];

            if ((a >= 0 && a < grid->rows) && (b >= 0 && b < grid->cols)) {
                neighb[j].row = a;
                neighb[j].col = b;
                j++;
                nr_neigh++;
            }
        }
    }

    return nr_neigh;
}


void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[8];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph) {
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op) {
    int i = 0, j = 0;
    int index = 0;
    q.push(s);
    if (op != nullptr)
        op->count();
    s->color = COLOR_GRAY;
    while(!q.empty()) {
        Node *temp = q.front();
        if (op != nullptr)
            op->count();

        int dist = 1;
        while (index < temp->adjSize) {
            if (temp->adj[index]->color == COLOR_WHITE) {
                q.push(temp->adj[index]);
                temp->adj[index]->color = COLOR_GRAY;
                temp->adj[index]->dist = dist;

                dist++;
//                if (index == 0)
                    temp->adj[index]->parent = q.front();
//                else
//                    temp->adj[index]->parent = temp->adj[index - 1];
                if (op != nullptr)
                    op->count(4);
            }
            index++;
        }
        dist = 1;
        index = 0;
        q.front()->color = COLOR_BLACK;
        //graph->v[i] = q.front();
        if (op != nullptr)
            op->count();
        q.pop();
    }
}

//functia de pretty print
void printParent(int vector[], int parent, int indent, int n, Point *position) {
    for (int i = 1; i <= n; i++)
        if (vector[i] == parent) {
            std::cout << std::string(indent, ' ') << "(" << position[i].row
            << ", " << position[i].col << ")" << "\n";
            printParent(vector, i, indent + 4, n, position);
        }
}

void print_bfs_tree(Graph *graph) {
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the rep

    // representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    int indent = 0;
    if(!err) {

        //actual code
        int parent = 0;
        for (int i = 0; i < n; i++)
            if (p[i] == -1) {
                parent = i;
                break;
            }

        std::cout << "(" << repr[parent].row << ", " << repr[parent].col << ")\n";
        printParent(p, parent, 4, n, repr);
        //pana aici
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

//shortest path

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) {

    bfs(graph, end, nullptr);
    if (start->dist != 0) {
        Node *p = start;
        int i = 0;
        while (p != end) {
            path[i] = p;
            p = p->parent;
            i++;
//        if (start->dist != 0)
//            return start->dist;
//        else
//            return -1;
        }
        return i;
    }
    else
        return -1;
}

//
void performance()
{
    int n;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(int i=0; i<graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        for (int i = 0; i < graph.nrNodes; i++)
            graph.v[i]->adj = (Node**)malloc(sizeof(Node *));
        int nrAdj = 0;
        for (int i = 0; i < (graph.nrNodes * (graph.nrNodes - 1)) / 2; i++) {


        }
        nrAdj++;
        // TODO: generate n random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        //free_graph(&graph);
    }

    // vary the number of vertices
//    for(n=100; n<=200; n+=10){
//        Operation op = p.createOperation("bfs-vertices", n);
//        Graph graph;
//        graph.nrNodes = n;
//        //initialize the nodes of the graph
//        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
//        for(i=0; i<graph.nrNodes; ++i){
//            graph.v[i] = (Node*)malloc(sizeof(Node));
//            memset(graph.v[i], 0, sizeof(Node));
//        }
//        // TODO: generate 4500 random edges
//        // make sure the generated graph is connected
//
//        bfs(&graph, graph.v[0], &op);
//        free_graph(&graph);
//    }

    p.showReport();
}
