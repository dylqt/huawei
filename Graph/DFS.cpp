#include "DFS.h"

DepthFirstSearch::DepthFirstSearch(Graph &G, int s) {
	marked = vector<bool>(G.V());
	edgeTo = vector<int>(G.V());
	this->s = s;
	DFS(G, s);
}

void DepthFirstSearch::DFS(Graph &G, int s) {
	marked[s] = true;
	for (auto w : G.link(s)){
		if (!marked[w]) {
			edgeTo[w] = s;
			DFS(G, w);
		}
	}
}

vector<int> DepthFirstSearch::pathTo(int v) {
	vector<int> links;
	if (hasPath(v)) {
		for (int tmp = v; tmp != s; tmp = edgeTo[tmp]) {
			links.push_back(tmp);
		}
		links.push_back(s);
	}
	reverse(links.begin(), links.end());
	return links;
}

void DepthFirstSearch::printByString(int e) {
	vector<int> links = pathTo(e);
	cout << s << " to " << e << " :";
	for (auto x : links) {
		cout << " " << x;
	}
	cout << endl;
}