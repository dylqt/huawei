#pragma once

#include "Graph.h"

class BreadthFirstSearch {
public:
	BreadthFirstSearch(Graph &G, int s);
	void BFS(Graph &G, int s);
	bool hasPath(int v) { return marked[v]; }
	vector<int> pathTo(int v);
	void printByString(int e);
private:
	vector<bool> marked;
	vector<int> edgeTo;
	int s;
};
