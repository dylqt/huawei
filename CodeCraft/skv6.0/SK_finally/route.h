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

/*边*/
class Edge{
public:
	int id;
	int from;
	int to;
	int weight;

	Edge(int i, int f, int t, int c) :id(i), from(f), to(t), weight(c){};
	Edge(){};
};

/*路径*/
class Path
{
public:
	class Graph;
	int origin;					//路径起点
	int target;					//路径终点
	int cost;

	vector<int> edges;			//路径中经过边的集合
	vector<int> nodes;			//路径中经过顶点的集合
	vector<int> specialNodes;	//路径中必经节点的集合
	vector<int> freeNodes;		//路径中自由节点的集合

	Path &operator=(const Path &rhs);	//重载运算符
	Path operator+(const Path &rhs);

	Path() : cost(MAX_INT) {};
	void printPath();				//打印当前路径
//	bool pathIsAns(Graph &g);	//验证路径是否正确
};

/*用于Dij中优先级队列*/
class PriorityNode{
public:
	int vIndex;		//顶点编号
	int curDist;	//2个顶点的当前距离

	PriorityNode(int v, int c) : vIndex(v), curDist(c){};	

	bool operator < (const PriorityNode& rhs) const {		//优先级队列运算符重载
		return curDist > rhs.curDist;
	}
};

/*操作类*/
class Graph{
public:
	/* Graph */
	int verNum;					//所有顶点的数目
	int specialNum;				//必经节点的数目
	int start;					//起点
	int terminial;				//终点
	vector<int> specialNodes;	//必经点集合
	vector<vector<Edge> > edgeTable;		//存储图的邻接表

	void initGraph(char *topo[5000], int edge_num, char *demand); //构建图
	void toString();
	/* Dij */
	vector<int> dist;			//每次dij中保存2点的距离
	vector<bool> visited;		//dij中标记节点是否已访问
	vector<int> rmNodes;		//dij不能访问的顶点
	vector<Edge> edgeTo;		//dij中用于保存相连的边
	vector<vector<Path> > D;		//sk迭代专用

	void Dijstra(int s, vector<int> &rmNodes);		//dij
	Path bulidPath(int s, int t);						//添加2点间的路径

	/* Sk */
	vector<Path> result;				//可能结果路径
	vector<vector<vector<Path> > > F;		//SK迭代结果

	//Path ***F;
	vector<vector<int> > cost;			//保存两点间的cost

	void initSK();
	void doSK();
	Path handleCycle(Path &lhsD, Path &rhsF);	//处理环
	bool getFinallySK();						//SK得到结果，成功输出到文件，不成功开始强插
	void insertNoWay(int iterTimes);			//做完一次迭代后，检查一次fn，查看是否还存在路径，如果全部没有路径，则进行强插
	void insertKeepN(int iterTimes);			//做完一次迭代后，检查fn， 如果有路径的必经点数目小于迭代次数，则进行插入
	bool insertNodeAll(Path &path);				//强插
	Path insertNodeSp(Path &path);
};

bool pathIsAns(Path &path, Graph &g);

#endif
