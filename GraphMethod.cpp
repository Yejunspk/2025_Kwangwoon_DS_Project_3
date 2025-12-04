#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <limits>
#include <algorithm>

using std::cout;
using std::endl;

// ---------- Utilities ----------
static void getNeighbors(Graph* g, char option, int u, std::map<int,int>& adj) {
	if(option == 'O') g->getAdjacentEdgesDirect(u, &adj);
	else g->getAdjacentEdges(u, &adj);
}

static bool hasNegativeEdge(Graph* g, char option) {
	int n = g->getSize();
	std::map<int,int> m;
	for(int u=0; u<n; ++u) {
		getNeighbors(g, option, u, m);
		for(auto& kv : m) if(kv.second < 0) return true;
	}
	return false;
}

static bool isConnectedUndirected(Graph* g) {
	int n = g->getSize();
	if(n == 0) return false;
	std::vector<int> visited(n, 0);
	std::queue<int> q;
	q.push(0); visited[0] = 1;
	std::map<int,int> m;
	while(!q.empty()) {
		int u = q.front(); q.pop();
		g->getAdjacentEdges(u, &m); // undirected view
		for(auto& kv : m) if(!visited[kv.first]) {
			visited[kv.first] = 1;
			q.push(kv.first);
		}
	}
	for(int i=0;i<n;++i) if(!visited[i]) return false;
	return true;
}

// ---------- BFS ----------
bool BFS(Graph* graph, char option, int vertex)
{
	int n = graph->getSize();
	std::vector<int> visited(n, 0);
	std::queue<int> q;
	std::vector<int> order;

	cout << "========BFS========\n";
	cout << (option=='O' ? "Directed Graph BFS" : "Undirected Graph BFS") << "\n";
	cout << "Start: " << vertex << "\n";

	visited[vertex] = 1;
	q.push(vertex);
	while(!q.empty()){
		int u = q.front(); q.pop();
		order.push_back(u);
		std::map<int,int> adj;
		getNeighbors(graph, option, u, adj);
		for(const auto& kv : adj){ // map keeps ascending order
			int v = kv.first;
			if(!visited[v]) {
				visited[v] = 1;
				q.push(v);
			}
		}
	}

	// print order
	for(size_t i=0;i<order.size();++i){
		if(i) cout << " -> ";
		cout << order[i];
	}
	cout << "\n======================\n\n";
	return true;
}

// ---------- DFS ----------
static void dfsRec(Graph* g, char option, int u, std::vector<int>& visited, std::vector<int>& order) {
	visited[u]=1;
	order.push_back(u);
	std::map<int,int> adj;
	getNeighbors(g, option, u, adj);
	for(const auto& kv: adj) {
		int v = kv.first;
		if(!visited[v]) dfsRec(g, option, v, visited, order);
	}
}

bool DFS(Graph* graph, char option, int vertex)
{
	int n = graph->getSize();
	std::vector<int> visited(n, 0);
	std::vector<int> order;
	cout << "========DFS========\n";
	cout << (option=='O' ? "Directed Graph DFS" : "Undirected Graph DFS") << "\n";
	cout << "Start: " << vertex << "\n";

	dfsRec(graph, option, vertex, visited, order);

	for(size_t i=0;i<order.size();++i){
		if(i) cout << " -> ";
		cout << order[i];
	}
	cout << "\n======================\n\n";
	return true;
}

// ---------- Kruskal (Undirected, Weighted) ----------
struct Edge { int u,v,w; };
struct Dsu {
	std::vector<int> p, r;
	explicit Dsu(int n): p(n), r(n,0){ for(int i=0;i<n;++i) p[i]=i; }
	int f(int x){ return p[x]==x?x:p[x]=f(p[x]); }
	bool unite(int a,int b){ a=f(a); b=f(b); if(a==b) return false; if(r[a]<r[b]) std::swap(a,b); p[b]=a; if(r[a]==r[b]) r[a]++; return true; }
};

