#pragma once

#include "Digraph.h"

class DirectedDFS {
public:
	DirectedDFS(Digraph &G, int n) : s(n), marked(G.V()), adj(G.V()) { DFS(G, n); }
	void DFS(Digraph &G, int s);
	void toString(Digraph &G);
	bool isReachable(int t) { return marked[t]; }
private:
	int s;
	vector<bool> marked;
	vector<int> adj;
};



