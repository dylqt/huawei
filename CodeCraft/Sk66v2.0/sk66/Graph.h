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
	int vertexNum;	//�ܵĶ������
	int demandNum;	//�ؾ���ĸ���
	vector<vector<Edge>> graph;	//ͼ�����ӱ�

	int s;
	int t;
	vector<int> demandVertexSet;	//�ؾ��ĵ㼯��
};
