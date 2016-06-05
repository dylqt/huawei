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
	
	vector<bool> allMarked;	//���õ��Ƿ񱻷��ʵ���
	vector<priority_queue<Edge>> branch;	//bfs��֦Ҷ
	vector<Edge> goOver; //��ǰ��·��
	int pi ;		//branch�ĵڼ���
	int goWeight ;	//��ǰ��Ȩ��

	int weight = INT_MAX;		//��⵽��·�ߵ�Ȩ��
	vector<Edge> result;	//����Ľ��

};