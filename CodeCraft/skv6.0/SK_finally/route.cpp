#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <assert.h>

#define INSERTSP
Path &Path::operator=(const Path &rhs)
{
	cost = rhs.cost;
	origin = rhs.origin;
	target = rhs.target;
	nodes = rhs.nodes;
	edges = rhs.edges;
	specialNodes = rhs.specialNodes;
	freeNodes = rhs.freeNodes;

	return *this;
}

/* Path 的加号重载（左边加上右边） */
Path Path::operator+(const Path &rhs)
{
	Path sum = *this;
	sum.cost = this->cost + rhs.cost;

	sum.target = rhs.target;					//左边的终点改成右边的终点
	for (int i = 0; i < rhs.edges.size(); i++)  //边全部正序存储
		sum.edges.push_back(rhs.edges[i]);

	for (int i = 1; i < rhs.nodes.size(); i++)		//顶点不添加右边的起点，防止起点重复
		sum.nodes.push_back(rhs.nodes[i]);




	for (int i = 0; i < rhs.specialNodes.size(); i++)   //合并路径中的必经节点
	{
		if (find(sum.specialNodes.begin(), sum.specialNodes.end(), rhs.specialNodes[i]) == sum.specialNodes.end())
			sum.specialNodes.push_back(rhs.specialNodes[i]);
	}

	for (int i = 0; i < rhs.freeNodes.size(); i++)		//合并路径中的自由节点
	{
		if (find(sum.freeNodes.begin(), sum.freeNodes.end(), rhs.freeNodes[i]) == sum.freeNodes.end())
			sum.freeNodes.push_back(rhs.freeNodes[i]);
	}

	return sum;
}

/*打印当前路径*/
void printPath(Path &path)
{
	cout << "Path: " << path.origin << " -> " << path.target << " cost: " << path.cost << endl;

	/*输出路径中的边*/
	cout << "path's edges :  ";
	for (int i = 0; i < path.edges.size(); i++) {
		cout << path.edges[i] << " ";
	}
	cout << endl;
	/*输出路径中的顶点*/
	cout << "path's nodes : ";
	for (int i = 0; i < path.nodes.size(); i++) {
		cout << path.nodes[i] << " ";

	}
	cout << endl;
	/*输出路径中的指定顶点*/
	cout << "Path's SpecNodes : ";
	for (int i = 0; i < path.specialNodes.size(); i++) {
		cout << path.specialNodes[i] << " ";

	}
	cout << endl;
	/*判断路径是否有环*/
	cout << "Path's FreeNodes : ";
	for (int i = 0; i < path.freeNodes.size(); i++)
		cout << path.freeNodes[i] << " ";
	cout << endl;

}

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	Graph deal;
	deal.initGraph(topo, edge_num, demand);
	//deal.toString();
	deal.initSK();
	/*
	for (int i = 0; i < deal.verNum; i++)
	{
		for (int j = 0; j < deal.verNum; j++)
		{
			cout << deal.cost[i][j] << " ";
		}
		cout << endl;
	}
	*/

	
	/*for (int i = 0; i < deal.specialNum; i++)
	{
		int vl = deal.specialNodes[i];
		printPath(deal.D[deal.start][vl]);
	}
	for (int i = 0; i < deal.specialNum; i++)
	{
		int vi = deal.specialNodes[i];
		for (int j = 0; j < deal.specialNum; j++)
		{
			int vl = deal.specialNodes[j];
			printPath(deal.D[vi][vl]);
		}
	}
	for (int i = 0; i < deal.specialNum; i++)
	{
		int vl = deal.specialNodes[i];
		printPath(deal.D[vl][deal.terminial]);
	}
	*/
	deal.doSK();
	/*printPath(deal.D[3][17]);
	printPath(deal.D[17][19]);
	printPath(deal.D[3][17] + deal.D[17][19]);
	*/
	int minCost = MAX_INT;
	int  res = -1;
//	for (int i = 0; i < deal.result.size(); i++)
//	{
//		if(pathIsAns(deal.result[i], deal) == true)
//			printPath(deal.result[i]);
//	}
	for (int i = 0; i < deal.result.size(); i++)
	{
		if (pathIsAns(deal.result[i],deal) == false)
			continue;
		if (deal.result[i].cost < minCost)
		{
			minCost = deal.result[i].cost;
			res = i;
		}
	}
	if(res != -1)
	{
		for(int i=0; i < deal.result[res].edges.size(); i++)
			record_result(deal.result[res].edges[i]);
	}
		//printPath(deal.result[res]);
	else
		cout << "no path"<<endl;
		

		//unsigned short result[] = { 2, 6, 3 };//示例中的一个解

		//for (int i = 0; i < 3; i++)
		//	record_result(result[i]);
}

