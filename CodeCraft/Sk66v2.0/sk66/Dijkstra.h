#pragma once

#include "Graph.h"
#include "Link.h"

class Dijstra {
public:
	Dijstra(Graph &graph) : 
		edgeTo(vector<Edge>(graph.getVertexNum()))
		,dist(vector<int>(graph.getVertexNum()) )
		,demandSet(graph.getDemandVertexSet())
		,demandCostMap(graph.getVertexNum(), vector<Link>(graph.getVertexNum()))
		{};
	
	void dijstraSK66(Graph &graph, int s, int t);
	void dijstraSK(Graph &graph, int s);

	void addLink(Graph &graph, int s, int t);
	void toString(int s, int t);

	vector<vector<Link>> getDemandCostMap() { return demandCostMap; };
private:
	void doDijstra(Graph &graph, int s);
	vector<Edge> edgeTo;	//��¼����
	list<int> needVisited;	//��¼����Ҫ���ʵĵ�
	vector<int> dist;		//��¼��s��������Ȩ��
	vector<int> demandSet;   //������ʵĵ㣻

	vector<vector<Link>> demandCostMap;
};
