#pragma once

#include "Dijkstra.h"

class Tuihuo {
public:
	Tuihuo(Graph &graph) : s(graph.getS()), t(graph.getT()), demandV(graph.getDemandVertexSet()) {};
	Tuihuo(vector<int> input) : workRoad(input) {};
	void initTuohuo(Dijstra &dij);
	void doTuihuo(Dijstra &dij);
	void toString();
	
	void check();

private:
	void initMAP();

	void tuihuoChangeTwo(int Ti);
	void changeTwo(int ch1, int ch2);
	int getNewCostBychange(int ch1, int ch2);
	
	void tuihuoReverse(int Ti);
	int getNewCostByReverse(int ch1, int ch2);
	void reverseTwo(int ch1, int ch2);

	double T0;		//��ʼ�¶�
	double endT;	//�����¶�
	int tmpTimes;	//ÿ���¶ȵ㣬���ݲ���Ĵ���
	int L;		//ÿ���¶ȵ����ݲ����������д���
	double a;		//ÿ�ν��±���
	int q;		//����q��û�иı�ʱ�˳�
	int costWay;	//��ǰ��cost
	int noNewTimes;

	// �������·��
	int costBest;
	vector<int> wayBest;

	int s;
	int t;
	vector<int> demandV;	//�ؾ��㼯��
	int dealNum;

	map<int, int> MAP;
	vector<vector<int>> costMap;
	vector<vector<Link>> resultMap;
	vector<int> workRoad;
};