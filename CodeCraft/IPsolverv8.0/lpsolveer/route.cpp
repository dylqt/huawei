#include "route.h"
#include "lib_record.h"
#include <stdio.h>


int lpsolver_verNum;					// ver number
int lpsolver_specialNum;				// special number
int lpsolver_start;					// s
int lpsolver_terminial;				// t

vector<int> lpsolver_specialNodes;	// special vertex set
vector<bool> lpsolver_spNodesSet;	// set special nodes indexNum be true 

vector<vector<int> > lpsolver_weightTable;	//
vector<vector<int> > lpsolver_idTable;


vector<vector<int> > lpsolver_indexTable;	// lpsolver_idTable to index
int Ncol; 					// the number of structial variable in expression 
lprec *lp;

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

bool Path::operator==(const Path &rhs)
{
	if(this->cost != rhs.cost || this->start != rhs.start || this->target != rhs.target) return false;
	if(this->edges.size() != rhs.edges.size()) return false;
	else {
		for(int i = 0; i < (int)this->edges.size(); i++) {
			if(this->edges[i] != rhs.edges[i]) return false;
			else continue; 
		}
	} 
	return true;	
}

//打印路径函数
void Path::printPath(class Dijkstra &work)
{
#ifdef PRINT_PAHT
	int costTest = 0;
	cout << "Path:" << start << "->" << target << "cost:" << cost << ",edges(";
	for(int i = (int)edges.size() - 1; i >= 0; i--) {
		costTest += work.edges[edges[i]].dist;
		if(i != 0) cout << edges[i] << '|';
		else cout << edges[i] << ')';
	}
	if(costTest != cost) cout << "cost wrong" << endl;
	else cout << "cost right" << endl;
	cout << "spNodes(";
	sort(specialNodes.begin(), specialNodes.end());
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

//处理环
void Path::handleCycle(Path &rhs, Dijkstra &work, bool judgeCycle)
{
#ifdef HANDLE_CYCLE
	bool hasCycle = false;
	if(judgeCycle) {	
		for(int i = 0; i < (int)this->freeNodes.size(); i++) { //判断自由节点是否重复
			if(find(rhs.freeNodes.begin(), rhs.freeNodes.end(), this->freeNodes[i]) != rhs.freeNodes.end()) {
				hasCycle = true;
				break;
			}
		}
		if(hasCycle == false) { //判断特殊节点是否重复
			if(this->target == rhs.start) {// this + rhs
				for(int i = 0; i < (int)this->specialNodes.size(); i++) {
					 if(this->specialNodes[i] != this->target &&\
					find(rhs.specialNodes.begin(), rhs.specialNodes.end(), this->specialNodes[i]) !=\
					 rhs.specialNodes.end()) {
						hasCycle = true;
						break;
					}
				}
			}
			else if(this->start == rhs.target) { //rhs + this
				for(int i = 0; i < (int)this->specialNodes.size(); i++) {
					 if(this->specialNodes[i] != this->start &&\
					find(rhs.specialNodes.begin(), rhs.specialNodes.end(), this->specialNodes[i]) !=\
					 rhs.specialNodes.end()) {
						hasCycle = true;
						break;
					}
				}
			}
		}
		if(hasCycle == false) return;
	}

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

	if(repeatNodes.size() == 0) return ;
	else {
		work.dijkstra(this->start, repeatNodes);
		work.BuildPath(this->start, this->target, *this);
	}

	/*if(hasCycle == false) {
		if(prepath == *this) cout << "no problem" << endl;
		else {
			cout << "no Cycle, but have bug." << endl;
			prepath.printPath(work);
			this->printPath(work);
			rhs.printPath(work);
		}
	}*/
#endif
}

//对当前路径插入一个不包含的特殊节点
bool Path::insertSpNodes(Dijkstra &work)
{
	for(int i = 0; i < (int)work.includingSet.size(); i++) { //遍历所有特殊节点，找到哪个特殊节点还没有插入
		if(find(this->specialNodes.begin(), this->specialNodes.end(), work.includingSet[i]) == this->specialNodes.end()) {
			for(int j = (int)this->edges.size() - 1; j >= 0; j--) { //遍历边，看能在哪里插入
				int start, target; //在这两个节点之间插入特殊节点
				if(j == (int)this->edges.size() - 1)
					start = work.edges[this->edges[j]].from, target = work.edges[this->edges[j]].to; 
				else {
					start = target;
					target = work.edges[this->edges[j]].to;
				}

				vector<int> rmNodes; //不能经过的节点
				Path tmp1, tmp2; //新添加的两条路径

				rmNodes.clear();	
				rmNodes.push_back(work.start);
				rmNodes.push_back(work.target);
				for(int isp = 0; isp < (int)specialNodes.size(); isp++)
					if(this->specialNodes[isp] != start) rmNodes.push_back(this->specialNodes[isp]);
				for(int ifr = 0; ifr < (int)freeNodes.size(); ifr++)
					if(this->freeNodes[ifr] != start) rmNodes.push_back(this->freeNodes[ifr]);

				work.dijkstra(start, rmNodes);
				work.BuildPath(start, work.includingSet[i], tmp1); //构造第一条路径

				if(tmp1.cost < 0) continue;

				rmNodes.clear();
				rmNodes.push_back(work.start);
				if(target != work.target) rmNodes.push_back(work.target);
				for(int isp = 0; isp < (int)specialNodes.size(); isp++)
					if(this->specialNodes[isp] != target) rmNodes.push_back(this->specialNodes[isp]);
				for(int ifr = 0; ifr < (int)freeNodes.size(); ifr++)
					if(this->freeNodes[ifr] != target) rmNodes.push_back(this->freeNodes[ifr]);
				for(int itm = 0; itm < (int)tmp1.freeNodes.size(); itm++) rmNodes.push_back(tmp1.freeNodes[itm]);

				work.dijkstra(work.includingSet[i], rmNodes);
				work.BuildPath(work.includingSet[i], target, tmp2); //构造第二条路径

				if(tmp1.cost > 0 && tmp2.cost > 0) { //构造路径成功
					this->cost += tmp1.cost + tmp2.cost - work.edges[this->edges[j]].dist; //新路径权值计算
					this->specialNodes.push_back(work.includingSet[i]);
					//若新路径经过特殊节点，则也要加入
					for(int i = 0; i < (int)tmp1.specialNodes.size(); i++)
						if(find(this->specialNodes.begin(), this->specialNodes.end(), tmp1.specialNodes[i]) ==\
						 this->specialNodes.end())
							this->specialNodes.push_back(tmp1.specialNodes[i]);
					for(int i = 0; i < (int)tmp2.specialNodes.size(); i++)
						if(find(this->specialNodes.begin(), this->specialNodes.end(), tmp2.specialNodes[i]) ==\
						 this->specialNodes.end())
							this->specialNodes.push_back(tmp2.specialNodes[i]);

					for(int i = 0; i < (int)tmp1.freeNodes.size(); i++)
						this->freeNodes.push_back(tmp1.freeNodes[i]);
					for(int i = 0; i < (int)tmp2.freeNodes.size(); i++)
						this->freeNodes.push_back(tmp2.freeNodes[i]);	
			
					vector<int> tmpedges; //新路径的边
					for(int i = 0; i < (int)this->edges.size(); i++) {
						if(i != j) tmpedges.push_back(this->edges[i]);
						else {
							for(int k = 0; k < (int)tmp2.edges.size(); k++)
								tmpedges.push_back(tmp2.edges[k]);
							for(int k = 0; k < (int)tmp1.edges.size(); k++)
								tmpedges.push_back(tmp1.edges[k]);
						}
					}
					this->edges = tmpedges;

					return true; //节点插入成功
				}
				else continue;
			} //遍历边
		}
		else continue; //特殊节点已经包含
	}
	return false;
}

//当前路径是否是答案
bool Path::pathIsAns(Dijkstra &work)
{
	set<int> allNodes;
	vector<int> spNodes;

	for(int i = (int)this->edges.size() - 1; i >= 0; i--) {
		if(i == (int)this->edges.size() - 1) {
			allNodes.insert(work.edges[this->edges[i]].from);
			allNodes.insert(work.edges[this->edges[i]].to);
			if(work.isIncludingSet[work.edges[this->edges[i]].to]) {
				spNodes.push_back(work.edges[this->edges[i]].to);
				//cout << work.edges[this->edges[i]].to << endl;
			}
		}
		else {
			if(work.edges[this->edges[i]].from != work.edges[this->edges[i + 1]].to) { //边不能构成路径
				cout << "edges error!" << endl;
				return false;
			}
			if(allNodes.count(work.edges[this->edges[i]].to) != 0) { //有重复的节点（同时判断自由节点和特殊节点）
				cout << "repet!" << work.edges[this->edges[i]].to << endl;
				return false;
			}
			else { //节点未重复
				allNodes.insert(work.edges[this->edges[i]].to);			
				if(work.isIncludingSet[work.edges[this->edges[i]].to]) { //此节点是特殊节点
					spNodes.push_back(work.edges[this->edges[i]].to);
					//cout << work.edges[this->edges[i]].to << endl;
				}
			}
		}
	}

	if(spNodes.size() < work.includingSet.size()) { //特殊节点个数不够
		cout << "not all spNodes!" << spNodes.size() << endl;
		return false;
	}
	else return true;
}

void Dijkstra::init(char *topo[5000], int edge_num, char *demand) {	//用节点数目初始化
	m = edge_num;
	n = 0;

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
		isIncludingSet.set(from);
	}while(ss >> tmp);
}