bool Kruskal(Graph* graph)
{
	int n = graph->getSize();
	// connectivity check
	if(!isConnectedUndirected(graph)) {
		cout << "========ERROR========\n";
        cout << "500\n";
        cout << "======================\n\n";
		return false;
	}

	// build undirected unique edge set (u<v)
	std::vector<Edge> edges;
	std::set<std::pair<int,int>> seen;
	for(int u=0; u<n; ++u) {
		std::map<int,int> adj;
		graph->getAdjacentEdges(u, &adj); // undirected view
		for(const auto& kv : adj){
			int v = kv.first, w = kv.second;
			int a = std::min(u,v), b = std::max(u,v);
			if(seen.insert({a,b}).second){
				edges.push_back({a,b,w});
			}
		}
	}

	std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b){ return a.w < b.w; });

	Dsu dsu(n);
	std::vector<std::vector<std::pair<int,int>>> mst(n);
	long long total = 0;
	for(const auto& e: edges){
		if(dsu.unite(e.u, e.v)){
			mst[e.u].push_back({e.v, e.w});
			mst[e.v].push_back({e.u, e.w});
			total += e.w;
		}
	}

	cout << "========KRUSKAL========\n";
	// print adjacency list of MST (neighbors sorted by vertex id)
	for(int i=0;i<n;++i){
		std::sort(mst[i].begin(), mst[i].end());
		cout << "[" << i << "]";
		if(!mst[i].empty()) cout << " ";
		for(size_t k=0;k<mst[i].size();++k){
			if(k) cout << " ";
			cout << mst[i][k].first << "(" << mst[i][k].second << ")";
		}
		cout << "\n";
	}
	cout << "Cost: " << total << "\n";
	cout << "======================\n\n";
	return true;
}

// ---------- Dijkstra ----------
bool Dijkstra(Graph* graph, char option, int start)
{
	if(hasNegativeEdge(graph, option)) {
		cout << "========ERROR========\n600\n======================\n\n";
		return false;
	}

	int n = graph->getSize();
	const long long INF = std::numeric_limits<long long>::max()/4;
	std::vector<long long> dist(n, INF);
	std::vector<int> parent(n, -1);
	using P = std::pair<long long,int>;
	std::priority_queue<P, std::vector<P>, std::greater<P> > pq;

	dist[start]=0;
	pq.push(P(0, start));  

	while(!pq.empty()){
		P cur = pq.top();   
		pq.pop();           
		long long d = cur.first; 
		int u = cur.second;      

		if(d!=dist[u]) continue;
		std::map<int,int> adj;
		getNeighbors(graph, option, u, adj);
		for(const auto& kv: adj){
			int v = kv.first; int w = kv.second;
			if(dist[v] > dist[u] + w){
				dist[v] = dist[u] + w;
				parent[v] = u;
				pq.push(P(dist[v], v)); 
			}
		}
	}

	cout << "========DIJKSTRA========\n";
	cout << (option=='O' ? "Directed Graph Dijkstra" : "Undirected Graph Dijkstra") << "\n";
	cout << "Start: " << start << "\n";

	for(int v=0; v<n; ++v){
		cout << "[" << v << "] ";
		if(dist[v] == INF){
			cout << "x\n";
			continue;
		}
		// reconstruct path
		std::vector<int> path;
		for(int x=v; x!=-1; x=parent[x]) path.push_back(x);
		std::reverse(path.begin(), path.end());
		for(size_t i=0;i<path.size();++i){
			if(i) cout << " -> ";
			cout << path[i];
		}
		cout << " (" << dist[v] << ")\n";
	}
	cout << "======================\n\n";
	return true;
}

// ---------- Bellman-Ford ----------
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex) 
{
	int n = graph->getSize();
	struct E { int u,v,w; };
	std::vector<E> edges;

	// Build edge list by option
	for(int u=0; u<n; ++u){
		std::map<int,int> adj;
		getNeighbors(graph, option, u, adj);
		for(const auto& kv: adj){
			edges.push_back({u, kv.first, kv.second});
		}
	}

	const long long INF = std::numeric_limits<long long>::max()/4;
	std::vector<long long> dist(n, INF);
	std::vector<int> parent(n, -1);
	dist[s_vertex] = 0;

	for(int i=0;i<n-1;++i){
		bool any=false;
		for(const auto& e: edges){
			if(dist[e.u] == INF) continue;
			if(dist[e.v] > dist[e.u] + e.w){
				dist[e.v] = dist[e.u] + e.w;
				parent[e.v] = e.u;
				any=true;
			}
		}
		if(!any) break;
	}
	// detect negative cycle
	for(const auto& e: edges){
		if(dist[e.u]!=INF && dist[e.v] > dist[e.u] + e.w){
			cout << "========ERROR========\n";
            cout << "700\n";
            cout << "======================\n\n";
			return false;
		}
	}

	cout << "========BELLMANFORD========\n";
	cout << (option=='O' ? "Directed Graph Bellman-Ford" : "Undirected Graph Bellman-Ford") << "\n";

	if(dist[e_vertex] == INF){
		cout << "x\n";
		cout << "Cost: x\n";
		cout << "======================\n\n";
		return true;
	}

	std::vector<int> path;
	for(int x=e_vertex; x!=-1; x=parent[x]) path.push_back(x);
	std::reverse(path.begin(), path.end());
	for(size_t i=0;i<path.size();++i){
		if(i) cout << " -> ";
		cout << path[i];
	}
	cout << "\nCost: " << dist[e_vertex] << "\n";
	cout << "======================\n\n";
	return true;
}

