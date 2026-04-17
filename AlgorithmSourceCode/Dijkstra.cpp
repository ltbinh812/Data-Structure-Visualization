#include <vector>
#include <queue>
#include <climits>

class Dijkstra {
private:
    int V; 
    std::vector<std::vector<std::pair<int, int>>> adj; 
    std::vector<int> dist;

public:
    void Initialize(int n, const std::vector<std::vector<std::pair<int, int>>>& adjList) {
        V = n;
        adj = adjList;
    }

    void CalculateDist(std::vector<int> sourceNodes) {
        dist.assign(V, INT_MAX);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        
        for(int sourceNode : sourceNodes) {
            dist[sourceNode] = 0;
            pq.push({0, sourceNode});
        }

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }
};