void Graph::initGraph(char * topo[5000], int edge_num, char * demand)
{
	verNum = 0;
	specialNum = 0;
	edgeTable = vector<vector<Edge> >(MAX_NODES);

	int index, from, to, dist;
	char tmp;
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> index >> tmp >> from >> tmp >> to >> tmp >> dist;
		Edge newEdge(index, from, to, dist);
		edgeTable[from].push_back(newEdge);

		if (from >= verNum)
			verNum = from + 1;
		if (to >= verNum)
			verNum = to + 1;
	}

	stringstream ss;
	ss << demand;
	ss >> this->start >> tmp >> this->terminial >> tmp;
	do {
		ss >> from;
		specialNodes.push_back(from);
		specialNum++;
	} while (ss >> tmp);
}

void Graph::toString()
{
	cout << "verNum : " << verNum << endl;					//所有顶点的数目
	cout << "specialNum : " << specialNum << endl;;				//必经节点的数目
	cout << "start : " << start << endl;					//起点
	cout << "terminial : " << terminial << endl;				//终点

	cout << "specialNodes :";
	for (int i = 0; i < specialNodes.size(); i++)	//必经点集合
	{
		cout << " " << specialNodes[i];
	}
	cout << endl;

	cout << "edgeTable : " << endl;		//存储图的邻接表
	for (int i = 0; i < verNum; i++)
	{
		cout << i << " : ";
		for (int j = 0; j < edgeTable[i].size(); j++)
		{
			cout << edgeTable[i][j].id << " " << edgeTable[i][j].from << " "
				<< edgeTable[i][j].to << " " << edgeTable[i][j].weight << " | ";
		}
		cout << endl;
	}
}

void Graph::Dijstra(int s, vector<int>& rmNodes)
{
	edgeTo = vector<Edge>(verNum);		//初始化与终点相连的边；下标是终点；内容是指向它的边
	dist = vector<int>(verNum, MAX_INT); 		//初始化d数组
	dist[s] = 0;

	visited = vector<bool>(verNum, false);
	for (int i = 0; i < (int)rmNodes.size(); i++)	//把移除的节点标记为已经完成
		visited[rmNodes[i]] = true;

	priority_queue<PriorityNode> Q;
	Q.push(PriorityNode(s, 0));				//把起点加入队列

	while (!Q.empty()) {
		PriorityNode x = Q.top();
		Q.pop();
		int index = x.vIndex;
		if (visited[index])
			continue;
		visited[index] = true;

		for (unsigned int i = 0; i < edgeTable[index].size(); i++) {	//	遍历节点u的所有边
			Edge e = edgeTable[index][i];
			if (dist[e.to] > dist[index] + e.weight) {
				dist[e.to] = dist[index] + e.weight;
				edgeTo[e.to] = e;
				Q.push(PriorityNode(e.to, dist[e.to]));
			}
		}
	}
}

Path Graph::bulidPath(int s, int t)
{
	Path newPath;

	newPath.origin = s;
	newPath.target = t;
	newPath.cost = MAX_INT;
	newPath.edges.clear();
	newPath.nodes.clear();
	newPath.specialNodes.clear();
	newPath.freeNodes.clear();

	if (dist[t] != MAX_INT && t != s)
	{
		Edge rhs = edgeTo[t];
		newPath.cost = 0;
		newPath.nodes.push_back(rhs.to);
		if (find(specialNodes.begin(), specialNodes.end(), rhs.to) != specialNodes.end())
			newPath.specialNodes.push_back(rhs.to);
		else
			newPath.freeNodes.push_back(rhs.to);

		while (rhs.from != s) {
			newPath.cost += rhs.weight;
			newPath.edges.push_back(rhs.id);
			newPath.nodes.push_back(rhs.from);
			if (find(specialNodes.begin(), specialNodes.end(), rhs.from) != specialNodes.end())
				newPath.specialNodes.push_back(rhs.from);
			else
				newPath.freeNodes.push_back(rhs.from);
			rhs = edgeTo[rhs.from];
		}

		newPath.cost += rhs.weight;
		newPath.edges.push_back(rhs.id);
		newPath.nodes.push_back(rhs.from);
		if (find(specialNodes.begin(), specialNodes.end(), rhs.from) != specialNodes.end()) //cbegin()?
			newPath.specialNodes.push_back(rhs.from);
		else
			newPath.freeNodes.push_back(rhs.from);
	}
	reverse(newPath.edges.begin(), newPath.edges.end());
	reverse(newPath.nodes.begin(), newPath.nodes.end());
	return newPath;
}

