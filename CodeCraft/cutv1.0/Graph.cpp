#include "Graph.h"

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
