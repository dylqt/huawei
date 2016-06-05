#pragma once
#include "std.h"
#include "Edge.h"
class Graph {
public:
	Graph();
	Graph(char **topo, int edg_num, char *demand);
	void addEdge(Edge &edge) 
	{ 
		graph[edge.getFrom()].push_back(edge); 
		if (vertexNum < edge.getFrom())
		{
			vertexNum = edge.getFrom();
		}
	}
	int getVertexNum() { return vertexNum; }
	int getDemandNum() { return demandNum; }
	vector<Edge> getAdj(int s) { return graph[s]; }
	vector<int> getDemandVertexSet() { return demandVertexSet; }
	int getS() { return s; }
	int getT() { return t; }
	void toString();
private:
	int vertexNum;	//总的顶点个数
	int demandNum;	//必经点的个数
	vector<vector<Edge>> graph;	//图的连接表

	int s;
	int t;
	vector<int> demandVertexSet;	//必经的点集合
};