void Dijkstra::AddEdge(int index, int from, int to, int dist) {	//添加一条边
	//只添加一个删除旧边的操作
	if(G[from].size() != 0) { //此起点已经有边存在
		for(int i = 0; i < (int)G[from].size(); i++) {
			if(edges[G[from][i]].to == to && edges[G[from][i]].dist > dist) {
				G[from][i] = -1; //删除旧边
			}
		}
	}

	while(1) {
		if((int)edges.size() >= index) {
			edges.push_back(Edge(index, from, to, dist));	//此处假设边的读入顺序和index相同。否则需要事先设计edges的大小。
			break;
		}
		else edges.push_back(Edge(-1, -1, -1, -1));
	}
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
		for (unsigned int i = 0; i < G[u].size() && G[u][i] != -1; i++) {	//	遍历节点u的所有边, 删除的边不遍历
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

bitset<maxn> ISPNODES;
bool compare(const Edge &a, const Edge &b)
{
	/*if(ISPNODES[a.to] && !ISPNODES[b.to]) return true;
	else if(ISPNODES[b.to] && !ISPNODES[a.to]) return false;
	else return a.dist < b.dist;*/
	if(ISPNODES[a.to] && !ISPNODES[b.to]) return false;
	else if(ISPNODES[b.to] && !ISPNODES[a.to]) return true;
	else return a.dist > b.dist;
	//return a.dist > b.dist;
}

bool DFS::spNodesDegree(bitset<maxn> &allNodes, const vector<int> &spNodes, vector<int> &newSpNodes, bitset<maxn> &isdone)
{
	bool cut = false;

	while(1) {
		bool flag = false;
		for(unsigned int i = 0; i < spNodes.size(); i++) {
			if(!allNodes[spNodes[i]]) continue;
			int indegree = 0, outdegree = 0, from, to;
			for(int j = 0; j < allNodesNum; j++) {
				if(G[0][j][spNodes[i]] != -1) {
					indegree++;
					from = j;
				}
			}
			for(int j = 0; j < allNodesNum; j++) {
				if(G[0][spNodes[i]][j] != -1) {
					outdegree++;
					to = j;
				}
			}
			if(indegree == 1) {
				flag = true;
				allNodes.reset(spNodes[i]);
				isdone.set(spNodes[i]);
				for(int j = 0; j < allNodesNum; j++) {
					if(j != spNodes[i]) G[0][from][j] = -1;
				}
				G[0][spNodes[i]][from] = -1;
				newSpNodes.push_back(from);
				//cout << "nodes:" << spNodes[i] << " indegree:" << 1 << " from:" << from << endl;
			}
			if(outdegree == 1) {
				flag = true;
				allNodes.reset(spNodes[i]);
				isdone.set(spNodes[i]);
				for(int j = 0; j < allNodesNum; j++) {
					if(j != spNodes[i]) G[0][j][to] = -1;
				}
				G[0][to][spNodes[i]] = -1;
				newSpNodes.push_back(to);
				//cout << "nodes:" << spNodes[i] << " outdegree:" << 1 << " to:" << to << endl;
			}
		}
		if(!flag) break;
		else cut = true;
	}
	return cut;
}

void DFS::init(char *topo[5000], int edge_num, char *demand, Dijkstra &work)
{
	memset(G, -1, sizeof(G));
	int index, from, to, dist;
	char tmp;
	allNodesNum = -1;

	for(int i = 0; i < edge_num; i++){
		stringstream ss;
		ss << topo[i];
	    	ss >> index >> tmp >> from >> tmp >> to >> tmp >> dist;
		if(G[0][from][to] == -1 || dist < G[0][from][to]) {
			G[0][from][to] = dist;
			G[1][from][to] = index;
		}

		int tmp = max(from, to);
		allNodesNum = max(allNodesNum, tmp + 1);
	}

	stringstream ss;
	ss << demand;
	ss >> start >> tmp >> target >> tmp;
	do{
		ss >> from;
		includingSet.push_back(from);
		isIncludingSet.set(from);
	}while(ss >> tmp);
	ISPNODES = isIncludingSet;
/*********************************************************************************/
	G[0][start][target] = -1; // start->target is no use
	memset(G[0][target], -1, sizeof(G[0][target])); // target->any is no use

	vector<int> spNodes = includingSet;
	vector<int> newSpNodes;
	bitset<maxn> allNodes;
	bitset<maxn> isdone; 

	for(unsigned int i = 0; i < spNodes.size(); i++) 
		allNodes.set(spNodes[i], true); //who is sp node

	//cout << "sp Nodes:";
	//sort(spNodes.begin(), spNodes.end());
	//for(int i = 0; i < (int)spNodes.size(); i++) cout << spNodes[i] << ' ';
	//cout << endl;

	while(spNodesDegree(allNodes, spNodes, newSpNodes, isdone)) {
		allNodes.reset();
		spNodes.clear();
		for(unsigned int i = 0; i < newSpNodes.size(); i++) {
			if(!isdone[newSpNodes[i]]) {
				spNodes.push_back(newSpNodes[i]);
				allNodes.set(newSpNodes[i], true);
			}
		}
		if(spNodes.size() == 0) break;
		newSpNodes.clear();
		//cout << "new sp Nodes:";
		//sort(spNodes.begin(), spNodes.end());
		//for(int i = 0; i < (int)spNodes.size(); i++) cout << spNodes[i] << ' ';
		//cout << endl;
	}

	int outdegree = 0;
	for(int i = 0; i < allNodesNum; i++) {
		if(!isdone[i]) continue;
		else {
			for(int j = 0; j < allNodesNum; j++) {
				if(G[0][i][j] != -1) outdegree++;
			}
			if(outdegree == 0) {
				//cout << "outdegree == 0" << endl;
				for(int j = 0; j < allNodesNum; j++) G[0][i][j] = -1;
			}
		}
	}
/*********************************************************************************/
	for(int i = 0; i < allNodesNum; i++) {
		for(int j = 0; j < allNodesNum; j++) {
			if(i == j || G[0][i][j] == -1) continue;
			G_list[i].push_back(Edge(G[1][i][j], i, j, G[0][i][j]));
			//work.G[i].push_back(G[1][i][j]);
		}
		sort(G_list[i].begin(), G_list[i].end(), compare);
	}
#ifdef DEBUG
	for(int i = 0; i < allNodesNum; i++) {
		cout << i << ":";
		for(int j = 0; j < (int)G_list[i].size(); j++) cout << G_list[i][j].dist << "(" << G_list[i][j].num << ") ";
		cout << endl; 
	}
	//while(1);
#endif
/*********************************************************************************/
	bestAns.clear();
	bestCost = INT_MAX;
	for(int i = 0; i < allNodesNum; i++) done.push_back(false);

#ifdef DEBUG
	cout << "allNodesNum:" << allNodesNum << endl;
	for(int i = 0; i < allNodesNum; i++) {
		for(int j = 0; j < allNodesNum; j++) {
			if(G[0][i][j] != INT_MAX) cout << G[1][i][j] << "(" << G[0][i][j] << ") ";
		}
		cout << endl;
	}
	cout << "start:" << start << " target:" << target << endl;
	cout << "bestCost:" << bestCost << endl;
	cout << "spNodes:";
	for(int i = 0; i < (int)includingSet.size(); i++) cout << includingSet[i] << ' ';
	cout << endl;
	cout << "done:";
	for(int i = 0; i < (int)done.size(); i++) cout << done[i] << ' ';
	cout << endl;
#endif
}

/*void DFS::dfs(Dijkstra &work)
{
	stack<Edge> path; //当前路径
	int cost = 0, spNum = 0; //当前路径权重,当前路径特殊节点个数

	int bestSize = maxn;
	int allSpNum = includingSet.size();
	//double cut3 = 1.0 * allNodesNum / allSpNum;

	stack<vector<int> > stk; //节点遍历
	int u = start; //start point

	vector<int> tmp;
	tmp.push_back(u);
	tmp.push_back(0);
	stk.push(tmp); //start from u,

	done[u] = true;

	while(stk.size() != 0) {
		int hasPath = false;
		for(int i = stk.top()[1]; i < (int)G_list[u].size(); i++) {
			if(done[G_list[u][i].to]) continue;
			else if(cost + G_list[u][i].dist >= bestCost) break; //cost cut
			//else if((int)path.size() >= bestSize - allSpNum + spNum) break; //edges cut
			//else if(spNum && (1.0 * stk.size() / spNum > cut3)) continue; //
			//else if(cost > )
			else {
				//cout << "u:" << u << " v:" << G_list[u][i].to << endl;
				hasPath = true;
				cost += G_list[u][i].dist;
				path.push(G_list[u][i]);
				if(isIncludingSet[G_list[u][i].to]) spNum++;
				done[G_list[u][i].to] = true;
			
				vector<int> tmp;
				tmp.push_back(G_list[u][i].to);
				tmp.push_back(0);
				stk.top()[1] = i + 1;
				stk.push(tmp);

				if(spNum == (int)includingSet.size()) {
					//cout << "all ap find..." << endl;
					vector<int> rmNodes;
					Path tmp;
					
					stack<Edge> tmppath = path;
					while(!tmppath.empty()) {
						rmNodes.push_back(tmppath.top().from);
						tmppath.pop();
					}
					
					work.dijkstra(G_list[u][i].to, rmNodes);
					work.BuildPath(G_list[u][i].to, target, tmp);
					//cout << "tmp.cost:" << tmp.cost << endl;
					if(tmp.cost != -1 && cost + tmp.cost < bestCost) {
						bestAns.clear();
						for(int i = tmp.edges.size() - 1; i >= 0; i--) 
							bestAns.push_back(work.edges[tmp.edges[i]].num); //edges
						stack<Edge> tmppath = path;
						while(!tmppath.empty()) {
							bestAns.push_back(tmppath.top().num);
							tmppath.pop();
						}
						bestCost = cost + tmp.cost; //cost
						bestSize = bestAns.size();
						cout << "find:" << bestCost << " size:" << bestAns.size() << endl;
						if(bestCost == breakCost) goto end;
					}

					if(isIncludingSet[path.top().to]) spNum--;
					path.pop();

					cost -= G_list[u][i].dist;
					done[G_list[u][i].to] = false;	
					hasPath = false;
					stk.pop();
				}
				else {
					u = G_list[u][i].to;
					break;
				}

				if(G_list[u][i].to == target) {
					if(spNum == (int)includingSet.size() && cost < bestCost) { //找到一条路径
						stack<Edge> tmp = path;
						bestAns.clear();
						while(!tmp.empty()) {
							bestAns.push_back(tmp.top().num);
							tmp.pop();
						}
						bestCost = cost;
						bestSize = bestAns.size();
						cout << "find:" << bestCost << " size:" << bestAns.size() << endl;
						//cout << " " << 1.0 * bestCost / bestAns.size() << endl;
						if(bestCost == breakCost) goto end;
					}	
					
					if(isIncludingSet[path.top().to]) spNum--;
					path.pop();
					
					cost -= G_list[u][i].dist;
					done[G_list[u][i].to] = false;	
					hasPath = false;
					stk.pop();	
				}
				else {
					u = G_list[u][i].to;
					break;
				}
			} // for
		}
		if(hasPath == false) {
			if(!stk.empty()) {
				stk.pop();
				if(!stk.empty()) u = stk.top()[0];
			}
			
			if(!path.empty()) {
				cost -= path.top().dist;
				done[path.top().to] = false;
				if(isIncludingSet[path.top().to]) spNum--;
				path.pop();
			}
		}
	} //while
end:
	for(int i = bestAns.size() - 1; i >= 0; i--) {
		record_result(bestAns[i]);
		cout << bestAns[i] << ' ';
	}
	cout << endl;

	//cout << "cost:" << bestCost << " size:" << bestAns.size() << endl;
}*/

void DFS::dfs(void)
{
	int curCost = 0;  // current cost
	int spNum = 0;		// visited special number 
	done = vector<bool>(allNodesNum, false);
	vector<int> bestAnsTmp;

	// for check if node is special node
	bitset<maxn> allNodes;
	for(int i = 0; i < (int)includingSet.size(); i++)
		allNodes.set(includingSet[i], true);

	// push edges (from start) into stack & keep the bestAns 
	stack<Edge> stk;
	for(int i = 0; i < (int)G_list[start].size(); i++)
		stk.push(G_list[start][i]);

	//bestAns.push_back(G_list[start][G_list[start].size() - 1].num);
	done[start] = true;

	Edge lastEnd(-1, -1, -1, -1);
	// while the stack is not empty, do it 
	while(!stk.empty()){
		// get the first edge & get this edge's start named "s" 
		Edge minTmp = stk.top();
		int nextS = minTmp.to;
//cout << "------------------" << endl;
		if(minTmp.num == lastEnd.num){	// last time is not a new member
			done[nextS] = false;
			curCost -= minTmp.dist;
			if(allNodes[nextS] == true)
				--spNum;
			bestAnsTmp.pop_back();
			stk.pop();
#ifdef DEBUG
	cout << "pop out : ";
	lastEnd.toString();
	cout << "spNum : ";
	cout << spNum << endl;
	cout << "bestPath : ";
	for(int i = 0; i < (int)bestAnsTmp.size(); ++i)
		cout << bestAnsTmp[i] << " ";
	cout << endl;
#endif
			if(stk.empty())
				goto end;
			minTmp = stk.top();
			nextS = minTmp.to;
			while(nextS == lastEnd.from){
				done[nextS] = false;
				curCost -= minTmp.dist;
				if(allNodes[nextS] == true)
					--spNum;
				bestAnsTmp.pop_back();
				stk.pop();

				lastEnd = minTmp;
#ifdef DEBUG
	cout << "pop out : ";
	lastEnd.toString();
	cout << "spNum : ";
	cout << spNum << endl;
	cout << "bestPath : ";
	for(int i = 0; i < (int)bestAnsTmp.size(); ++i)
		cout << bestAnsTmp[i] << " ";
	cout << endl;
#endif
				if(stk.empty())
					goto end;
				minTmp = stk.top();
				nextS = minTmp.to;
			}
		}

		done[nextS] = true;
		curCost += minTmp.dist;
		if(allNodes[nextS] == true)
			++spNum;
		bestAnsTmp.push_back(minTmp.num);
		lastEnd = minTmp;
#ifdef DEBUG
	cout << "push in :";
	minTmp.toString();
	cout << "spNum : ";
	cout << spNum << endl;
	cout << "bestPath : ";
	for(int i = 0; i < (int)bestAnsTmp.size(); ++i)
		cout << bestAnsTmp[i] << " ";
	cout << endl;
#endif
		// go over the edges edge to "s" 
		for(int i = 0; i < (int)G_list[nextS].size(); ++i){
			// if this curCost < bestCost 
			if(!done[G_list[nextS][i].to] && curCost + G_list[nextS][i].dist < bestCost){	
#ifdef DEBUG
	G_list[nextS][i].toString();
	cout << spNum << " " << includingSet.size() << endl;
#endif
				// if E.to == terminal 
				
				if(G_list[nextS][i].to != target){	
					stk.push(G_list[nextS][i]);			
				}
				else{ 
					if(spNum == (int)includingSet.size()){
						// check if this path is ans 
						bestCost = curCost + G_list[nextS][i].dist;
						bestAns = bestAnsTmp;
						bestAns.push_back(G_list[nextS][i].num);
						if(bestCost == breakCost) goto end;

#ifdef DEBUG
	cout << "path : " ;						
	for(int i = 0; i < (int)bestAns.size(); ++i){
		cout << bestAns[i] << " ";
	}
	cout << endl;
#endif
					}
				}
			}
		}
	}
end:
	for(int i = 0; i < (int)bestAns.size(); ++i){
		record_result(bestAns[i]);
		cout << bestAns[i] << " ";
	}
	cout << endl;
	cout << "cost:" << bestCost << endl;
}

void Dijkstra::randFindPath(clock_t startTime, clock_t endTime)
{
	Path bestPath;
	bestPath.cost = INT_MAX; //用于剪枝

	//cout << "nodes:" << includingSet.size() << endl;
	vector<int> remSpNodes;

	for(;;) {

		endTime = clock();
		if((endTime - startTime) / CLOCKS_PER_SEC > 8.4) break;

		int u = start;
		vector<int> rmNodes;
		Path path;
		remSpNodes = includingSet;

		for(;;) {
			int v = rand() % remSpNodes.size();
			v = remSpNodes[v];
			rmNodes.clear();
			if(u != start) rmNodes.push_back(start);
			rmNodes.push_back(target); //最后一个sp节点参能经过终点
			//for(int i = 0; i < (int)remSpNodes.size(); i++) 
			//	if(remSpNodes[i] != v) rmNodes.push_back(remSpNodes[i]);
			if(path.freeNodes.size() != 0) {
				for(int i = 0; i < (int)path.freeNodes.size(); i++)
					rmNodes.push_back(path.freeNodes[i]);
			}
			for(int i = 0; i < (int)path.specialNodes.size(); i++)
				if(path.specialNodes[i] != u) rmNodes.push_back(path.specialNodes[i]);
			
			/*cout << "u:" << u << " v:" << v << endl;
			cout << "rmNodes:";
			for(int i = 0; i < (int)rmNodes.size(); i++) cout << rmNodes[i] << ' ';
			cout << endl;
			cout << "remSpNodes:";
			for(int i = 0; i < (int)remSpNodes.size(); i++) cout << remSpNodes[i] << ' ';
			cout << endl;*/

			dijkstra(u, rmNodes);
			Path tmp;
			BuildPath(u, v, tmp);
			if(tmp.cost != -1) { 
				if(u == start) path = tmp;
				else path = path + tmp;
				path.printPath(*this);

				if(path.cost >= bestPath.cost) break;
			}
			else break; //找不到就重新开始
			
			for(int i = 0; i < (int)tmp.specialNodes.size(); i++) {
				if(tmp.specialNodes[i] != u)
					remSpNodes.erase(find(remSpNodes.begin(), remSpNodes.end(), tmp.specialNodes[i]));
				else continue;
			}
			u = v;
			if(remSpNodes.size() == 0) {
				rmNodes.clear();
				for(int i = 0; i < (int)path.freeNodes.size(); i++)
					rmNodes.push_back(path.freeNodes[i]);
				rmNodes.push_back(start);
				for(int i = 0; i < (int)path.specialNodes.size(); i++)
					if(path.specialNodes[i] != u) rmNodes.push_back(path.specialNodes[i]);

				dijkstra(u, rmNodes);
				BuildPath(u, target, tmp);

				if(tmp.cost != -1) {
					path = path + tmp;
					//cout << "cost:" << path.cost << endl;
					if(path.cost < bestPath.cost) {
						bestPath = path;
						//goto end;
						//cout << "rand:" << bestPath.cost << endl; 
						//cout << "ii:" << ii << endl;
					}
					//path.printPath(*this);					
					//if(path.pathIsAns(*this) == true) cout << "ans is true!" << endl;
					break;
				}
				else break; //找不到就重新开始
			}
		}
	}
	for(int i = (int)bestPath.edges.size() - 1; i >= 0; i--) 
		record_result(bestPath.edges[i]);
	//cout << "cost:" << bestPath.cost << endl;	
}


//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	if(edge_num > 1200 && edge_num < 2000)
		goto SK;
	if(edge_num > 2200)
		goto SK;

	initGraph(topo, edge_num, demand);
	lpSolver();
	return;

SK:
	clock_t start, end = 0;
	start = clock();

	unsigned int seed1 = 9999999;	
	unsigned int seed2 = 9999999;
	unsigned int seed3 = 9999999;

	bool fmin = true;
	bool cut = false;
	bool onemoretime = true;
	bool noInsert = true; //when there is no path, insert?

	bool hasPath = false; //一次迭代后是否还有有效路径
	bool notChangeD = true;
	bool judgeCycle = true; // 判断环，true:速度快一般 false:保险版

	double uptime;
	if(edge_num <= 1500) uptime = 8.8;
	else uptime = 7.8;

	Path outPath;
	outPath.cost = INT_MAX;
/***************************************************************************/
	Dijkstra work;
	work.init(topo, edge_num, demand);

	DFS dfswork;
	dfswork.init(topo, edge_num, demand, work);
/***************************************************************************/
	if(edge_num >= 2200) { //case 14 15
	//else if(0) {
		fmin = false;
		cut = true;
		onemoretime = false;
		srand(seed2);
	}
	else srand(seed3);

/*SK**************************************************************************************************/
	const int spNodesNum = work.includingSet.size();
	//这个D只能包好必须经过的节点和起点and终点。这里面的点的下标和includingSet中的下标必须一致，起点放最后一行，终点放最后一列。
	Path D[spNodesNum + 1][spNodesNum + 1];	//i:表示起点编号 j:表示终点编号;两点之间的最短路径矩阵,
	Path f[spNodesNum][spNodesNum][spNodesNum];	//路径经过哪些边的矩阵。
	Path f0[spNodesNum];

	//初始化矩阵
	memset(D, 0, sizeof(D));
	memset(f, 0, sizeof(f));
	memset(f0, 0, sizeof(f0));

	//Dijkstra构造两两之间的最短路径，若路径不存在其权重为-1
	vector<int> rmNodes;
	for(int i = 0; i < spNodesNum; i++) {	//在special nodes中遍历起点
		rmNodes.clear();
		rmNodes.push_back(work.start);
		rmNodes.push_back(work.target);	
		work.dijkstra(work.includingSet[i], rmNodes);	//移除起点和终点进行计算
		for(int j = 0; j < spNodesNum; j++){
			if(i == j) continue;
			work.BuildPath(work.includingSet[i], work.includingSet[j], D[i][j]);
		}

		rmNodes.clear();
		rmNodes.push_back(work.start);	//只移除起点进行计算
		work.dijkstra(work.includingSet[i], rmNodes);	

		work.BuildPath(work.includingSet[i], work.target, D[i][spNodesNum]);
	}
	rmNodes.clear();
	rmNodes.push_back(work.target);	//只移除终点进行计算
	work.dijkstra(work.start, rmNodes);	
	for(int j = 0; j < spNodesNum; j++){
		work.BuildPath(work.start, work.includingSet[j], D[spNodesNum][j]);
	}

beginSK:
	memset(f, 0, sizeof(f));
	memset(f0, 0, sizeof(f0));
	//SK迭代
	for(int i = 0; i < spNodesNum; i++) {	//i:只表示需要循环的次数
		//cout << "SK迭代i=" << i << endl;
		if(i == 0) {
			for(int j = 0; j < spNodesNum; j++) {	//j:遍历起点--vi
				for(int k = 0; k < spNodesNum; k++) {	//k:遍历终点--vl
					if(j == k) continue;
					if(D[j][k].cost > 0 && D[k][spNodesNum].cost > 0) {
						Path tmp = D[j][k];
						Path tmp2 = D[k][spNodesNum];
						D[j][k].handleCycle(D[k][spNodesNum], work, judgeCycle);
						if(D[j][k].cost != -1) {
							f[i][j][k] = D[j][k] + D[k][spNodesNum];
						}
						else {
							D[j][k] = tmp;
							D[k][spNodesNum].handleCycle(D[j][k], work, judgeCycle);
							if(D[k][spNodesNum].cost != -1) {
								f[i][j][k] = D[j][k] + D[k][spNodesNum];
							}
							else f[i][j][k].cost = -1;		
						}
						if(notChangeD) {
							D[j][k] = tmp;
							D[k][spNodesNum] = tmp2;
						}
					}
					else f[i][j][k].cost = -1;	//这次迭代不存在这条路径
					if(cut && f[i][j][k].cost != -1 && f[i][j][k].cost >= outPath.cost) f[i][j][k].cost = -1; //cut
				}	
			}
		}
		else {
			for(int j = 0; j < spNodesNum; j++) {	//j:遍历起点
				for(int k = 0; k < spNodesNum; k++) {	//k:遍历终点
					if(j == k) continue;

					int min = INT_MAX;
					int addk, addl;
					addk = addl = -1;					
					vector<int> vecaddk;
					vector<int> vecaddl;
					if(D[j][k].cost > 0) {
						for(int l = 0; l < spNodesNum; l++) {
							if(k == l || j == l) continue;	//跳过起点和终点相同，或者出现环。
							if(f[i - 1][k][l].cost > 0) {
								vecaddk.push_back(k);
								vecaddl.push_back(l);
								if(D[j][k].cost + f[i - 1][k][l].cost < min) {
									min = D[j][k].cost + f[i - 1][k][l].cost;
									addk = k;
									addl = l;
								}
							}	
						}
					}
					else {
						f[i][j][k].cost = -1;
						continue;
					}

					if(addk != -1) {	//找到一条路径（若addk==-1则之前也不可能有这条路径）
						Path tmp = D[j][k];
						if(fmin) {
							D[j][k].handleCycle(f[i - 1][addk][addl], work, judgeCycle);
							if(D[j][k].cost != -1) {
								f[i][j][k] = D[j][k] + f[i - 1][addk][addl];
							}
							else f[i][j][k].cost = -1;
						}
						else {
							int kk, ll, rd;
							rd = rand() % vecaddk.size(); 
							kk = vecaddk[rd];
							ll = vecaddl[rd];
							D[j][k].handleCycle(f[i - 1][kk][ll], work, judgeCycle);
							if(D[j][k].cost != -1) {
								f[i][j][k] = D[j][k] + f[i - 1][kk][ll];
							}
							else f[i][j][k].cost = -1;
						}
						if(notChangeD) D[j][k] = tmp;	
					
						//SK--(2)--(3)
						if(f[i][j][k].cost < 0 && (int)f[i - 1][j][k].specialNodes.size() >= i + 1) {
							f[i][j][k] = f[i - 1][j][k];
						}
						if((int)f[i][j][k].specialNodes.size() >= i + 1 &&\
					           (int)f[i - 1][j][k].specialNodes.size() == (int)f[i][j][k].specialNodes.size() &&\
						   f[i - 1][j][k].cost < f[i][j][k].cost) {
							f[i][j][k] = f[i - 1][j][k];
						}
					}
					else if((int)f[i - 1][j][k].specialNodes.size() >= i + 1) {
						f[i][j][k] = f[i - 1][j][k];				
					}
					else f[i][j][k].cost = -1;	//此条路径终结
					//cut
					if(cut && f[i][j][k].cost != -1 && f[i][j][k].cost >= outPath.cost) f[i][j][k].cost = -1;
				}	
			}
		} //迭代结束
		//判断是否存在有效路径
		hasPath = false;
		for(int j = 0; j < spNodesNum; j++) {		
			for(int k = 0; k < spNodesNum; k++) {
				if(j != k && f[i][j][k].cost > 0) {
					hasPath = true;
					break;
				}
			}
		}
		if(hasPath == true) continue;
		else {
			if(noInsert) {
				end = clock();
				if((end - start)/CLOCKS_PER_SEC <= uptime) {
					if(fmin) fmin = false;
				}
				else break;
			}
			else{
				for(int j = 0; j < spNodesNum; j++) {		
					for(int k = 0; k < spNodesNum; k++) {
						if(j != k && f[i - 1][j][k].cost > 0) {
							if(f[i - 1][j][k].insertSpNodes(work) == true) {
								f[i][j][k] = f[i - 1][j][k];
								if(cut && f[i][j][k].cost >= outPath.cost) f[i][j][k].cost = -1; //cut
								else hasPath = true;
							}
						}
					}
				}
				if(hasPath == false) {
					end = clock();
					if((end - start)/CLOCKS_PER_SEC <= uptime) {
						if(fmin) fmin = false;
						goto beginSK;
					}
					else break;
				}
			}
		}
	}//迭代循环
	
	//vi = work.start;
	for(int i = 0; i < spNodesNum; i++) {	//遍历D中的终点，也就是f中的起点
		int min = INT_MAX;
		int addi, addj;
		addi = addj = -1;

		for(int j = 0; j < spNodesNum; j++) {	//遍历f中的终点
			if(i == j) continue;
			if(D[spNodesNum][i].cost > 0 && f[spNodesNum - 1][i][j].cost > 0 && \
			   D[spNodesNum][i].cost + f[spNodesNum - 1][i][j].cost < min) {
					min = D[spNodesNum][i].cost + f[spNodesNum - 1][i][j].cost;
					addi = i;
					addj = j;
			}
		}

		if(addi != -1) {
			Path tmp = D[spNodesNum][i];

			D[spNodesNum][i].handleCycle(f[spNodesNum - 1][addi][addj], work, judgeCycle);	
			if(D[spNodesNum][i].cost != -1) {
				f0[i] = D[spNodesNum][i] + f[spNodesNum - 1][addi][addj];
				if(cut && f0[i].cost >= outPath.cost) f0[i].cost = -1; //cut
			}
			else f0[i].cost = -1;

			if(notChangeD) D[spNodesNum][i] = tmp;
			
		}
		else f0[i].cost = -1;
	}

	int min = INT_MAX;
	int ans = -1;
	for(int i = 0; i < spNodesNum; i++) {
		if(f0[i].cost > 0 && f0[i].cost < min) {
			min = f0[i].cost;
			ans = i;
		}
	}

	if(ans != -1) {
		if(f0[ans].cost < outPath.cost) 
			outPath = f0[ans];
		//f0[ans].printPath(work);
		cout << f0[ans].cost << " " << outPath.cost << endl;
	}

	end = clock();
	if((end - start)/CLOCKS_PER_SEC > uptime && outPath.cost != INT_MAX) {
		if(onemoretime) {
			onemoretime = false;
			goto beginSK;
		}
		for(int i = (int)outPath.edges.size() - 1; i >= 0; i--)
			record_result(outPath.edges[i]);
		return;
	}
	else {
		if(fmin) fmin = false;
		goto beginSK;
	}
}



bool isAns(vector<int> &tour)
{
	int num = 0;
	for(int i = 0 ; i < (int)tour.size(); i++){
		if(lpsolver_spNodesSet[tour[i]] == true)
			num++;
	}
	if(num == lpsolver_specialNum)
		return true;
	else return false;
}


// Given an integer-feasible solution 'sol', find the smallest
// sub-tour.  Result is returned in 'tour', and length is
// returned in 'tourlenP'.

bool
findsubtour(vector<vector<int> > &sol,
            vector<int> &tour)
{
	int from = lpsolver_start;
	int to = lpsolver_terminial;
	vector<int> path;
	path.push_back(from);
	while(from != to){
		int lastFrom = from;
		//cout << from << " ";
		for (int i = 0; i < lpsolver_verNum; ++i)
		{
			if(sol[from][i] != 0){
				tour.push_back(lpsolver_idTable[from][i]);
				from = i;
				path.push_back(from);
				break;
			}
		}
		if(lastFrom == from)
			return false;
	}

	if(isAns(path)){
		//cout << "isAns" << endl;
		return true;
	}
	else{
		//cout << "ans Error" << endl;
		return false;
	}
	
}



/* 得到第一个没有经过的点 */ 
int JudegCycle(bool visited[],const vector<int> &allnode)
{
	for(int i = 0;i < allnode.size();i++)
	{
		if(visited[allnode[i]] == false)
			//if(find(lpsolver_specialNodes.begin(),lpsolver_specialNodes.end(),allnode[i]) != lpsolver_specialNodes.end())
				return allnode[i];
	}
	return -1;
}


/*Judge is right*/
int isAnsRight(vector<vector<int> > &curCycle)
{
	//cout << "94 " << endl;
	vector<int> allNode;
	vector<int> curSpec;
	vector<vector<int> > curRes(lpsolver_verNum,vector<int>(lpsolver_verNum,0));
	int next;
	int jndex = 0;
	for(int i=0;i<lpsolver_verNum;i++)
	{
		for(int j=0;j<lpsolver_verNum;j++)
		{
			if(lpsolver_indexTable[i][j] == -1)
				continue;
			jndex++;
			//assert(jndex == lpsolver_indexTable[i][j]);
			if(get_var_primalresult(lp, get_Norig_rows(lp) + jndex) == 1)
			{
				if(find(allNode.begin(),allNode.end(),i) == allNode.end())
					allNode.push_back(i);
				if(find(allNode.begin(),allNode.end(),j) == allNode.end())
					allNode.push_back(j);
				curRes[i][j] = 1;
			}
		}
	}
	/*cout << " i "<<" j"<<endl;
	for(int i=0;i<lpsolver_verNum;i++)
	{
		for(int j=0;j<lpsolver_verNum;j++)
		{
			if(curRes[i][j] == 1)
				cout << i << "  " << j<<endl;
		}
	}*/
	//cout << "118 " << endl;
	bool visited[lpsolver_verNum + 1];  	//table flag Specnodes
	memset(visited,false,sizeof(visited));
	int flagNext = 0;
	while(1)
	{
		if(flagNext == 0)
		{
			next = lpsolver_start;
			//cout << "lpsolver_start " << lpsolver_start <<endl;
			flagNext = 1;
		}
		else
		{
			next = JudegCycle(visited,allNode);
			//cout << "next " << next << endl;
			if(next == -1)
				break;
		}
		//cout << "135 " << endl;
		//cout << "next "<< next<<endl;
		visited[next] = true;
		vector<int> tmp;
		tmp.push_back(next);

		int firstnum = next;
		//cout << "dddd  " <<  get_var_primalresult(lp, get_Norig_rows(lp) + lpsolver_indexTable[15][0])<<endl;
		while(1)
		{
			int flag = 0;
			int flagCycle= 0;
			for(int i=0;i < lpsolver_verNum;i++)
			{
				if(lpsolver_indexTable[next][i] == -1)
					continue;
				if(get_var_primalresult(lp, get_Norig_rows(lp) + lpsolver_indexTable[next][i]) == 1)
				{
					tmp.push_back(i);
					next = i;
					visited[i] = true;
					flag = 1;
					if(i == firstnum)
					{
						flagCycle = 1;
						break;
					}
				}
			}
			if(flag == 0 || flagCycle == 1)
			{
				//for(int i=0;i<tmp.size();i++)
				//	cout << tmp[i] << "->";
				//cout <<endl;
				curCycle.push_back(tmp);
				break;
			}
		}
	//	cout << "170 " << endl;
	}
	//cout << "172 " << endl; 
	//判断结果是否正确
	int SpecNUM=0;
	for(int i=0;i<curCycle[0].size();i++)
	{
	//	cout << curCycle[0][i] << "- > ";
		if(find(lpsolver_specialNodes.begin(),lpsolver_specialNodes.end(),curCycle[0][i]) != lpsolver_specialNodes.end())
			SpecNUM++;
	}
	//cout << endl << " lpsolver_specialNum " << SpecNUM<<endl;
	if(curCycle[0][0] != lpsolver_start || curCycle[0][curCycle[0].size()-1] != lpsolver_terminial || SpecNUM != lpsolver_specialNodes.size())
		return false;
	else
	{
		vector<int> tour;
		findsubtour(curRes, tour);
		for(int i = 0; i < (int)tour.size(); i++){
			//cout << tour[i] << " ";
			record_result(tour[i]);
		}
		//cout << endl;
		return true;
	}
}


void addConstr(vector<vector<int> > &curCycle)
{
	for(int i=0;i < curCycle.size();i++)
	{
		if(curCycle[i][0] == curCycle[i][curCycle[i].size()-1])  //如果是环
		{
			//cout << "there hava a cycle" << endl;
			//for(int z=0;z<curCycle[i].size();z++)
			//	cout << curCycle[i][z] << "->";
			//cout << endl;

			int colno[Ncol];
			memset(colno, 0, sizeof(colno));
			for(int j = 0; j < Ncol; j++){
				colno[j] = j + 1;
			}
			REAL sparserow[Ncol];
			memset(sparserow, 0, sizeof(sparserow));

			for(int j = 0;j < curCycle[i].size()-1; j++){
				sparserow[lpsolver_indexTable[curCycle[i][j]][curCycle[i][j+1]] - 1] = 1;	
			}
			add_constraintex(lp, Ncol, sparserow, colno, LE, curCycle[i].size()-2);
		}
	}
}

void lpSolver()
{
	Ncol = 0;
	lpsolver_indexTable = vector<vector<int> >(lpsolver_verNum, vector<int>(lpsolver_verNum, -1));

	for(int i = 0; i < lpsolver_verNum; i++){
		for(int j = 0; j < lpsolver_verNum; j++){
			if(lpsolver_weightTable[i][j] == MAX_INT){
				continue;
			}
			++Ncol;
			lpsolver_indexTable[i][j] = Ncol;			
		}
	}

	lp = make_lp(0, Ncol);

	// set constraint
	set_add_rowmode(lp, true); //If TRUE, then add_constraint, add_constraintex, str_add_constraint performs best.
	int colno[Ncol];
	memset(colno, 0, sizeof(colno));
	for(int i = 0; i < Ncol; i++){
		colno[i] = i + 1;
	}
	REAL sparserow[Ncol];

	for(int i = 0; i < lpsolver_verNum; i++){
		if(i == lpsolver_start){
			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[i][j] != -1)
					sparserow[lpsolver_indexTable[i][j] - 1] = 1;
				if(lpsolver_indexTable[j][i] != -1)
					sparserow[lpsolver_indexTable[j][i] - 1] = -1;			
			}
			add_constraintex(lp, Ncol, sparserow, colno, EQ, 1); // outDegree minus inDegree of lpsolver_start be 1

		}
		else if(i == lpsolver_terminial){	
			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[i][j] != -1)
					sparserow[lpsolver_indexTable[i][j] - 1] = 1;
				if(lpsolver_indexTable[j][i] != -1)
					sparserow[lpsolver_indexTable[j][i] - 1] = -1;			
			}
			add_constraintex(lp, Ncol, sparserow, colno, EQ, -1); //  outDegree minus inDegree of target be -1
			
		}
		else{			
			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[i][j] != -1)
					sparserow[lpsolver_indexTable[i][j] - 1] = 1;
				if(lpsolver_indexTable[j][i] != -1)
					sparserow[lpsolver_indexTable[j][i] - 1] = -1;			
			}
			add_constraintex(lp, Ncol, sparserow, colno, EQ, 0); // outDegree minus inDegree of others be 0	
		}	
	}

	for(int i = 0; i < lpsolver_verNum; i++){

		if(i == lpsolver_start || i == lpsolver_terminial)
			continue;
			
		if(lpsolver_spNodesSet[i] == true){
			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[i][j] != -1)
					sparserow[lpsolver_indexTable[i][j] - 1] = 1;		
			}
			add_constraintex(lp, Ncol, sparserow, colno, EQ, 1); // outDegree of special nodes 1

			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[j][i] != -1)
					sparserow[lpsolver_indexTable[j][i] - 1] = 1;		
			}
			add_constraintex(lp, Ncol, sparserow, colno, EQ, 1); // inDegree of special nodes 1
		}
		else{
			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[i][j] != -1)
					sparserow[lpsolver_indexTable[i][j] - 1] = 1;		
			}
			add_constraintex(lp, Ncol, sparserow, colno, LE, 1); // outDegree of every node no more than 1	
			add_constraintex(lp, Ncol, sparserow, colno, GE, 0); // outDegree of every node no more than 1	

			memset(sparserow, 0, sizeof(sparserow));
			for(int j = 0; j < lpsolver_verNum; j++){
				if(lpsolver_indexTable[j][i] != -1)
					sparserow[lpsolver_indexTable[j][i] - 1] = 1;		
			}
			add_constraintex(lp, Ncol, sparserow, colno, LE, 1); // inDegree of every node no more than 1	
			add_constraintex(lp, Ncol, sparserow, colno, GE, 0); // inDegree of every node no more than 1			
		}
	}
	
	// set obj
    set_add_rowmode(lp, FALSE);
    int objNum = 0;
    for(int i = 0; i < lpsolver_verNum; i++){
		for(int j = 0; j < lpsolver_verNum; j++){
			if(lpsolver_indexTable[i][j] != -1){
				++objNum;
				//assert(lpsolver_indexTable[i][j] == objNum);
				set_binary(lp, objNum, TRUE);
				set_obj(lp, objNum, lpsolver_weightTable[i][j]);
			}			
		}
	}
	//assert(objNum == Ncol);

	// solve 
	int ret = -1;
	ret = solve(lp);
	//cout << "ret " << ret  << endl;
	vector<int> tour;
	vector<vector<REAL> > resultTable(Ncol, vector<REAL>(Ncol, false));
	if(ret == 0)
	{	// OPTIMAL (0)
    	while(1)
    	{
	    	vector<vector<int> > curCycle;
	    	if(isAnsRight(curCycle))
	    	{
	    		
	    		break;
	    	}
	    	else
	    	{
	    		//cout << " 347 " << endl;
	    		addConstr(curCycle);
	    		solve(lp);
		    }
	    }
	}
}



