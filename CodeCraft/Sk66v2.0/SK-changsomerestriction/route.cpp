
#include "route.h"
#include "lib_record.h"
#include <stdio.h>

using namespace std;

Path &Path::operator=(const Path &rhs)
{
	cost = rhs.cost;
	start = rhs.start;
	target = rhs.target;
	edges = rhs.edges;
	specialNodes = rhs.specialNodes;
	freeNodes = rhs.freeNodes;

	return *this;
}

Path Path::operator+(const Path &rhs)
{
	Path sum = *this;
	sum.cost = this->cost + rhs.cost;

	if(sum.target == rhs.start) {
		sum.target = rhs.target;
		vector<int> vec(rhs.edges);
		for(unsigned int i = 0; i < sum.edges.size(); i++)
			vec.push_back(sum.edges[i]);
		sum.edges = vec;
	}
	else if(rhs.target == sum.start) {
		sum.start = rhs.start;
		for(unsigned int i = 0; i < rhs.edges.size(); i++)
			sum.edges.push_back(rhs.edges[i]);
	}
		
	for(unsigned int i = 0; i < rhs.specialNodes.size(); i++)
		if(find(sum.specialNodes.begin(), sum.specialNodes.end(), rhs.specialNodes[i]) == sum.specialNodes.end())
			sum.specialNodes.push_back(rhs.specialNodes[i]);

	for(unsigned int i = 0; i < rhs.freeNodes.size(); i++)
		if(find(sum.freeNodes.begin(), sum.freeNodes.end(), rhs.freeNodes[i]) == sum.freeNodes.end())
			sum.freeNodes.push_back(rhs.freeNodes[i]);

	return sum;
}
/*
Path::Path()
{
	cost = start = target = -1;
	edges.clear();
}

Path::Path(const Path &orig)
{
	cost = orig.cost;
	start = orig.start; target = orig.target;
	edges = orig.edges;
	specialNodes = orig.specialNodes;
}
*/
void Path::printPath(class Dijkstra &work)
{
#ifdef PRINT_PAHT
	cout << "Path:" << start << "->" << target << "cost:" << cost << ",edges(";
	for(int i = (int)edges.size() - 1; i >= 0; i--) {
		if(i != 0) cout << edges[i] << '|';
		else cout << edges[i] << ')';
	}
	cout << "\nspNodes(";
	for(int i = 0; i < (int)specialNodes.size(); i++){
		if(i != (int)specialNodes.size() - 1) cout << specialNodes[i] << ",";
		else cout << specialNodes[i] << ")";
	}
	cout << "NodesNum:" << specialNodes.size() << endl;

	set<int> nodes;
	cout << "Nodes:" << start << "->";
	for(int i = (int)edges.size() - 1; i >= 0; i--) {
		if(nodes.count(work.edges[edges[i]].to) == 0) nodes.insert(work.edges[edges[i]].to);
		else cout << "(cycle)";	//节点重复出现了
		if(i != 0) cout << work.edges[edges[i]].to << "->";
		else cout << work.edges[edges[i]].to << endl;
	}
	cout << endl;
#endif
}

void Path::handleCycle(const Path &rhs, Dijkstra work)
{
#ifdef HANDLE_CYCLE
	vector<int> repeatNodes;
	for(int i = 0; i < (int)rhs.freeNodes.size(); i++)	//freeNodes不包括起点和终点
		repeatNodes.push_back(rhs.freeNodes[i]);

	for(int i = 0; i < (int)rhs.specialNodes.size(); i++)	//
		if(this->target == rhs.start && rhs.specialNodes[i] != rhs.start)
			repeatNodes.push_back(rhs.specialNodes[i]);
		else if(this->start == rhs.target && rhs.specialNodes[i] != rhs.target)
			repeatNodes.push_back(rhs.specialNodes[i]);

	if(this->target != work.target) repeatNodes.push_back(work.target);
	if(this->start != work.start) repeatNodes.push_back(work.start);
	//cout << "repeatNodes:" << endl;
	//for(int i = 0; i < (int)repeatNodes.size(); i++) cout << repeatNodes[i] << ' ';
	//cout << endl;

	if(repeatNodes.size() == 0) return;
	else {
		work.dijkstra(this->start, repeatNodes);
		work.BuildPath(this->start, this->target, *this);
	}
#endif
}