void Graph::initSK()
{
	/* cost 数组的初始化 */
	cost = vector<vector<int> >(verNum, vector<int>(verNum, MAX_INT));
	for (int i = 0; i < verNum; i++)
	{
		for (int j = 0; j < edgeTable[i].size(); j++)
		{
			if (edgeTable[i][j].weight < cost[edgeTable[i][j].from][edgeTable[i][j].to])
				cost[edgeTable[i][j].from][edgeTable[i][j].to] = edgeTable[i][j].weight;
		}
	}

	D = vector<vector<Path> >(verNum, vector<Path>(verNum));
	/*迭代保存初始化*/
	//cout << "F start"<<endl;
	F = vector<vector<vector<Path> > >(verNum, vector<vector<Path> >(verNum, vector<Path>(verNum)));
	/*F = new Path **[specialNum];
	for (int i = 0; i <= specialNum; i++)
		*(F + i) = new Path*[verNum];

	for (int i = 0; i <= specialNum; i++)
	{
		for (int j = 0; j < verNum; j++)
			*(*(F + i) + j) = new Path[verNum];
	}*/
	//	cout << "F end"<<endl;

		/*构建D*/
	vector<int> noVisited;
	noVisited.push_back(terminial);
	Dijstra(start, noVisited);
	for (int i = 0; i < specialNum; i++)
	{
		D[start][specialNodes[i]] = bulidPath(start, specialNodes[i]);
		//printPath(D[start][specialNodes[i]]);
	}

	noVisited.clear();
	noVisited.push_back(start);
	noVisited.push_back(terminial);
	for (int i = 0; i < specialNum; i++)
	{
		Dijstra(specialNodes[i], noVisited);
		for (int j = 0; j < specialNum; j++)
		{
			//if (i == j)
				//continue;	//no need to continue; if equal,the result cost will be MAX_INT
			D[specialNodes[i]][specialNodes[j]] = bulidPath(specialNodes[i], specialNodes[j]);
		}
	}

	noVisited.clear();
	noVisited.push_back(start);
	for (int i = 0; i < specialNum; i++)
	{
		Dijstra(specialNodes[i], noVisited);
		D[specialNodes[i]][terminial] = bulidPath(specialNodes[i], terminial);
	}


}