void initGraph(char * topo[5000], int edge_num, char * demand)
{
	lpsolver_verNum = 0;
	lpsolver_specialNum = 0;

	int id, from, to, dist;
	char tmp;
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> id >> tmp >> from >> tmp >> to >> tmp >> dist;

		if (from >= lpsolver_verNum)
			lpsolver_verNum = from + 1;
		if (to >= lpsolver_verNum)
			lpsolver_verNum = to + 1;
	}

	lpsolver_weightTable = vector<vector<int> >(lpsolver_verNum, vector<int>(lpsolver_verNum, MAX_INT));
	lpsolver_idTable = vector<vector<int> >(lpsolver_verNum, vector<int>(lpsolver_verNum, -1));
	
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> id >> tmp >> from >> tmp >> to >> tmp >> dist;
		
		lpsolver_weightTable[from][to] = dist;
		lpsolver_idTable[from][to] = id;
	}

	lpsolver_spNodesSet = vector<bool>(lpsolver_verNum, false);
	stringstream ss;
	ss << demand;
	ss >> lpsolver_start >> tmp >> lpsolver_terminial >> tmp;
	do {
		ss >> from;
		lpsolver_specialNodes.push_back(from);
		lpsolver_specialNum++;
		lpsolver_spNodesSet[from] = true;
	} while (ss >> tmp);
}

void toString()
{
	cout << "lpsolver_verNum : " << lpsolver_verNum << endl;					//所有顶点的数目
	cout << "lpsolver_specialNum : " << lpsolver_specialNum << endl;;				//必经节点的数目
	cout << "lpsolver_start : " << lpsolver_start << endl;					//起点
	cout << "lpsolver_terminial : " << lpsolver_terminial << endl;				//终点
	cout << "lpsolver_specialNodes :";

	//assert(lpsolver_specialNum == (int)lpsolver_specialNodes.size());

	for (int i = 0; i < lpsolver_specialNum; i++)	//必经点集合
	{
		cout << " " << lpsolver_specialNodes[i];
	}
	cout << endl;

	cout << "edgeTable : " << endl;		//存储图的邻接表
	for (int i = 0; i < lpsolver_verNum; i++)
	{
		for (int j = 0; j < lpsolver_verNum; j++)
		{
			cout << lpsolver_idTable[i][j] << " : " << lpsolver_weightTable[i][j] << " | ";
		}
		cout << endl;
	}
}



