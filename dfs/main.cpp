#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <Profiler.h>
Profiler prof("da");
int op{};

class Graph {
private:
    std::vector<std::list<int>> node;
    int vertices, edges;
    std::vector<bool> visited;
    enum {
        WHITE, GRAY, BLACK
    };

    void tSort(int v, bool visited1[],
               std::stack<int> &Stack) {
        // Mark the current node as visited.
        visited1[v] = true;

        // Recur for all the vertices adjacent to this vertex
        std::list<int>::iterator i;
        for (i = node[v].begin(); i != node[v].end(); ++i)
            if (!visited[*i])
                tSort(*i, visited1, Stack);

        // Push current vertex to stack which stores result
        Stack.push(v);
    }


public:
    Graph(int vertices, int edges) {
        this->vertices = vertices;
        this->edges = edges;
        node.resize(vertices);
        visited.resize(vertices);
        std::vector<bool>::iterator i;
        for (i = visited.begin(); i != visited.end(); i++)
            visited[*i] = WHITE;
    }

    void printGraph() {
        for (int i = 0; i < vertices; i++) {
            std::cout << i << ": ";
            for (auto const &j: node[i])
                std::cout << j << " ";
            std::cout << "\n";
        }
    }

    template<typename... Args>
    void addEdge(int source, Args... targets) {
        (node[source].push_back(targets), ...);
    }

    void dfs(int start) {
        std::cout << start << " ";
        visited[start] = GRAY;

        std::list<int>::iterator i;
        for (i = node[start].begin(); i != node[start].end(); i++)
            if (visited[*i] == WHITE) {
                dfs(*i);
            }

        visited[*i] = BLACK;
    }

    void tarjan() {
        //  TODO: implement tarjan
    }

    void topologicalSort(int start, int ok, Operation vertex, Operation edge) {
        visited[start] = GRAY;

        std::list<int>::iterator i;
        for (i = node[start].begin(); i != node[start].end(); i++)
            if (visited[*i] == WHITE) {
                dfs(*i);
            }

        visited[start] = BLACK;
        std::cout << start << " ";
    }

    void dfs_conn(int number) {
        bool matrix[vertices][vertices];
        for (int i = 0; i < vertices; i++)
            for (int j = 0; j < vertices; j++) {
                matrix[i][j] = false;
                matrix[i][i] = true;
            }

        int k = 0;

        int a[1], b[1];
        while (k < number) {
            FillRandomArray(a, 1, 0, vertices - 1, true, UNSORTED);
            FillRandomArray(b, 1, 0, vertices - 1, true, UNSORTED);

            if (!matrix[a[0]][b[0]]) {
                k++;
                addEdge(a[0], b[0]);
                matrix[a[0]][b[0]] = true;
            }
        }
    }
};

void perf() {
    int starting_size = 1000, final_size = 4500;
    int increment = 100;


    for (int n = starting_size; n <= final_size; n += increment) {
        op = 0;
        Operation vertex = prof.createOperation("vertices", n);
        Graph node(100, n);
        int a[1];
        FillRandomArray(a, 1, 0, 99, true, UNSORTED);
        node.dfs_conn(n);
        node.dfs(a[0]);
        //std::cout << "." << "\n";
        vertex.count(op + n);
    }

    for (int n = 100; n <= 200; n += 10) {
        op = 0;
        Operation edges = prof.createOperation("edges", n);
        Graph node(n, 4500);
        int a[1];
        FillRandomArray(a, 1, 0, n - 1, true, UNSORTED);
        node.dfs_conn(4500);
        node.dfs(a[0]);
        //std::cout << "." << "\n";
        edges.count(4500 + n);
    }

    prof.showReport();
}

int main() {
    Graph node(8, 10);
    Operation edge = prof.createOperation("edge", 10);
    Operation vertex = prof.createOperation("vertice", 10);

//    node.addEdge(0, 2, 3);
//    node.addEdge(1, 5);
//    node.addEdge(2, 0, 4);
//    node.addEdge(4, 6);
//    node.addEdge(5, 1, 6, 7);
//    node.addEdge(6, 4, 7);
//    node.addEdge(7, 5);
    node.dfs_conn(10);
    node.printGraph();
    std::cout << "\n\n";
    node.dfs(1);


    std::cout << "\n\n";
    Graph node2(9, 13);
    node2.addEdge(1, 3);
    node2.addEdge(3, 6);
    node2.addEdge(2, 5, 4);
    node2.addEdge(4, 7, 6);
    node2.addEdge(5, 7);
    node2.addEdge(7, 8, 0);
    node2.topologicalSort(2, 9, vertex, edge);

    //perf();
    return 0;
}
