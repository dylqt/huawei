#pragma once

#include "Dijkstra.h"
class KM {
public:
	KM(Graph &graph) :s(graph.getS()), t(graph.getT()), dealNum(graph.getDemandNum() + 2)
		{ 
			demandV = graph.getDemandVertexSet();
			match = vector<int>(dealNum, -1);
			visited = vector<bool>(dealNum, false);
			indexMap = vector<vector<Link>>(dealNum, vector<Link>(dealNum));
			costMap = vector<vector<int>>(dealNum, vector<int>(dealNum, -INF));
			}
	void doKM(Dijstra &dij);
	void toString();
private:
	void initKM(Dijstra &dij);
	bool findPath(int u);

	int s;
	int t;
	vector<int> demandV;	//必须经过的点
	map<int, int> MAP;	//节点对应index
	vector<vector<int>> costMap;	//两个节点之间的cost
	vector<vector<Link>> indexMap;	//两个节点间的所有值的类
	vector<bool> visited;	//记录访问成功的点
	vector<bool> visitX;	//x的访问记录
	vector<bool> visitY;	//y的访问记录
	vector<int> Lx;		//x标杆
	vector<int> Ly;		//y标杆
	vector<int> match;	//edgeTo
	int dealNum;	//需要处理的数目
	vector <int> slack; //保存d

	vector<int> result; //
};

