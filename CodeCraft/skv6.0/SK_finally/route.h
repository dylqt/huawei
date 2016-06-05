#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <vector>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

#define MAX_INT 96000
#define MAX_NODES 600
void search_route(char *topo[5000], int edge_num, char *demand);

/*��*/
class Edge{
public:
	int id;
	int from;
	int to;
	int weight;

	Edge(int i, int f, int t, int c) :id(i), from(f), to(t), weight(c){};
	Edge(){};
};

/*·��*/
class Path
{
public:
	class Graph;
	int origin;					//·�����
	int target;					//·���յ�
	int cost;

	vector<int> edges;			//·���о����ߵļ���
	vector<int> nodes;			//·���о�������ļ���
	vector<int> specialNodes;	//·���бؾ��ڵ�ļ���
	vector<int> freeNodes;		//·�������ɽڵ�ļ���

	Path &operator=(const Path &rhs);	//���������
	Path operator+(const Path &rhs);

	Path() : cost(MAX_INT) {};
	void printPath();				//��ӡ��ǰ·��
//	bool pathIsAns(Graph &g);	//��֤·���Ƿ���ȷ
};

/*����Dij�����ȼ�����*/
class PriorityNode{
public:
	int vIndex;		//������
	int curDist;	//2������ĵ�ǰ����

	PriorityNode(int v, int c) : vIndex(v), curDist(c){};	

	bool operator < (const PriorityNode& rhs) const {		//���ȼ��������������
		return curDist > rhs.curDist;
	}
};

/*������*/
class Graph{
public:
	/* Graph */
	int verNum;					//���ж������Ŀ
	int specialNum;				//�ؾ��ڵ����Ŀ
	int start;					//���
	int terminial;				//�յ�
	vector<int> specialNodes;	//�ؾ��㼯��
	vector<vector<Edge> > edgeTable;		//�洢ͼ���ڽӱ�

	void initGraph(char *topo[5000], int edge_num, char *demand); //����ͼ
	void toString();
	/* Dij */
	vector<int> dist;			//ÿ��dij�б���2��ľ���
	vector<bool> visited;		//dij�б�ǽڵ��Ƿ��ѷ���
	vector<int> rmNodes;		//dij���ܷ��ʵĶ���
	vector<Edge> edgeTo;		//dij�����ڱ��������ı�
	vector<vector<Path> > D;		//sk����ר��

	void Dijstra(int s, vector<int> &rmNodes);		//dij
	Path bulidPath(int s, int t);						//���2����·��

	/* Sk */
	vector<Path> result;				//���ܽ��·��
	vector<vector<vector<Path> > > F;		//SK�������

	//Path ***F;
	vector<vector<int> > cost;			//����������cost

	void initSK();
	void doSK();
	Path handleCycle(Path &lhsD, Path &rhsF);	//����
	bool getFinallySK();						//SK�õ�������ɹ�������ļ������ɹ���ʼǿ��
	void insertNoWay(int iterTimes);			//����һ�ε����󣬼��һ��fn���鿴�Ƿ񻹴���·�������ȫ��û��·���������ǿ��
	void insertKeepN(int iterTimes);			//����һ�ε����󣬼��fn�� �����·���ıؾ�����ĿС�ڵ�������������в���
	bool insertNodeAll(Path &path);				//ǿ��
	Path insertNodeSp(Path &path);
};

bool pathIsAns(Path &path, Graph &g);

#endif
