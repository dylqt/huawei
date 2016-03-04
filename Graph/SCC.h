#pragma once
#include "Digraph.h"

class SCC {
public:
	SCC(Digraph &G);
	bool stronglyConnected(int v, int w) { return id[v] == id[w]; }
	int getCount() { return count; }
	int getId(int v) { id[v]; }
	void dfs(Digraph &G, int v, vector<int> &);
	void conToString();
private:
	vector<int> id;
	vector<bool> marked;
	int count;
	vector<vector<int>> connects;
};