void Graph::doSK()
{
	//求解f0；
	vector<Path> F0(verNum);
	for (int i = 0; i < specialNum; i++)
	{
		F0[specialNodes[i]] = D[specialNodes[i]][terminial];
		//printPath(F0[specialNodes[i]]);
	}
	//求解f1；
	for (int i = 0; i < specialNum; i++)
	{
		int vi = specialNodes[i];
		for (int j = 0; j < specialNum; j++)
		{
			int vl = specialNodes[j];
			if (vi == vl)
				continue;
			Path tmpPath;
			if (D[vi][vl].cost < MAX_INT && F0[vl].cost < MAX_INT)
			{
				tmpPath = handleCycle(D[vi][vl], F0[vl]);  //处理环,拆开D(vi, vl);

				if (tmpPath.cost < MAX_INT)	//处理环成功
					F[1][vi][vl] = tmpPath;
				else									   //不存在这种路径,拆开F0
				{
					vector<int> noVisited;
					noVisited.push_back(start);
					for (int i = 0; i < D[vi][vl].nodes.size(); i++)
					{
						if (D[vi][vl].nodes[i] != vl && D[vi][vl].nodes[i] != terminial)
							noVisited.push_back(D[vi][vl].nodes[i]);
					}

					Dijstra(vl, noVisited);
					if (bulidPath(vl, terminial).cost < MAX_INT)
						F[1][vi][vl] = D[vi][vl] + bulidPath(vl, terminial);
					else
						F[1][vi][vl].cost = MAX_INT;
					//printPath(F[1][vi][vl]);
					//cout << "--------------------------" << endl;
				}
			}
			else
				F[1][vi][vl].cost = MAX_INT;
//try1			if (F[1][vi][vl].cost < MAX_INT && (int)F[1][vi][vl].specialNodes.size() - 1 < 1)
//				F[1][vi][vl].cost = MAX_INT; //the path is not admissible
		}
	}

	//从2迭代到vs
	for (int n = 2; n <= specialNum; n++)
	{

		for (int i = 0; i < specialNum; i++)
		{
			int vi = specialNodes[i];
			int vl = 0;
			int vj = 0;
			for (int j = 0; j < specialNum; j++)
			{
				vl = specialNodes[j];
				if (vi == vl)
					continue;
				//循环找minf(n - 1)(vl, vj);
				int minFcost = MAX_INT;
				Path minF;
				if (D[vi][vl].cost < MAX_INT)	// 3.23 add
				{
					for (int k = 0; k < specialNum; k++)
					{
						vj = specialNodes[k];
						if (vj == vl || vi == vj) //不允许vi = vj, vl = vj
							continue;
						int tmpFcost = F[n - 1][vl][vj].cost;
						if (tmpFcost < minFcost)
						{
							minFcost = tmpFcost;
							minF = F[n - 1][vl][vj];
						}
					}
				}
				else
				{
					F[n][vi][vl].cost = MAX_INT;
					continue;
				}
				/* if minF exist */
				if (minF.cost < MAX_INT) {
					//D(vi, vl)是否存在f(n - 1)(vl, vj)中的点, 如果存在就处理它
					F[n][vi][vl] = handleCycle(D[vi][vl], minF);

					if (F[n][vi][vl].specialNodes.size() < n)
						F[n][vi][vl].cost = MAX_INT;
					//条件二
					if (F[n][vi][vl].cost >= MAX_INT && F[n - 1][vi][vl].specialNodes.size() >= n)
						F[n][vi][vl] = F[n - 1][vi][vl];
					//条件三
					if ((int)F[n][vi][vl].specialNodes.size() >= n &&
						(int)F[n - 1][vi][vl].specialNodes.size() == (int)F[n][vi][vl].specialNodes.size() && 	//3.23 add
						F[n - 1][vi][vl].cost < F[n][vi][vl].cost)
						F[n][vi][vl] = F[n - 1][vi][vl];
					//cout << "f" << n << " v" << vi << " v" << vl << " : ";
					//printPath(F[n][vi][vl]);
					//cout << "-------------------" << endl;
				}
				else if (F[n - 1][vi][vl].specialNodes.size() >= n)	// minF not exist
					F[n][vi][vl] = F[n - 1][vi][vl];
				else
					F[n][vi][vl].cost = MAX_INT;
			}
		}
		/************************************************************************/
		/* 试验1                                                                     */
		/************************************************************************/
		//做完一次迭代后，检查一次fn，查看是否还存在路径，如果全部没有路径，则进行强插
		insertNoWay(n);
		//insertKeepN(n);


	}

	//计算从s开始的点

	for (int i = 0; i < specialNum; i++)
	{
		int minCost = MAX_INT;
		int vi = specialNodes[i];
		Path minTmp;
		for (int j = 0; j < specialNum; j++)
		{
			int vl = specialNodes[j];
			if (vi == vl)
				continue;

			if (D[start][vi].cost + F[specialNum][vi][vl].cost < minCost)
			{
				minCost = D[start][vi].cost + F[specialNum][vi][vl].cost;
				minTmp = F[specialNum][vi][vl];
			}

		}
		//printPath(minTmp);
		if (minCost < MAX_INT)
		{
			result.push_back(handleCycle(D[start][vi], minTmp));
		}

	}
}

