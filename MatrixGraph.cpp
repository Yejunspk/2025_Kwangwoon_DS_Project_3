#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int*[m_Size];
	for(int i = 0; i < m_Size; ++i) {
		m_Mat[i] = new int[m_Size];
		for(int j = 0; j < m_Size; ++j) m_Mat[i][j] = 0;
	}
}

MatrixGraph::~MatrixGraph()
{
	for(int i = 0; i < m_Size; ++i) delete [] m_Mat[i];
	delete [] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, std::map<int, int>* out) // undirected view
{
	out->clear();
	if(vertex < 0 || vertex >= m_Size) return;
	// outgoing
	for(int v = 0; v < m_Size; ++v) {
		if(m_Mat[vertex][v] != 0) (*out)[v] = m_Mat[vertex][v];
	}
	// incoming
	for(int u = 0; u < m_Size; ++u) {
		if(m_Mat[u][vertex] != 0) {
			auto it = out->find(u);
			if(it == out->end()) (*out)[u] = m_Mat[u][vertex];
			else it->second = std::min(it->second, m_Mat[u][vertex]);
		}
	}
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, std::map<int, int>* out)
{
	out->clear();
	if(vertex < 0 || vertex >= m_Size) return;
	for(int v = 0; v < m_Size; ++v) {
		if(m_Mat[vertex][v] != 0) (*out)[v] = m_Mat[vertex][v];
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)	
{
	if(from < 0 || from >= m_Size || to < 0 || to >= m_Size) return;
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(std::ofstream *fout)	
{
	if(!fout || !fout->is_open()) return false;

	(*fout) << "========PRINT========\n";
	// header
	(*fout) << "    ";
	for(int j = 0; j < m_Size; ++j) (*fout) << "[" << j << "] ";
	(*fout) << "\n";
	// rows
	for(int i = 0; i < m_Size; ++i) {
		(*fout) << "[" << i << "] ";
		for(int j = 0; j < m_Size; ++j) {
			(*fout) << m_Mat[i][j] << (j+1==m_Size? "" : "  ");
		}
		(*fout) << "\n";
	}
	(*fout) << "======================\n\n";
	return true;
}