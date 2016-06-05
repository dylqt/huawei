#include "Graph.h"

Graph::Graph(char ** topo, int edg_num, char * demand)
{
	graph = vector<vector<Edge>>(600);	//连接图初始化
	vertexNum = 0;	//顶点数初始化
	demandNum = 0; //必经点初始化
	
	//处理表
	for (int i = 0; i < edg_num; i++)	
	{
		istringstream input(topo[i]);	//一行数据
		int id;
		int from;
		int to;
		int cost;
		input >> id;
		input.ignore(10, ',');
		input >> from;
		input.ignore(10, ',');
		input >> to;
		input.ignore(10, ',');
		input >> cost;
		Edge edge(id, from, to, cost);
		addEdge(edge);
	}
	++vertexNum;	//顶点数 = 起点的最大索引 + 1；


	// 处理必经点
	istringstream inputDemand(demand);
	inputDemand >> s;
	inputDemand.ignore(10, ',');
	inputDemand >> t;
	inputDemand.ignore(10, ',');
	int demandV = 0;
	while (inputDemand >> demandV)
	{
		demandVertexSet.push_back(demandV);
		inputDemand.ignore(10, '|');
		++demandNum;
	}
}

void Graph::toString()
{
	cout << vertexNum << endl;
	for (int i = 0; i < vertexNum; i++)
	{
		cout << i << " :";
		for (auto s : graph[i])
		{
			cout << " " << s.getID() << " " << s.getFrom() <<
				" " << s.getTo() << " " << s.getCost() << endl;
		}
		cout << endl;
	}

	cout << "s : " << s << endl;
	cout << "t : " << t << endl;
	cout << "demand Number : " << demandNum << endl;
	for (auto s : demandVertexSet)
	{
		cout << " " << s;
	}
	cout << endl;
}
