#include <iostream>
#include <vector>
#include <list>

template <std::size_t T>
class Graph {
private:
    std::vector<std::list<int>> node;

    std::vector<bool> visited;
public:
    Graph() {
        node.resize(T);
        visited.resize(T);
        for (int i = 0; i < node.size(); i++)
            visited[i] = false;
    }

    void printGraph() {
        for (int i = 0; i < T; i++) {
            std::cout << i  << ": ";
            for (auto const &j: node[i])
                std::cout << j << " ";
            std::cout << "\n";
        }
    }

    void addEdge(int source, int target) {
        node[source].push_back(target);
    }

    void addEdge(int source, const std::vector<int>& targets) {
        for (auto &i: targets)
            node[source].push_back(i);
    }

    void dfs(int start) {
        std::cout << start << " ";
        visited[start] = true;

        std::list<int>::iterator i;
        for (i = node[start].begin(); i != node[start].end(); i++)
            if (!visited[*i])
                dfs(*i);
    }

    void Tarjan() {
        //  TODO: implement tarjan
    }

    void topological() {
        //  TODO: implement topological sort
    }
};

int main() {
    const int i = 8;
    Graph<i> node;

    node.addEdge(0, {2, 3});
    node.addEdge(1, 5);
    node.addEdge(2, {0, 4});
    node.addEdge(4, 6);
    node.addEdge(5, {1, 6, 7});
    node.addEdge(6, {4, 7});
    node.addEdge(7, 5);
    node.printGraph();
    std::cout << "\n\n";
    node.dfs(2);

    return 0;
}