Path Graph::handleCycle(Path & lhsD, Path & rhsF)
{
	Path newPath;
	newPath.origin = lhsD.origin;
	newPath.target = rhsF.target;
	vector<int> noVisited;
	bool hasCycle = false;
	//如果lhsD即D(vi, vl)不存在，或者lhsD的起点在rhsF中，则对D(vi, vl)再怎么处理也不可能解决环
	if (lhsD.cost >= MAX_INT ||
		find(rhsF.nodes.begin(), rhsF.nodes.end(), lhsD.nodes[0]) != rhsF.nodes.end())
	{
		newPath.cost = MAX_INT;
		return newPath;
	}
	//查询是否有环
	for (int i = 0; i < lhsD.nodes.size() - 1; i++)
	{
		if (find(rhsF.nodes.begin(), rhsF.nodes.end(), lhsD.nodes[i]) != rhsF.nodes.end())
		{
			hasCycle = true;
			break;
		}
	}

	if (hasCycle)  //如果有环
	{
		for (int i = 1; i < rhsF.nodes.size(); i++)
		{
			if (rhsF.nodes[i] != lhsD.target)	//D(vi, vl)的终点必须被访问的
				noVisited.push_back(rhsF.nodes[i]);
		}
		Dijstra(lhsD.origin, noVisited);	//重新对D这一边做Dij
		if (bulidPath(lhsD.origin, lhsD.target).cost == MAX_INT)	//存在环，且重新寻找后找不到路径
			newPath.cost = MAX_INT;
		else
			newPath = bulidPath(lhsD.origin, lhsD.target) + rhsF;
	}
	else
		newPath = lhsD + rhsF;

	return newPath;
}

void Graph::insertNoWay(int n)
{
	//cout << n << "------------" << endl;
	bool pathExist = false;
	for (int i = 0; i < specialNum; i++)
	{
		int vi = specialNodes[i];
		for (int j = 0; j < specialNum; j++)
		{
			int vl = specialNodes[j];
			if (vi == vl)
				continue;
			if (F[n][vi][vl].cost < MAX_INT)
			{
				pathExist = true;
				break;
			}
		}
		if (pathExist == true)
			break;
	}
	// 完全不存在路径，必须强插
	if (pathExist == false)
	{
		for (int i = 0; i < specialNum; i++)
		{
			int vi = specialNodes[i];
			for (int j = 0; j < specialNum; j++)
			{
				int vl = specialNodes[j];
				if (vi == vl)
					continue;
#ifdef INSERTALL
				if (insertNodeAll(F[n - 1][vi][vl]) == true)
					F[n][vi][vl] = F[n - 1][vi][vl];
#endif // INSERTALL

#ifdef INSERTSP
				F[n][vi][vl] = insertNodeSp(F[n - 1][vi][vl]);
#endif // INSERTSP

			}
		}
	}
}

void Graph::insertKeepN(int n)
{
	//cout << n << "---------------" << endl;
	for (int i = 0; i < specialNum; i++)
	{
		int vi = specialNodes[i];
		for (int j = 0; j < specialNum; j++)
		{
			int vl = specialNodes[j];
			if (vi == vl)
				continue;

			//cout << n << endl;
			//cout << "from" << endl;
			//printPath(F[n][vi][vl]);
			if (F[n][vi][vl].cost >= MAX_INT)
			{
#ifdef INSERTALL
				//assert(F[n - 1][vi][vl].cost < MAX_INT);
				if (F[n - 1][vi][vl].cost < MAX_INT && insertNodeAll(F[n - 1][vi][vl]))	//不存在边
					F[n][vi][vl] = F[n - 1][vi][vl];
#endif // INSERTALL

				
#ifdef INSERTSP
				if (F[n - 1][vi][vl].cost < MAX_INT)	//不存在边
					F[n][vi][vl] = insertNodeSp(F[n - 1][vi][vl]);
#endif // INSERTSP

			}
			else if (F[n][vi][vl].nodes.size() < n)
			{
#ifdef INSERTALL
				if (insertNodeAll(F[n][vi][vl]))	//必经点数目不够
					F[n][vi][vl] = F[n][vi][vl];
#endif // INSERTALL
#ifdef INSERTSP
				F[n][vi][vl] = insertNodeSp(F[n][vi][vl]);
#endif // INSERTSP
	
			}
		}
	}
}

