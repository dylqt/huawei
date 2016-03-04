#include "EdgeWeightedGraph.h"

EdgeWeightedGraph::EdgeWeightedGraph(int v)
{
	vertex = v;
	edges = 0;
}

EdgeWeightedGraph::EdgeWeightedGraph(ifstream & is)
{
	string test;
	getline(is, test);
	vertex = stoi(test);
	getline(is, test);
	edges = stoi(test);
	adj = vector<vector<Edge>> (vertex);
	while (getline(is, test))
	{
		istringstream line(test);
		string v, w, weight;
		line >> v >> w >> weight;
		Edge edgeTmp(stoi(v), stoi(w), stod(weight));
		adj[stoi(v)].push_back(edgeTmp);
		adj[stoi(w)].push_back(edgeTmp);
	}
}

void EdgeWeightedGraph::toString()
{
	for (int i = 0; i < adj.size(); i++)
	{
		cout << i << " :";
		for (auto w : adj[i])
		{
			cout << " |" << w.V() << " " << w.W() << " " << w.getWeight();
		}
		cout << endl;
	}
}
