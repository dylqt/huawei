#ifndef __ROUTE_H__
#define __ROUTE_H__

#define HANDLE_CYCLE	//是否处理环？当然
//#define DEBUG
//#define PRINT_PAHT	//是否能够打印路径

#include <memory.h>
#include <limits.h>
#include <time.h>

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <stack>
#include <bitset>
#include "lp_lib.h"
using namespace std;

#define MAX_INT 96000
#define MAX_NODES 600

void initGraph(char *topo[5000], int edge_num, char *demand); //¹¹½¨Í¼
void toString();
bool isAns(vector<int> &tour);
void lpSolver();
bool findsubtour(vector<vector<int> > &sol, vector<int> &tour);
int JudegCycle(bool visited[],const vector<int> &allnode);
int isAnsRight(vector<vector<int> > &curCycle);
void addConstr(vector<vector<int> > &curCycle);



void search_route(char *graph[5000], int edge_num, char *condition);

const int maxn = 600;	//最大节点数目

//边
class Edge {
public:
	int num, from, to, dist;

	Edge();
	Edge(int index, int u, int v, int d) : num(index), from(u), to(v), dist(d) {}
};

//路径
class Path {
public:
	int cost;
	int start, target;	//路径的起点和终点
	vector<int> edges;	//从离终点最近的边开始放入
	vector<int> specialNodes;	//此时不会出现重复的节点，因为这里是最短距离
	vector<int> freeNodes;	//路径中要保证不能有环，构造路径时要保证（freeNodes不包括起点和终点）

	Path &operator=(const Path &rhs);
	Path operator+(const Path &rhs);
	bool operator==(const Path &rhs);
	void printPath(class Dijkstra &work);
	void handleCycle(Path &rhs, Dijkstra &work, bool judgeCycle);
	bool insertSpNodes(Dijkstra &work); //将此条路径缺少的特殊节点插入	
	bool insertSpNodesbsp(Dijkstra &work); //将此条路径缺少的特殊节点插入	
	bool pathIsAns(Dijkstra &work);  //此条路径是答案吗
};

//Dijkstra
class HeapNode {
public:
	int d, u;	//d:节点u的最短路径估计 u:节点号
	HeapNode(int a, int b) : d(a), u(b) {}
	bool operator < (const HeapNode& rhs) const {
		return d > rhs.d;
	}
};

class Dijkstra {
private:
	int n, m;			//n:节点数目 m:边的数目
	bool done[maxn];	//节点是否已永久标号

public:
	int p[maxn];		//最短路中的上一条弧
	int d[maxn];		//s到各个点的距离
	vector<int> includingSet;
	bitset<maxn> isIncludingSet;
	int start, target;
	vector<Edge> edges;		//边的集合
	vector<int> G[maxn];//图G:下标表示节点，vector<int>中存的是，edges中的下标集合。-1表示此边已经被排除·

	int nodeNums();
	void init(char *topo[5000], int edge_num, char *demand);
	void AddEdge(int index, int from, int to, int dist);
	void dijkstra(int s, const vector<int> rmNodes);
	void BuildPath(int vi, int vl, Path& path);
	void print_information(void);
	void print_p(void);
	void randFindPath(clock_t start, clock_t end);
};

class DFS {
private:
	int G[2][maxn][maxn]; //0:权值 1:边的编号 无穷表示边不存在
	int start, target; //起点 终点
	vector<int> includingSet; //特殊节点集合
	bitset<maxn> isIncludingSet;
	int allNodesNum; //所有节点个数
	vector<int> bestAns; //最优解, 边向反
	vector<bool> done; //节点是否已经经过
	vector<Edge> G_list[maxn];
	
public:
	int bestCost, breakCost;
	void init(char *topo[5000], int edge_num, char *demand, Dijkstra &work);
	//void dfs(Dijkstra &work);
	void dfs();
	bool spNodesDegree(bitset<maxn> &allNodes, const vector<int> &spNodes, vector<int> &newSpNodes, bitset<maxn> &isdone);
};
#endif
