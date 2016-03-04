#include "diDFS.h"

void DirectedDFS::DFS(Digraph & G, int s)
{
	marked[s] = true;
	for (auto w : G.link(s)) {
		if (!marked[w]) {
			adj[w] = s;
			DFS(G, w);
		}
	}
}

void DirectedDFS::toString(Digraph &G)
{
	for (int i = 0; i < G.V(); i++) {
		if (isReachable(i)) {
			cout << "to " << i << " :" ;
			for (int x = i; x != s; x = adj[x]) {
				cout << " " << x;
			}
			cout << endl;
		}
	}
}

