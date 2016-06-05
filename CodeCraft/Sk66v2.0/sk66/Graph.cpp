#include "Graph.h"

Graph::Graph(char ** topo, int edg_num, char * demand)
{
	graph = vector<vector<Edge>>(600);	//����ͼ��ʼ��
	vertexNum = 0;	//��������ʼ��
	demandNum = 0; //�ؾ����ʼ��
	
	//�����
	for (int i = 0; i < edg_num; i++)	
	{
		istringstream input(topo[i]);	//һ������
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
	++vertexNum;	//������ = ����������� + 1��


	// ����ؾ���
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
