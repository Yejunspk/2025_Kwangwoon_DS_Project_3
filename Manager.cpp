#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

Manager::Manager()	
{
	graph = nullptr;	
	// overwrite log.txt as required (not append)
	fout.open("log.txt", ios::out | ios::trunc); // 새로 저장
	load = 0;
}

Manager::~Manager()
{
	if(load) delete graph;	
	if(fout.is_open()) fout.close();
}

static std::vector<std::string> splitTokens(const std::string& line){
	std::stringstream ss(line);
	std::vector<std::string> t;
	std::string w;
	while(ss >> w) t.push_back(w);
	return t;
}

void Manager::run(const char* command_txt){
	ifstream fin;
	fin.open(command_txt, ios_base::in);
	if(!fin) { 
		fout<<"command file open error"<<endl;
		return;
	}

	std::string line;
	while(std::getline(fin, line)){
		if(line.empty()) continue;
		auto tk = splitTokens(line);
		if(tk.empty()) continue;

		std::string cmd = tk[0];

		// All commands are uppercase as per spec; proceed.
		if(cmd == "LOAD"){
			if(tk.size() != 2){ printErrorCode(100); continue; }
			LOAD(tk[1].c_str());
		}
		else if(cmd == "PRINT"){
			if(tk.size() != 1){ printErrorCode(200); continue; }
			PRINT();
		}
		else if(cmd == "BFS"){
			if(tk.size() != 3){ printErrorCode(300); continue; }
			char opt = tk[1][0];
			int s = std::stoi(tk[2]);
			if(!(opt=='O'||opt=='X')){ printErrorCode(300); continue; }
			mBFS(opt, s);
		}
		else if(cmd == "DFS"){
			if(tk.size() != 3){ printErrorCode(400); continue; }
			char opt = tk[1][0];
			int s = std::stoi(tk[2]);
			if(!(opt=='O'||opt=='X')){ printErrorCode(400); continue; }
			mDFS(opt, s);
		}
		else if(cmd == "KRUSKAL"){
			if(tk.size() != 1){ printErrorCode(500); continue; }
			mKRUSKAL();
		}
		else if(cmd == "DIJKSTRA"){
			if(tk.size() != 3){ printErrorCode(600); continue; }
			char opt = tk[1][0];
			int s = std::stoi(tk[2]);
			if(!(opt=='O'||opt=='X')){ printErrorCode(600); continue; }
			mDIJKSTRA(opt, s);
		}
		else if(cmd == "BELLMANFORD"){
			if(tk.size() != 4){ printErrorCode(700); continue; }
			char opt = tk[1][0];
			int s = std::stoi(tk[2]);
			int e = std::stoi(tk[3]);
			if(!(opt=='O'||opt=='X')){ printErrorCode(700); continue; }
			mBELLMANFORD(opt, s, e);
		}
		else if(cmd == "FLOYD"){
			if(tk.size() != 2){ printErrorCode(800); continue; }
			char opt = tk[1][0];
			if(!(opt=='O'||opt=='X')){ printErrorCode(800); continue; }
			mFLOYD(opt);
		}
		else if(cmd == "CENTRALITY"){
			if(tk.size() != 1){ printErrorCode(900); continue; }
			mCentrality();
		}
		else if(cmd == "EXIT"){
			// Always success
			fout << "========EXIT========\n";
			fout << "Success\n";
			fout << "======================\n\n";
			break;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	// delete previous
	if(load){ delete graph; graph=nullptr; load=0; }

	std::ifstream gin(filename);
	if(!gin){
		printErrorCode(100);
		return false;
	}
	char type_char='L';
	int n=0;
	gin >> type_char;
	gin >> n;
	if(!gin || n<=0){
		printErrorCode(100);
		return false;
	}

	if(type_char=='L') graph = new ListGraph(false, n);
	else graph = new MatrixGraph(true, n);

	// consume endline
	std::string rest;
	std::getline(gin, rest);

	if(type_char=='L'){
		// Robust parser for adjacency list:
		// Accepts lines like:
		// "0 1 2 3 4" (means from=0, edges (1,2),(3,4))
		// or two-line style:
		// "0"  then next line "1 2 3 4"
		int current_from = -1;
		std::string line;
		while(std::getline(gin, line)){
			if(line.empty()) continue;
			std::stringstream ss(line);
			std::vector<int> nums;
			int x;
			while(ss >> x) nums.push_back(x);
			if(nums.empty()) continue;

			if(nums.size()==1){
				current_from = nums[0];
			}else{
				if(current_from==-1){
					// treat first as from, rest as pairs
					int from = nums[0];
					for(size_t i=1;i+1<nums.size(); i+=2){
						int to=nums[i], w=nums[i+1];
						graph->insertEdge(from, to, w);
					}
				}else{
					for(size_t i=0;i+1<nums.size(); i+=2){
						int to=nums[i], w=nums[i+1];
						graph->insertEdge(current_from, to, w);
					}
					current_from = -1; // optional
				}
			}
		}
	}else{ // Matrix
		for(int i=0;i<n;++i){
			for(int j=0;j<n;++j){
				int w=0; gin >> w;
				if(!gin){ printErrorCode(100); return false; }
				if(w!=0) graph->insertEdge(i,j,w);
			}
		}
	}

	load = 1;
	fout << "========LOAD========\n";
	fout << "Success\n";
	fout << "======================\n\n";
	return true;
}

bool Manager::PRINT()	
{
	if(!load){
		printErrorCode(200);
		return false;
	}
	// Graph classes handle full block printing
	return graph->printGraph(&fout);
}

static bool checkStartVertex(Graph* g, int v){
	return v>=0 && v<g->getSize();
}

bool Manager::mBFS(char option, int vertex)	
{
	if(!load || !checkStartVertex(graph, vertex)){
		printErrorCode(300);
		return false;
	}
	// redirect cout to fout while GraphMethod prints
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = BFS(graph, option, vertex);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mDFS(char option, int vertex)	
{
	if(!load || !checkStartVertex(graph, vertex)){
		printErrorCode(400);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = DFS(graph, option, vertex);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mDIJKSTRA(char option, int vertex)	
{
	if(!load || !checkStartVertex(graph, vertex)){
		printErrorCode(600);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = Dijkstra(graph, option, vertex);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mKRUSKAL()
{
 	if(!load){
		printErrorCode(500);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = Kruskal(graph);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex) 
{
	if(!load || !checkStartVertex(graph, s_vertex) || !checkStartVertex(graph, e_vertex)){
		printErrorCode(700);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = Bellmanford(graph, option, s_vertex, e_vertex);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mFLOYD(char option)
{
	if(!load){
		printErrorCode(800);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = FLOYD(graph, option);
	std::cout.rdbuf(bak);
	return ok;
}

bool Manager::mCentrality() {
	if(!load){
		printErrorCode(900);
		return false;
	}
	std::streambuf* bak = std::cout.rdbuf(fout.rdbuf());
	bool ok = Centrality(graph);
	std::cout.rdbuf(bak);
	return ok;
}

void Manager::printErrorCode(int n)
{
	fout<<"========ERROR========"<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;}