bool Graph::insertNodeAll(Path &path)
{
	//printPath(path);
	for (int i = 0; i < specialNum; i++) //¿¿¿¿¿¿¿¿¿
	{
		if (find(path.specialNodes.begin(), path.specialNodes.end(), specialNodes[i]) == path.specialNodes.end()) //¿¿¿¿¿¿
		{
			for (int j = 0; j < path.nodes.size() - 1; j++)   //¿¿¿¿¿¿¿¿¿
			{
				int s, t;   //start,terminial

				s = path.nodes[j];
				t = path.nodes[j + 1];

				vector<int> rmNodes;			//¿¿¿¿¿¿¿¿¿¿¿¿
				Path tmp1, tmp2;					//2¿¿¿

				rmNodes.clear();
				rmNodes.push_back(start);		//¿¿¿¿¿¿¿¿
				rmNodes.push_back(terminial);

				for (int isp = 0; isp < (int)path.specialNodes.size(); isp++)  //rmNodes¿¿¿¿¿¿¿¿¿¿¿
				{
					if (path.specialNodes[isp] != s)
						rmNodes.push_back(path.specialNodes[isp]);
				}
				for (int ife = 0; ife < (int)path.freeNodes.size(); ife++)
				{
					if (path.freeNodes[ife] != s)
						rmNodes.push_back(path.freeNodes[ife]);
				}

				Dijstra(s, rmNodes);				//¿¿¿¿¿¿¿
				tmp1 = bulidPath(s, specialNodes[i]);
				if (tmp1.cost == MAX_INT)			//¿¿¿¿¿¿¿¿¿ ¿¿¿¿¿¿¿
					continue;

				//¿¿¿¿¿¿¿¿
				rmNodes.clear();
				rmNodes.push_back(s);
				if (t != terminial)
					rmNodes.push_back(terminial);

				for (int isp = 0; isp < (int)path.specialNodes.size(); isp++)  //rmNodes¿¿¿¿¿¿¿¿¿¿¿
				{
					if (path.specialNodes[isp] != t)
						rmNodes.push_back(path.specialNodes[isp]);
				}
				for (int ife = 0; ife < (int)path.freeNodes.size(); ife++)
				{
					if (path.freeNodes[ife] != t)
						rmNodes.push_back(path.freeNodes[ife]);
				}
				for (int itm = 0; itm < (int)tmp1.freeNodes.size(); itm++)	//¿¿¿¿1¿¿¿¿¿¿
					rmNodes.push_back(tmp1.freeNodes[itm]);

				Dijstra(specialNodes[i], rmNodes);					//¿¿¿2¿
				tmp2 = bulidPath(specialNodes[i], t);

				if (tmp1.cost != MAX_INT && tmp2.cost != MAX_INT)  //¿¿¿¿
				{

					path.cost += tmp1.cost + tmp2.cost;				//¿¿¿¿
					path.specialNodes.push_back(specialNodes[i]);	//¿¿¿¿¿¿¿¿¿

					for (int ife = 0; ife < (int)tmp1.freeNodes.size(); ife++)  //¿¿¿¿¿¿
						path.freeNodes.push_back(tmp1.freeNodes[ife]);
					for (int ife = 0; ife < (int)tmp2.freeNodes.size(); ife++)  //¿¿¿¿¿¿
						path.freeNodes.push_back(tmp2.freeNodes[ife]);

					//更新nodes和边
					Path newPath = tmp1 + tmp2; //先合并2条子路径
					vector<int> tmpNodes;
					vector<int> tmpEdges;
					for (int tN = 0; tN < (int)path.nodes.size(); tN++)
					{
						if (tN != j)
							tmpNodes.push_back(path.nodes[tN]);
						else
						{
							for (int tN1 = 0; tN1 < (int)newPath.nodes.size() - 1; tN1++)
								tmpNodes.push_back(newPath.nodes[tN1]);
						}

					}

					for (int tE = 0; tE < (int)path.edges.size(); tE++)
					{
						if (tE != j)
							tmpEdges.push_back(path.edges[tE]);
						else
						{
							for (int tE1 = 0; tE1 < (int)newPath.edges.size(); tE1++)
								tmpEdges.push_back(newPath.edges[tE1]);
						}
					}

					path.nodes = tmpNodes;
					path.edges = tmpEdges;
					return true;          //¿¿
				}
				else				//¿¿¿¿¿¿¿¿¿¿¿¿¿
					continue;
			}
		}
		else  //¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
			continue;
	}
	return false;
}

