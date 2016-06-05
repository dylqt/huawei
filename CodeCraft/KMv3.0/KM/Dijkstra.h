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
	vector<Edge> edgeTo;	//记录链接
	list<int> needVisited;	//记录下需要访问的点
	vector<int> dist;		//记录从s到索引的权重
	vector<int> demandSet;   //必须访问的点；
	set<int> demandSetSearch;	//用于检索必须访问的点是否存在

	vector<vector<Link>> demandCostMap;
	vector<Link> sToVsMap;	//s to Vs map
	vector<Link> vsToTMap;	//Vs to t map
	
};
