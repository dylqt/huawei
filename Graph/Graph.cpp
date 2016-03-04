#include "Graph.h"

Graph::Graph(ifstream &is) {
	string text;
	//write into vertex & edge
	getline(is, text);
	vertex = stoi(text);
	getline(is, text);
	edge = stoi(text);
	vector<vector<int>> adjTmp(vertex);
	// write adj
	while (getline(is, text)){
		istringstream stream(text);
		string word;
		string from;
		string to;
		while (stream >> from >> to) {
			adjTmp[stoi(from)].push_back(stoi(to));
			adjTmp[stoi(to)].push_back(stoi(from));
		}
	}
	adj = adjTmp;
}

void Graph::printByString() {
	cout << vertex << " vertices" << edge << " edges" << endl;
	for (int i = 0; i < adj.size(); i++) {
		cout << i << ":";
		for (auto tmp : adj[i]) {
			cout << " " << tmp;
		}
		cout << endl;
	}
}

void Graph::addEdge(int from, int to) {
	adj[from].push_back(to);
	adj[to].push_back(from);
	edge++;
}

CC::CC(Graph & G)
{
	count = 0;
	marked = vector<bool>(G.V(), false);
	id = vector<int>(G.V());
	for (int i = 0; i < G.V(); i++) {
		if (!marked[i]) {
			DFS(G, i);
			++count;
		}
	}
}


void CC::DFS(Graph & G, int s)
{
	marked[s] = true;
	id[s] = count;
	for (int x : G.link(s)) {
		if (!marked[x]) {
			DFS(G, x);
		}
	}
}

/************************************************************************/
/* Cycle                                                                     */
/************************************************************************/
Cycle::Cycle(Graph & G)
{
	marked = vector<bool>(G.V(), false);
	hasCycle = false;
	for (int i = 0; i < G.V(); i++) {
		if (!marked[i]) {
			DFS(G, i, i);
		}
	}
}

void Cycle::DFS(Graph & G, int s, int t)
{
	marked[s] = true;
	for (int w : G.link(s)) {
		if (!marked[w]) {
			DFS(G, w, s);
		}
		else if(w != t){
			hasCycle = true;
		}
	}
}

void TwoColor::DFS(Graph & G, int s)
{
	marked[s] = true;
	for (auto n : G.link(s)) {
		if (!marked[n]) {
			color[n] = !color[s];
			DFS(G, n);
		}
		else if (color[n] == color[s])
			isTwoColorable = false;
	}
}
