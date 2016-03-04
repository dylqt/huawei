#pragma once
#include "std.h"

class Graph {
public:
	Graph(ifstream &is);
	int V() { return vertex; }
	int E() { return edge; }
	void printByString();
	void addEdge(int from, int to);
	vector<int> link(int s) { return adj[s]; }
private:
	int vertex;
	int edge;
	vector<vector<int>> adj;
};

// connect count
class CC {
public:
	CC(Graph &G);
	bool connected(int v, int w) { return id[v] == id[w]; }
	int Count() { return count; }
	int Id(int v) { return id[v]; }
	void DFS(Graph &G, int s);
private:
	vector<bool> marked;
	int count;
	vector<int> id;
};

// Cycle
class Cycle
{
public:	
	Cycle(Graph &G);
	void DFS(Graph &G, int v, int w);
	int isCycle() { return hasCycle; }
private:
	vector<bool> marked;
	bool hasCycle;
};

// TwoColor
class TwoColor {
public:
	TwoColor(Graph &G) : marked(G.V()), color(G.V()), isTwoColorable(true) {
		for (int i = 0; i < G.V(); i++) {
			if (!marked[i])
				DFS(G, i);
		}
	}
	void DFS(Graph &G, int s);
	bool isBipartite() { return isTwoColorable; }
private:
	vector<bool> marked;
	vector <bool> color;
	bool isTwoColorable;
};