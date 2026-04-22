#include <vector>
#include <algorithm>

struct Edge {
    int u, v, weight;
    bool isSelected = false;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class KruskalManager {
private:
    std::vector<int> parent;
    std::vector<Edge> edges;
    int numNodes;

    int findRoot(int i) {
        if (parent[i] == i) return i;
        return parent[i] = findRoot(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = findRoot(i);
        int root_j = findRoot(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }

public:
    void Initialize(int n, const std::vector<Edge>& inputEdges) {
        this->numNodes = n;
        this->edges = inputEdges;
        
        parent.assign(n, -1);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    void MST() {
        std::sort(edges.begin(), edges.end());

        for (auto& edge : edges) {
            int rootU = findRoot(edge.u);
            int rootV = findRoot(edge.v);

            if (rootU != rootV) {
                unite(edge.u, edge.v);
                edge.isSelected = true;
            } else {
                edge.isSelected = false;
            }
        }
    }
};