#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "GraphMethod.h"

// Command 
class Manager{	
private:
	Graph* graph;	    // current graph 
	ofstream fout;	    // log.txt 
	int load;           // 0 = not loaded, 1 = loaded

public:
	Manager();	
	~Manager();	

	// Run using a command file
	void run(const char * command_txt);
	
	// Commands
	bool LOAD(const char* filename);	
	bool PRINT();	
	bool mBFS(char option, int vertex);	
	bool mDFS(char option, int vertex);	
	bool mDIJKSTRA(char option, int vertex);	
	bool mKRUSKAL();	
	bool mBELLMANFORD(char option, int s_vertex, int e_vertex);	
	bool mFLOYD(char option); 
	bool mCentrality();

	// Error 
	void printErrorCode(int n); 
};

#endif