// ---------- Floyd-Warshall ----------
bool FLOYD(Graph* graph, char option)
{
	int n = graph->getSize();
	const long long INF = std::numeric_limits<long long>::max()/4;
	std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
	for(int i=0;i<n;++i) d[i][i]=0;

	for(int u=0; u<n; ++u){
		std::map<int,int> adj;
		getNeighbors(graph, option, u, adj);
		for(const auto& kv: adj) d[u][kv.first] = std::min<long long>(d[u][kv.first], kv.second);
	}

	// Floyd
	for(int k=0;k<n;++k){
		for(int i=0;i<n;++i){
			if(d[i][k]==INF) continue;
			for(int j=0;j<n;++j){
				if(d[k][j]==INF) continue;
				if(d[i][j] > d[i][k] + d[k][j]) d[i][j] = d[i][k] + d[k][j];
			}
		}
	}

	// negative cycle?
	for(int i=0;i<n;++i){
		if(d[i][i] < 0){
			cout << "========ERROR========\n";
            cout << "800\n";
            cout << "======================\n\n";
			return false;
		}
	}

	cout << "========FLOYD========\n";
	cout << (option=='O' ? "Directed Graph Floyd" : "Undirected Graph Floyd") << "\n";
	// header
	cout << "    ";
	for(int j=0;j<n;++j) cout << "[" << j << "] ";
	cout << "\n";
	// rows
	for(int i=0;i<n;++i){
		cout << "[" << i << "] ";
		for(int j=0;j<n;++j){
			if(d[i][j]==INF) cout << "x";
			else cout << d[i][j];
			if(j+1<n) cout << "  ";
		}
		cout << "\n";
	}
	cout << "======================\n\n";
	return true;
}

// ---------- Closeness Centrality (Undirected, Weighted) ----------
bool Centrality(Graph* graph) {
	int n = graph->getSize();
	const long long INF = std::numeric_limits<long long>::max()/4;
	// Reuse Floyd (undirected)
	// Build distances
	std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
	for(int i=0;i<n;++i) d[i][i]=0;
	for(int u=0; u<n; ++u){
		std::map<int,int> adj;
		graph->getAdjacentEdges(u, &adj); // undirected
		for(const auto& kv: adj) d[u][kv.first] = std::min<long long>(d[u][kv.first], kv.second);
	}
	for(int k=0;k<n;++k){
		for(int i=0;i<n;++i){
			if(d[i][k]==INF) continue;
			for(int j=0;j<n;++j){
				if(d[k][j]==INF) continue;
				if(d[i][j] > d[i][k] + d[k][j]) d[i][j] = d[i][k] + d[k][j];
			}
		}
	}
	for(int i=0;i<n;++i){
		if(d[i][i] < 0){
			cout << "========ERROR========\n";
            cout << "900\n";
            cout << "======================\n\n";
			return false;
		}
	}

	// Compute closeness centrality as (n-1) / sum of distances to others
	std::vector<long long> denom(n, 0);
	for(int u=0; u<n; ++u){
		long long sum=0;
		for(int v=0; v<n; ++v){
			if(u==v) continue;
			if(d[u][v] == INF) sum += 0; // unreachable contributes 0; spec doesn't define, we keep as is.
			else sum += d[u][v];
		}
		denom[u]=sum;
	}
	// find minimum denominator (max centrality)
	long long best = std::numeric_limits<long long>::max();
	for(int i=0;i<n;++i) best = std::min(best, denom[i]);

	cout << "========CENTRALITY========\n";
	for(int i=0;i<n;++i){
		cout << "[" << i << "] " << (n-1) << "/" << denom[i];
		if(denom[i] == best) cout << " <- Most Central";
		cout << "\n";
	}
	cout << "======================\n\n";
	return true;
}