#include "Graph.h"

// store flag
Graph::Graph(bool type, int size)
{
	m_Type = type;
	m_Size = size;
}

Graph::~Graph()	
{

}

// small getter
bool Graph::getType(){return m_Type;}	
int Graph::getSize(){return m_Size;}