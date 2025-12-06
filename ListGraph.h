#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

class ListGraph : public Graph{	
private:
	// adjacency list: m_List[u][v] = weight 
	std::map<int, int>* m_List;

public:	
	ListGraph(bool type, int size);
	~ListGraph();
		
	void getAdjacentEdges(int vertex, std::map<int, int>* m) override;	    // undirect view
	void getAdjacentEdgesDirect(int vertex, std::map<int, int>* m) override; // direct view
	void insertEdge(int from, int to, int weight) override;	
	bool printGraph(std::ofstream *fout) override;
};

#endif