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
	vector<Edge> edgeTo;	//记录链接
	list<int> needVisited;	//记录下需要访问的点
	vector<int> dist;		//记录从s到索引的权重
	vector<int> demandSet;   //必须访问的点；

	vector<vector<Link>> demandCostMap;
};
