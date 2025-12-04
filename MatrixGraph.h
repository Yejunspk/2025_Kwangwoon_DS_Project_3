#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <map>
#include "Graph.h"

class MatrixGraph : public Graph{	
private:
	// adjacency matrix: m_Mat[u][v] = weight, 0 mean no edge
	int** m_Mat;

public:
	MatrixGraph(bool type, int size);
	~MatrixGraph();
		
	void getAdjacentEdges(int vertex, std::map<int, int>* m) override;	
	void getAdjacentEdgesDirect(int vertex, std::map<int, int>* m) override;
	void insertEdge(int from, int to, int weight) override;	
	bool printGraph(std::ofstream *fout) override;
};

#endif