void Dijkstra::init(char *topo[5000], int edge_num, char *demand) {	//用节点数目初始化
	m = edge_num;
	for (int i = 0; i < maxn; i++) G[i].clear();
	edges.clear();

	int index, from, to, dist;
	char tmp;
	for(int i = 0; i < edge_num; i++){
		stringstream ss;
		ss << topo[i];
	    	ss >> index >> tmp >> from >> tmp >> to >> tmp >> dist;
		this->AddEdge(index, from, to, dist);	
	}

	stringstream ss;
	ss << demand;
	ss >> this->start >> tmp >> this->target >> tmp;
	do{
		ss >> from;
		includingSet.push_back(from);
	}while(ss >> tmp);
}

void Dijkstra::AddEdge(int index, int from, int to, int dist) {	//添加一条边
	edges.push_back(Edge(index, from, to, dist));	//此处假设边的读入顺序和index相同。否则需要事先设计edges的大小。
	//m = edges.size();
	int tmp = max(from, to);
	this->n = max(this->n, tmp + 1);
	G[from].push_back(index);
}

void Dijkstra::dijkstra(int s, const vector<int> rmNodes) {	//计算起点到各个点的最短路径
	
	for (int i = 0; i < n; i++) d[i] = INT_MAX;	//初始化d数组
	d[s] = 0;

	memset(done, 0, sizeof(done));	//初始化done数组
	for(int i = 0; i < (int)rmNodes.size(); i++)	//把移除的节点标记为已经完成
		done[rmNodes[i]] = true;
		
	memset(p, -1, sizeof(p));	//-1表示没有这条边
	priority_queue<HeapNode> Q;
	Q.push(HeapNode(0, s));	//把起点加入队列

	while (!Q.empty()) {
		HeapNode x = Q.top(); Q.pop();
		int u = x.u;
		if (done[u]) continue;
		done[u] = true;
		for (unsigned int i = 0; i < G[u].size(); i++) {	//	遍历节点u的所有边
			Edge& e = edges[G[u][i]];
			if (d[e.to] > d[u] + e.dist) {
				d[e.to] = d[u] + e.dist;
				p[e.to] = G[u][i];
				Q.push(HeapNode(d[e.to], e.to));
			}
		}
	}
}

void Dijkstra::BuildPath(int vi, int vl, Path& path)
{
	path.cost = this->d[vl];
	path.start = vi;
	path.target = vl;
	path.edges.clear();
	path.specialNodes.clear();
	path.freeNodes.clear();

	for(int i = vl; ; ){	//从终点开始
		if(find(this->includingSet.begin(), this->includingSet.end(), i) != this->includingSet.end())
			path.specialNodes.push_back(i);
		else if(i != vi && i != vl) path.freeNodes.push_back(i);

		if(this->p[i] == -1) {
			path.cost = -1;
			break;	//不存在这条路径
		}
		path.edges.push_back(this->p[i]);	//找到一条边
		if((i = this->edges[this->p[i]].from) != vi) continue;
		else{
			if(find(this->includingSet.begin(), this->includingSet.end(), i) != this->includingSet.end())
				path.specialNodes.push_back(i);
			break;
		}
	}
}

int Dijkstra::nodeNums(void)
{
	return n;
}

void Dijkstra::print_p(void)
{
	for(int i = 0; i < this->n; i++)
		cout << p[i] << ' ';
	cout << endl;
}

void Dijkstra::print_information(void)
{
	cout << "start:" << start << " target:" << target << endl;
	for(unsigned int i = 0; i < includingSet.size(); i++)
		cout << includingSet[i] << '|';
	cout << endl;

	cout << "最短路径中，每个节点的上一条边的编号：" << endl; 
	for(int i = 0; i < n; i++)
		cout << this->p[i] << '|';
	cout << endl;

	cout << "节点总数n:" << n << ",边总数m:" << m << endl;
}

