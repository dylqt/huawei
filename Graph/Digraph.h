#pragma once
#include "std.h"

class Digraph {
public:
	Digraph(ifstream &is);
	Digraph(int v);
	int V() { return vertex; }
	int E() { return edge; }
	void addEdge(int v, int w);
	Digraph reverse();
	void toString();
	vector<int> link(int v);
private:
	int vertex;
	int edge;
	vector<vector<int>> adj;
};

class DirectedCycle {
public:
	DirectedCycle(Digraph &G);
	bool hasCycle() { return cycle; }
	void dfs(Digraph &G,  int v);
	void cycleToString();
private:
	vector<bool> marked;
	vector<int> edgeTo;
	vector<bool> onStacked;
	bool cycle;
	vector<vector<int>> cycleStacks;
};