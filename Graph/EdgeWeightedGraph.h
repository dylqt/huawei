#pragma once
#include "std.h"

class Edge {
public:
	Edge(int vl, int w, double wit) : v(vl), weight(wit) { this->w = w; }
	double getWeight() { return weight; }
	int V() { return v; }
	int W() { return w; }
	int otherV(int vertex)
	{
		if (vertex == this->v)
			return w;
		if (vertex == w)
			return v;
	}
private:
	int v;
	int w;
	double weight;
};


class EdgeWeightedGraph {
public:
	EdgeWeightedGraph(int v);
	EdgeWeightedGraph(ifstream &is);
	void toString();
	int V() { return vertex; }
	int E() { return edges; }
private:
	int vertex;
	int edges;
	vector<vector<Edge>> adj;
};
