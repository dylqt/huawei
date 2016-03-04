#include "Digraph.h"

Digraph::Digraph(ifstream &is) {
	string test;
	getline(is, test);
	vertex = stoi(test);
	getline(is, test);
	edge = stoi(test);
	adj = vector<vector<int>>(vertex);
	while (getline(is, test)) {
		istringstream s(test);
		string from;
		string to;
		s >> from >> to;
		adj[stoi(from)].push_back(stoi(to));
	}
}

Digraph::Digraph(int v){
	vertex = v;
	edge = 0;
	adj = vector<vector<int>>(v);
}

void Digraph::toString() {
	cout << vertex << "vertices" << edge << "edges" << endl;
	for (int i = 0; i < vertex; i++) {
		cout << "from " << i << " :";
		for (auto w : adj[i]) {
			cout << " " << w;
		}
		cout << endl;
	}
}

void Digraph::addEdge(int v, int w) {
	adj[v].push_back(w);
	edge++;
}

vector<int> Digraph::link(int v) {
	return adj[v];
}

Digraph Digraph::reverse() {
	Digraph R(vertex);
	for (int i = 0; i < vertex; i++) {
		for (auto w : adj[i]) {
			R.addEdge(w, i);
		}
	}
	return R;
}

DirectedCycle::DirectedCycle(Digraph & G)
{
	marked = vector<bool>(G.V());
	edgeTo = vector<int>(G.V());
	onStacked = vector<bool>(G.V());
	cycle = false;

	for (int v = 0; v < G.V(); v++) {
		if (!marked[v])
			dfs(G, v);
	}
}

void DirectedCycle::dfs(Digraph &G, int from)
{
	marked[from] = true;
	onStacked[from] = true;
	for (auto to : G.link(from)) {
		if (!marked[to]) {
			edgeTo[to] = from;
			dfs(G, to);
		}
		else if (onStacked[to]) {
			vector<int> cycleTmp;
			for (int x = from; x != to; x = edgeTo[x]) {
				cycleTmp.push_back(x);
			}
			cycleTmp.push_back(to);
			cycleTmp.push_back(from);
			reverse(cycleTmp.begin(), cycleTmp.end());
			cycle = true;
			cycleStacks.push_back(cycleTmp);
		}
	}
	onStacked[from] = false;
}

void DirectedCycle::cycleToString()
{
	for (int i = 0; i < cycleStacks.size(); i++) {
		cout << "cycle" << i << " :";
		for(auto w : cycleStacks[i]){
			cout << " " << w;
		}
		cout << endl;
	}
}