void SK66::sk66(char *topo[5000], int edge_num, char *demand)
{
	Dijkstra work;
	work.init(topo, edge_num, demand);


	//这个D很大，需要缩小，600点肯定段错误
	const int allNodesNum = work.nodeNums();
	Path D[allNodesNum][allNodesNum];	//i:表示起点编号 j:表示终点编号;两点之间的最短路径矩阵

	const int spNodesNum = work.includingSet.size();
	Path f[spNodesNum - 1][spNodesNum];	//路径经过哪些边的矩阵。
	Path f0;
	//初始化矩阵
	memset(D, 0, sizeof(D));
	memset(f, 0, sizeof(f));

	//Dijkstra构造两两之间的最短路径，若路径不存在其权重为-1
	for(int i = 0; i < allNodesNum; i++) {
		if(find(work.includingSet.begin(), work.includingSet.end(), i) != work.includingSet.end() || i == work.start){	
			vector<int> rmNodes;
			rmNodes.clear();	
			work.dijkstra(i, rmNodes);
			for(int j = 0; j < allNodesNum; j++){
				if(i == j) continue;
				if(find(work.includingSet.begin(), work.includingSet.end(), j) != work.includingSet.end() || j == work.target){
					work.BuildPath(i, j, D[i][j]);
					D[i][j].printPath(work);
				}
			}
		//	work.print_information();
		}
	}

	//SK66迭代
	int vi, vl, addvl, addj;	//vi:起点 vl:终点 
	for(int i = 0; i < spNodesNum; i++) {	//i:只表示需要循环的次数
		cout << "i=" << i << endl;
		if(i == 0) {
			for(int j = 0; j < (int)work.includingSet.size(); j++) {	//j:遍历起点
				vi = work.includingSet[j];
				int min = INT_MAX;
				for(int k = 0; k < (int)work.includingSet.size(); k++) {	//k:遍历终点
					if(j == k) continue;
					vl = work.includingSet[k];
					if(D[vi][vl].cost > 0 && D[vi][vl].cost + D[vl][work.target].cost < min) {
						min = D[vi][vl].cost + D[vl][work.target].cost;
						addvl = vl; //要添加的中间节点是哪一个。
					}
				}	
				f[i][j] = D[vi][addvl] + D[addvl][work.target];
				if((int)f[i][j].specialNodes.size() - 1 < i + 1) f[i][j].cost = -1; //the path is not admissible
			}
		}
		else if(i == spNodesNum - 1) {
				int min = INT_MAX;
				addvl = addj = -1;
				for(int k = 0; k < (int)work.includingSet.size(); k++) {	//k:遍历终点
					vl = work.includingSet[k];
					if(D[work.start][vl].cost > 0 && f[i - 1][k].cost > 0 && D[work.start][vl].cost + f[i - 1][k].cost < min) {
						min = D[work.start][vl].cost + f[i - 1][k].cost;
						addvl = vl; //要添加的中间节点是哪一个。
						addj = k;
					}
				}	
				if(addvl != -1) {
					f0 = D[work.start][addvl] + f[i - 1][addj];
					if((int)f0.specialNodes.size() < i + 1) f0.cost = -1; //the path is not admissible				
				}
				else cout << "NA" << endl;
				
		}
		else {
			for(int j = 0; j < (int)work.includingSet.size(); j++) {	//j:遍历起点
				vi = work.includingSet[j];
				int min = INT_MAX;
				addvl = addj = -1;
				for(int k = 0; k < (int)work.includingSet.size(); k++) {	//k:遍历终点
					if(j == k) continue;
					vl = work.includingSet[k];
					if(D[vi][vl].cost > 0 && f[i - 1][k].cost > 0 && D[vi][vl].cost + f[i - 1][k].cost < min &&\
		find(f[i - 1][k].specialNodes.begin(), f[i - 1][k].specialNodes.end(), vi) != f[i - 1][k].specialNodes.end()) {
						min = D[vi][vl].cost + f[i - 1][k].cost;
						addvl = vl; //要添加的中间节点是哪一个。
						addj = k;
					}
				}	
				if(addvl != -1) f[i][j] = D[vi][addvl] + f[i - 1][addj];
				else f[i][j] = f[i - 1][j];
				if((int)f[i][j].specialNodes.size() - 1 < i + 1) f[i][j].cost = -1; //the path is not admissible
			}
		}	
		//打印迭代过程
		for(int l = 0; i != spNodesNum - 1 && l < spNodesNum; l++)
			f[i][l].printPath(work);
		if(i == spNodesNum - 1) f0.printPath(work);
	}
}

void SK::sk(char *topo[5000], int edge_num, char *demand)
{
}


/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
#ifdef PRINT_INPUT
	cout << "接口输入实参：" << endl;
	cout << "edge_num:" << edge_num << endl;
	cout << "topo:" << endl;
	for (int i = 0; i < edge_num; i++)
		cout << topo[i];
	cout << endl;
	cout << "demand:" << endl;	
	cout << demand << endl;
