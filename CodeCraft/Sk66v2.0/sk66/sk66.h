#pragma once

#include "Dijkstra.h"
#include "Graph.h"

class Sk66 {
public:
	Sk66(Graph &graph) :VsNum(graph.getDemandNum()),demandVertexSet(graph.getDemandVertexSet()),
		fn(graph.getVertexNum(), vector<Link>(graph.getVertexNum())) {};
	void doSk66(Dijstra &dij, int s, int t);
	void toString(int s);
	void toString(Link &link);

private:
	vector<vector<Link>> fn;
	int VsNum;
	vector<int> demandVertexSet;	//必经的点集合
	Link result;
};