Path Graph::insertNodeSp(Path & path)
{
	Path newPath;
	newPath.cost = MAX_INT;
	for (int i = 0; i < specialNum; i++)
	{
		if (find(path.specialNodes.begin(), path.specialNodes.end(), specialNodes[i]) == path.specialNodes.end())	//找到不存在的必经点
		{
			int insertSpNode = specialNodes[i];
			int fromIndexInsert = -1;
			int toIndexInsert = -1;
			
			//找到尝试插入的开始必经点和结束必经点
			for (int j = 0; j < path.nodes.size(); j++)
			{
				if (find(path.specialNodes.begin(), path.specialNodes.end(), path.nodes[j]) == path.specialNodes.end())
					continue;
				else
				{
					fromIndexInsert = j;
					for (int k = j + 1; k < path.nodes.size(); k++)
					{
						if (find(path.specialNodes.begin(), path.specialNodes.end(), path.nodes[k]) == path.specialNodes.end())
							continue;
						else
						{
							toIndexInsert = k;
							break;
						}
					}
				}
				//没有可以插入的点，则跳出
				if (toIndexInsert == -1 || fromIndexInsert == -1)
					break;

				Path lhsPath;
				Path rhsPath;
				Path addPath;
				
				/* 计算路径一 */

				//把不允许经过的点加入到数组中
				rmNodes.clear();
				for (int i = 0; i < fromIndexInsert; i++)
					rmNodes.push_back(path.nodes[i]);
				for (int i = path.nodes.size() - 1; i >= toIndexInsert; i--)
					rmNodes.push_back(path.nodes[i]);
				
				

				Dijstra(path.nodes[fromIndexInsert], rmNodes);
				lhsPath = bulidPath(path.nodes[fromIndexInsert], insertSpNode);

				if (lhsPath.cost >= MAX_INT)
					continue;

			
				/* 计算路径二 */
				rmNodes.pop_back();	//toIndexInsert 必须经过；
				for (int i = 0; i < lhsPath.nodes.size() - 1; i++)
					rmNodes.push_back(lhsPath.nodes[i]);

				Dijstra(insertSpNode, rmNodes);
				rhsPath = bulidPath(insertSpNode, path.nodes[toIndexInsert]);


				if (rhsPath.cost >= MAX_INT)
					continue;

				/* 更新newPath的路径 */
				addPath = lhsPath + rhsPath;
				newPath.origin = path.origin;
				newPath.target = path.target;
				//cost
				newPath.cost = 0;
				newPath.cost += addPath.cost;
				for (int i = 0; i < fromIndexInsert; i++)
					newPath.cost += cost[path.nodes[i]][path.nodes[i + 1]];
				for (int i = toIndexInsert; i < path.nodes.size() - 1; i++)
					newPath.cost += cost[path.nodes[i]][path.nodes[i + 1]];
				//edge & nodes & sp & free
				for (int i = 0; i < fromIndexInsert; i++)
				{
					newPath.edges.push_back(path.edges[i]);
					newPath.nodes.push_back(path.nodes[i]);
					if (find(specialNodes.begin(), specialNodes.end(), path.nodes[i]) != specialNodes.end())
						newPath.specialNodes.push_back(path.nodes[i]);
					else
						newPath.freeNodes.push_back(path.nodes[i]);
				}

				
				for (int i = 0; i < addPath.nodes.size() - 1; i++)
				{
					newPath.edges.push_back(addPath.edges[i]);
					newPath.nodes.push_back(addPath.nodes[i]);
					if (find(specialNodes.begin(), specialNodes.end(), addPath.nodes[i]) != specialNodes.end())
						newPath.specialNodes.push_back(addPath.nodes[i]);
					else
						newPath.freeNodes.push_back(addPath.nodes[i]);
				}
				
				for (int i = toIndexInsert; i < path.nodes.size() - 1; i++)
				{
					newPath.edges.push_back(path.edges[i]);
					newPath.nodes.push_back(path.nodes[i]);
					if (find(specialNodes.begin(), specialNodes.end(), path.nodes[i]) != specialNodes.end())
						newPath.specialNodes.push_back(path.nodes[i]);
					else
						newPath.freeNodes.push_back(path.nodes[i]);
				}
				newPath.nodes.push_back(path.target);
				if (find(specialNodes.begin(), specialNodes.end(), path.target) != specialNodes.end())
					newPath.specialNodes.push_back(path.target);
				else
					newPath.freeNodes.push_back(path.target);

				if (newPath.cost > 0 && newPath.cost < MAX_INT)
					return newPath;
			}
		}
		
	}
	return newPath;
}




/*判断结果正否正确*/
bool pathIsAns(Path &path, Graph &g)
{
	vector<int> visited(g.verNum);
	for (int i = 0; i < path.nodes.size(); i++)
	{
		if (visited[path.nodes[i]] == 0)
			visited[path.nodes[i]] = 1;
		else
		{
			cout << path.nodes[i] << " for two times " << endl;
			return false;
		}
	}

	if (path.specialNodes.size() != g.specialNum)
	{
		cout << "spcialNodesNum error" << endl;
		return false;
	}
	return true;
}

