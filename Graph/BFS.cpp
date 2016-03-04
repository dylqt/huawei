#include "BFS.h"
#include <deque>

BreadthFirstSearch::BreadthFirstSearch(Graph &G, int s) {
	marked = vector<bool>(G.V());
	edgeTo = vector<int>(G.V());
	this->s = s;
	BFS(G, s);
}

void BreadthFirstSearch::BFS(Graph &G, int s) {
	deque<int> queue;
	queue.push_back(s);
	while (!queue.empty()) {
		int v = queue.front();
		queue.pop_front();
		for (int n : G.link(v)) {
			if (!marked[n]) {
				marked[n] = true;
				edgeTo[n] = v;
				queue.push_back(n);
			}
		}
	}
}


vector<int> BreadthFirstSearch::pathTo(int v) {
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

void BreadthFirstSearch::printByString(int e) {
	vector<int> links = pathTo(e);
	cout << s << " to " << e << " :";
	for (auto x : links) {
		cout << " " << x;
	}
	cout << endl;
}