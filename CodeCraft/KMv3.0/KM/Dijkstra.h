#pragma once

#include "Graph.h"
#include "Link.h"

class Dijstra {
public:
	Dijstra(Graph &graph) :
		edgeTo(vector<Edge>(graph.getVertexNum()))
		, dist(vector<int>(graph.getVertexNum()))
		, demandSet(graph.getDemandVertexSet())
		, demandCostMap(graph.getVertexNum(), vector<Link>(graph.getVertexNum()))
		, demandSetSearch(demandSet.begin(), demandSet.end())
		{};
	void dijstraTwo(Graph &graph, int s, int t);

	bool isDemandV(int s) { return demandSetSearch.find(s) != demandSetSearch.end(); }
	void addLink(Graph &graph, int s, int t);
	void toString(int s, int t);
	void toString();

	vector<vector<Link>> getDemandCostMap() { return demandCostMap; };
private:
	void doDijstraTwo(Graph &graph, int s);
	void doDijstra(Graph &graph, int s);
	vector<Edge> edgeTo;	//��¼����
	list<int> needVisited;	//��¼����Ҫ���ʵĵ�
	vector<int> dist;		//��¼��s��������Ȩ��
	vector<int> demandSet;   //������ʵĵ㣻
	set<int> demandSetSearch;	//���ڼ���������ʵĵ��Ƿ����

	vector<vector<Link>> demandCostMap;
	vector<Link> sToVsMap;	//s to Vs map
	vector<Link> vsToTMap;	//Vs to t map
	
};
