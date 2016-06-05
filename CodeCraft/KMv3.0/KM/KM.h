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
	vector<int> demandV;	//���뾭���ĵ�
	map<int, int> MAP;	//�ڵ��Ӧindex
	vector<vector<int>> costMap;	//�����ڵ�֮���cost
	vector<vector<Link>> indexMap;	//�����ڵ�������ֵ����
	vector<bool> visited;	//��¼���ʳɹ��ĵ�
	vector<bool> visitX;	//x�ķ��ʼ�¼
	vector<bool> visitY;	//y�ķ��ʼ�¼
	vector<int> Lx;		//x���
	vector<int> Ly;		//y���
	vector<int> match;	//edgeTo
	int dealNum;	//��Ҫ�������Ŀ
	vector <int> slack; //����d

	vector<int> result; //
};

