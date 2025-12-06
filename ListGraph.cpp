#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_List = new std::map<int,int>[m_Size];
}

ListGraph::~ListGraph()	
{
	delete [] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, std::map<int, int>* out) // undirect view
{
	out->clear();
	if(vertex < 0 || vertex >= m_Size) return;

	// 1) outgoing neighbor
	for(const auto& kv : m_List[vertex]) {
		(*out)[kv.first] = kv.second;
	}
	// 2) incoming neighbor
	for(int u = 0; u < m_Size; ++u) {
		auto it = m_List[u].find(vertex);
		if(it != m_List[u].end()) {
			// avoid overriding 
			if(out->find(u) == out->end()) (*out)[u] = it->second;
			else (*out)[u] = std::min((*out)[u], it->second);
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, std::map<int, int>* out) // directed view
{
	out->clear();
	if(vertex < 0 || vertex >= m_Size) return;
	for(const auto& kv : m_List[vertex]) {
		(*out)[kv.first] = kv.second;
	}
}

void ListGraph::insertEdge(int from, int to, int weight)
{
	if(from < 0 || from >= m_Size || to < 0 || to >= m_Size) return;
	m_List[from][to] = weight;
}

bool ListGraph::printGraph(std::ofstream *fout)
{
	if(!fout || !fout->is_open()) return false;

	(*fout) << "========PRINT=======\n";
	for(int u = 0; u < m_Size; ++u) {
		(*fout) << "[" << u << "]";
		if(!m_List[u].empty()) {
			(*fout) << " -> ";
			bool first = true;
			for(const auto& kv : m_List[u]) {
				if(!first) (*fout) << " -> ";
				first = false;
				(*fout) << "(" << kv.first << "," << kv.second << ")";
			}
		}
		(*fout) << "\n";
	}
	(*fout) << "======================\n\n";
	return true;
}
