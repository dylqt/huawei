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

	double T0;		//起始温度
	double endT;	//结束温度
	int tmpTimes;	//每个温度点，数据不变的次数
	int L;		//每个温度点数据不变的最多运行次数
	double a;		//每次降温比例
	int q;		//运行q次没有改变时退出
	int costWay;	//当前的cost
	int noNewTimes;

	// 记忆最佳路径
	int costBest;
	vector<int> wayBest;

	int s;
	int t;
	vector<int> demandV;	//必经点集合
	int dealNum;

	map<int, int> MAP;
	vector<vector<int>> costMap;
	vector<vector<Link>> resultMap;
	vector<int> workRoad;
};