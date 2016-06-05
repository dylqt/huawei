#pragma once
#include "std.h"

class Edge {
public:
	Edge() = default;
	Edge(int id, int f, int t, int c) :ID(id), from(f), to(t), cost(c) {};
	int getID() { return ID; };
	int getFrom() { return from; };
	int getTo() { return to; };
	int getCost() { return cost; };
private:
	int ID;
	int from;
	int to;
	int cost;	
};

