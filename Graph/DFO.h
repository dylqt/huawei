#pragma once

#include "Digraph.h"
#include <deque>
class DepthFirstOrder {
public:
	DepthFirstOrder(Digraph &G);
	void dfs(Digraph &G, int v);
	void toTopological(Digraph &G);
	deque<int> getTopStack() { return topStack; }
private:
	deque<int> topStack;
	vector<bool> marked;
};
