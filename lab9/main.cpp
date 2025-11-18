#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

struct Edge {
    int to;
    long long capacity;
    int rev;
};

bool bfs(int source, int sink, vector<vector<Edge>>& graph, vector<int>& parent, vector<int>& parentEdge) {
    fill(parent.begin(), parent.end(), -1);
    queue<int> q;
    q.push(source);
    parent[source] = -2;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < graph[u].size(); i++) {
            Edge& e = graph[u][i];
            if (parent[e.to] == -1 && e.capacity > 0) {
                parent[e.to] = u;
                parentEdge[e.to] = i;
                if (e.to == sink) {
                    return true;
                }
                q.push(e.to);
            }
        }
    }
    return false;
}

long long findMaxFlow(vector<vector<Edge>>& graph, int n, int source, int sink) {
    long long maxFlow = 0;
    vector<int> parent(n + 1);
    vector<int> parentEdge(n + 1);
    
    while (bfs(source, sink, graph, parent, parentEdge)) {
        long long pathFlow = LLONG_MAX;
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            int edgeIdx = parentEdge[v];
            pathFlow = min(pathFlow, graph[u][edgeIdx].capacity);
        }
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            int edgeIdx = parentEdge[v];
            graph[u][edgeIdx].capacity -= pathFlow;
            graph[v][graph[u][edgeIdx].rev].capacity += pathFlow;
        }
        
        maxFlow += pathFlow;
    }
    
    return maxFlow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<Edge>> graph(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        long long capacity;
        cin >> u >> v >> capacity;  
        graph[u].push_back({v, capacity, (int)graph[v].size()});
        graph[v].push_back({u, 0, (int)graph[u].size() - 1});
    }
    
    cout << findMaxFlow(graph, n, 1, n) << endl;
    
    return 0;
}

