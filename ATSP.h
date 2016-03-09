#pragma once
#include "Graph.h"
#include "std.h"

class Atsp {
public:
	Atsp(diGraph & G, int st, int tml, set<int> desires) :allMarked(G.getV()),
		s(st), t(tml), dV(desires), dVisite(desires.size()) {
		pi = 0; goWeight = 0;
	};
	void doAtsp(diGraph &graph, Edge v);
	void BFS(diGraph &graph, int v);
	void toString();
private:
	int s;
	int t;
	set<int> dV;		//input demand
	int dVisite;	//number of input demand
	
	vector<bool> allMarked;	//检测该点是否被访问到过
	vector<priority_queue<Edge>> branch;	//bfs的枝叶
	vector<Edge> goOver; //当前的路线
	int pi ;		//branch的第几层
	int goWeight ;	//当前的权重

	int weight = INT_MAX;		//检测到的路线的权重
	vector<Edge> result;	//输出的结果

};