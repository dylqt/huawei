#pragma once
#include "std.h"


class Edge {
public:
	
	Edge(int from, int to, int w) : fromV(from), toV(to), weight(w) {};	
	int getFrom() { return fromV; };
	int getTo() { return toV; };
	int getWeight() const{ return weight; };
private:
	int fromV;
	int toV;
	int weight;
};

class diGraph {
public:
	diGraph(ifstream &is);
	diGraph(int size) :graph(size) {};
	void addEdge(int from, int to, int weight);
	void toString();
	int getV() { return vertex; };
	vector<Edge> getAdj(int v) { return graph[v]; };
private:
	vector<vector<Edge>> graph;
	int vertex;
	int edges;
};
bool operator < (const Edge &lhs, const Edge &rhs);
