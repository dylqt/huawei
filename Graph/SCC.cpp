#include "SCC.h"
#include "DFO.h"

SCC::SCC(Digraph & G)
{
	marked = vector<bool>(G.V());
	id = vector<int>(G.V());
	count = 0;
	DepthFirstOrder dfo(G.reverse());
	deque<int> topStack(dfo.getTopStack());
	vector<int> comp; // connect
	for (int s : topStack)
	{
		comp.clear();
		if (!marked[s])
		{
			dfs(G, s, comp);
			count++;
			connects.push_back(comp);
		}
	
	}
}

void SCC::dfs(Digraph & G, int v, vector<int> &comp)
{
	marked[v] = true;
	id[v] = count;
	comp.push_back(v);
	for (int s : G.link(v))
	{
		if (!marked[s])
		{
			dfs(G, s, comp);
		}
	}
}

void SCC::conToString()
{
	cout << count << " components :" << endl;
	for (int i = 0; i < connects.size(); i++)
	{
		for (int s : connects[i])
		{
			cout << " " << s;
		}
		cout << endl;
	}
}
