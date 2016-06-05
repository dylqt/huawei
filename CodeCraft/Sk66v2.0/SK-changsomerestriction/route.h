#ifndef __ROUTE_H__
#define __ROUTE_H__

//#define PRINT_INPUT	//接口的行参输出
#define HANDLE_CYCLE	//是否处理环？当然
#define DEBUG
#define PRINT_PAHT	//是否能够打印路径
//#define GAME

#include <memory.h>
#include <limits.h>

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

void search_route(char *graph[5000], int edge_num, char *condition);

const int maxn = 600;	//最大节点数目

//边
class Edge {
public:
	int num, from, to, dist;
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

//	Path();
//	Path(const Path&);
	Path &operator=(const Path &rhs);
	Path operator+(const Path &rhs);
	void printPath(class Dijkstra &work);
	void handleCycle(const Path &rhs, Dijkstra work);	
};

/*class Paths {
public:
	int cost;
	vector<Path> paths;
	vector<int> specialNodes;	//这里可能出现重复的节点，要注意
};*/

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
	vector<int> G[maxn];//图G:下标表示节点，vector<int>中存的是，edges中的下标集合。
	bool done[maxn];	//节点是否已永久标号

public:
	int p[maxn];		//最短路中的上一条弧
	int d[maxn];		//s到各个点的距离
	vector<int> includingSet;
	int start, target;
	vector<Edge> edges;	//边的集合

	int nodeNums();
	void init(char *topo[5000], int edge_num, char *demand);
	void AddEdge(int index, int from, int to, int dist);
	void dijkstra(int s, const vector<int> rmNodes);
	void BuildPath(int vi, int vl, Path& path);
	void print_information(void);
	void print_p(void);
};


//SK66
class SK66 {
public:
	void init(void);
	void sk66(char *topo[5000], int edge_num, char *demand);
private:

};
//SK
class SK {
public:
	void init(void);
	void sk(char *topo[5000], int edge_num, char *demand);
private:

};

#endif
