#include "Graph.h"

diGraph::diGraph(ifstream & is)
{
	string test;
	getline(is, test);
	vertex = stoi(test);
	getline(is, test);
	edges = stoi(test);
	graph = vector<vector<Edge>>(vertex);
	while (getline(is, test))
	{
		istringstream line(test);
		string v, w, weight;
		line >> v >> w >> weight;
		Edge edgeTmp(stoi(v), stoi(w), stoi(weight));
		graph[edgeTmp.getFrom()].push_back(edgeTmp);
	}
}

void diGraph::addEdge(int from, int to, int weight)
{
	Edge tmp(from, to, weight);
	graph[from].push_back(tmp);
}

void diGraph::toString()
{
	for (int i = 0; i < graph.size(); i++)
	{
		cout << i << " :";
		for (auto j : graph[i])
		{
			cout << " (" << j.getTo() << " " << j.getWeight() << ")";
		}
		cout << endl;
	}
}

bool operator< (const Edge &lhs, const Edge &rhs)
{
	if (lhs.getWeight() > rhs.getWeight())
	{
		return true;
	}
	else
		return false;
}
