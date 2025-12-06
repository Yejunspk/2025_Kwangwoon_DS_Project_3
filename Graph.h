#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <map>
#include <set>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

// Base graph  (adjacency retrieval/insert/print).
class Graph{	
protected:
	bool m_Type;   // true: adjacency list, false: adjacency matrix
	int m_Size;    // vertices number

public:
	Graph(bool type, int size);
	virtual ~Graph();

	// Accessors
	bool getType();	
	int getSize();

	// Adjacency providers
	// getAdjacentEdges: undirect view (out + in)
	// getAdjacentEdgesDirect: direct view (out only)
	virtual void getAdjacentEdges(int vertex, map<int, int>* m) = 0;		
	virtual void getAdjacentEdgesDirect(int vertex, map<int, int>* m) = 0;	

	// Insert weighted edge u->v with weight
	virtual void insertEdge(int from, int to, int weight) = 0;				

	// Print the graph to fout in required format
	virtual	bool printGraph(ofstream *fout) = 0;
};

#endif