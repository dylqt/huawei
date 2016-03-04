#include "DFO.h"


DepthFirstOrder::DepthFirstOrder(Digraph & G)
{
	marked = vector<bool>(G.V(), false);
	for (int i = 0; i < G.V(); i++) {
		if (!marked[i])
		{
			dfs(G, i);
		}
	}
}

void DepthFirstOrder::dfs(Digraph &G, int v)
{
	marked[v] = true;
	for (auto w : G.link(v))
	{
		if (!marked[w])
		{
			dfs(G, w);
		}
	}
	topStack.push_front(v);
}

void DepthFirstOrder::toTopological(Digraph &G)
{
	DirectedCycle cycleFinder(G);
	if (!cycleFinder.hasCycle())
	{
		cout << "topological :";
		for (auto w : topStack)
		{
			cout << " " << w;
		}
		cout << endl;
	}
	else {
		cout << " has cycle " << endl;
	}
}