#endif

	//SK66 work;
	//work.sk66(topo, edge_num, demand);	case1：在类内调用使用19ms，不用类使用4ms

	Dijkstra work;
	work.init(topo, edge_num, demand);
	
	//const int allNodesNum = work.nodeNums();
	const int spNodesNum = work.includingSet.size();

	//这个D只能包好必须经过的节点和起点and终点。这里面的点的下标和includingSet中的下标必须一致，起点放最后一行，终点放最后一列。
	Path D[spNodesNum + 1][spNodesNum + 1];	//i:表示起点编号 j:表示终点编号;两点之间的最短路径矩阵,
	Path f[spNodesNum][spNodesNum][spNodesNum];	//路径经过哪些边的矩阵。
	Path f0[spNodesNum];

	//初始化矩阵
	memset(D, 0, sizeof(D));
	memset(f, 0, sizeof(f));
	
	//Dijkstra构造两两之间的最短路径，若路径不存在其权重为-1
#ifdef DEBUG
	cout << "Dijkstra:" << endl;
#endif
	vector<int> rmNodes;
	for(int i = 0; i < spNodesNum; i++) {	//在special nodes中遍历起点
		rmNodes.clear();
		rmNodes.push_back(work.start);
		rmNodes.push_back(work.target);	
		work.dijkstra(work.includingSet[i], rmNodes);	//移除起点和终点进行计算
		for(int j = 0; j < spNodesNum; j++){
			if(i == j) continue;
			work.BuildPath(work.includingSet[i], work.includingSet[j], D[i][j]);
			D[i][j].printPath(work);
		}

		rmNodes.clear();
		rmNodes.push_back(work.start);	//只移除起点进行计算
		work.dijkstra(work.includingSet[i], rmNodes);	

		work.BuildPath(work.includingSet[i], work.target, D[i][spNodesNum]);
		D[i][spNodesNum].printPath(work);
	}


	rmNodes.clear();
	rmNodes.push_back(work.target);	//只移除终点进行计算
	work.dijkstra(work.start, rmNodes);	
	for(int j = 0; j < spNodesNum; j++){
		work.BuildPath(work.start, work.includingSet[j], D[spNodesNum][j]);
		D[spNodesNum][j].printPath(work);
	}

	//SK迭代
	//int vi, vl;	//vi:起点 vl:终点 
	for(int i = 0; i < spNodesNum; i++) {	//i:只表示需要循环的次数
#ifdef DEBUG
		cout << "SK迭代i=" << i << endl;
#endif
		if(i == 0) {
			for(int j = 0; j < (int)work.includingSet.size(); j++) {	//j:遍历起点--vi
				//vi = work.includingSet[j];
				for(int k = 0; k < (int)work.includingSet.size(); k++) {	//k:遍历终点--vl
					if(j == k) continue;
					//vl = work.includingSet[k];
					if(D[j][k].cost > 0 && D[k][spNodesNum].cost > 0) {
						Path tmp = D[j][k];
						D[j][k].handleCycle(D[k][spNodesNum], work);
						if(D[j][k].cost != -1) {
							f[i][j][k] = D[j][k] + D[k][spNodesNum];
						}
						else {
							D[j][k] = tmp;
							D[k][spNodesNum].handleCycle(D[j][k], work);					
							if(D[k][spNodesNum].cost != -1) {
								f[i][j][k] = D[j][k] + D[k][spNodesNum];
							}
							else f[i][j][k].cost = -1;		
						}
						//else f[i][j][k].cost = -1;
					}
					else f[i][j][k].cost = -1;	//这次迭代不存在这条路径

					//SK66中每步都要判断，SK中应该也要吧,(2 - 1 < 0 + 1 一定不满足)
					if(f[i][j][k].cost > 0 && \
					(int)f[i][j][k].specialNodes.size() - 1 < i + 1) f[i][j][k].cost = -1; //the path is not admissible
				}	
			}
		}
		else {
			for(int j = 0; j < (int)work.includingSet.size(); j++) {	//j:遍历起点
				//vi = work.includingSet[j];
				for(int k = 0; k < (int)work.includingSet.size(); k++) {	//k:遍历终点
					if(j == k) continue;
					//vl = work.includingSet[k];

					int min = INT_MAX;
					int addk, addl;
					addk = addl = -1;
					if(D[j][k].cost > 0) {
						for(int l = 0; l < (int)work.includingSet.size(); l++) {
							if(k == l || j == l) continue;	//跳过起点和终点相同，或者出现环。
							if(f[i - 1][k][l].cost > 0 &&\
							D[j][k].cost + f[i - 1][k][l].cost < min) {
								min = D[j][k].cost + f[i - 1][k][l].cost;
								addk = k;
								addl = l;
							}	
						}
					}
					else {
						f[i][j][k].cost = -1;
						continue;
					}
				
					//若addk = -1,则D[j][k].cost < 0 || f[i - 1][k][l].cost < 0.

					//此处3个条件还要研究论文
					if(addk != -1) {	//找到一条路径（若addk==-1则之前也不可能有这条路径）
						D[j][k].handleCycle(f[i - 1][addk][addl], work);
						if(D[j][k].cost != -1) {
							f[i][j][k] = D[j][k] + f[i - 1][addk][addl];
						}
						else f[i][j][k].cost = -1;
						//from SK66
						//if((int)f[i][j][k].specialNodes.size() - 1 < i + 1) f[i][j][k].cost = -1; //the path is not admissible
						if((int)f[i][j][k].specialNodes.size() < i + 1) f[i][j][k].cost = -1; 						
						//SK--(2)--(3)
						//if(f[i][j][k].cost < 0 && (int)f[i - 1][j][k].specialNodes.size() >= spNodesNum - 1)
						if(f[i][j][k].cost < 0 && (int)f[i - 1][j][k].specialNodes.size() >= i + 1)
							f[i][j][k] = f[i - 1][j][k];
						if((int)f[i][j][k].specialNodes.size() >= i + 1 &&\
					           (int)f[i - 1][j][k].specialNodes.size() == (int)f[i][j][k].specialNodes.size() &&\
						   f[i - 1][j][k].cost < f[i][j][k].cost)
							f[i][j][k] = f[i - 1][j][k];
					}
					else if((int)f[i - 1][j][k].specialNodes.size() >= i + 1) f[i][j][k] = f[i - 1][j][k];
					else f[i][j][k].cost = -1;	//此条路径终结
					
					
				}	
			}
		}	
		//打印迭代过程
		for(int j = 0; j < spNodesNum; j++) {		
			for(int k = 0; k < spNodesNum; k++) {
				if(j != k && f[i][j][k].cost > 0) f[i][j][k].printPath(work);
			}
		}
		//if(i == spNodesNum - 1) f0.printPath(work);
	}

	//vi = work.start;
	for(int i = 0; i < (int)work.includingSet.size(); i++) {	//遍历D中的终点，也就是f中的起点
		//vl = work.includingSet[i];
		int min = INT_MAX;
		int addi, addj;
		addi = addj = -1;
		for(int j = 0; j < (int)work.includingSet.size(); j++) {	//遍历f中的终点
			if(i == j) continue;
			if(D[spNodesNum][i].cost > 0 && f[spNodesNum - 1][i][j].cost > 0 && D[spNodesNum][i].cost + f[spNodesNum - 1][i][j].cost < min) {
				min = D[spNodesNum][i].cost + f[spNodesNum - 1][i][j].cost;
				addi = i;
				addj = j;
			}
		}

		if(addi != -1) {
			D[spNodesNum][i].handleCycle(f[spNodesNum - 1][addi][addj], work);	
			if(D[spNodesNum][i].cost != -1)
				f0[i] = D[spNodesNum][i] + f[spNodesNum - 1][addi][addj];
			else f0[i].cost = -1;
		}
		else f0[i].cost = -1;
	}
#ifdef DEBUG
	cout << "f0:" << endl;
	for(int i = 0; i < spNodesNum; i++)
		if(f0[i].cost > 0) f0[i].printPath(work);
#endif

#ifdef GAME
	int min = INT_MAX;
	int ans = -1;
	for(int i = 0; i < spNodesNum; i++) {
		if(f0[i].cost > 0 && f0[i].cost < min) {
			min = f0[i].cost;
			ans = i;
		}
	}
	if(ans == -1) {
		for(int i = (int)f0[ans].edges.size() - 1; i >= 0; i--)
			record_result(f0[ans].edges[i]);
	}
#endif

//    unsigned short result[] = {2, 6, 3};//示例中的一个解

//    for (int i = 0; i < 3; i++)
//        record_result(result[i]);

}
