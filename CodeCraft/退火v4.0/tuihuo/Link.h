#pragma once
#include "Edge.h"

class Link {
public:
	Link() = default;
	Link(int from, int to, int allcost, vector<int> &way, vector<int> &id) :
		s(from), t(to), cost(allcost) , link(way), idLink(id) {};
	
	//sk
	Link(int from, int to, int allcost, vector<int> &way, vector<int> &id, set<int> dvsSet) :
		s(from), t(to), cost(allcost), link(way), idLink(id), hasDemandVs(dvsSet){};
	vector<int> getLink() { return link; }
	vector<int> getID() { return idLink; }
	Link& operator= (Link &rhs);
	int getCost() { return cost; }
	int getS() { return s; }
	int getT() { return t; }

	void toString();
	/*sk*/
	set<int> getDemandVs() { return hasDemandVs; }
	//判断必经点中是否存在s
	bool hasS(int s) { return hasDemandVs.find(s) != hasDemandVs.end(); }	

private:	
	int s;
	int t;
	int cost = INF;
	vector<int> link;
	vector<int> idLink;
	set<int> hasDemandVs;	//保存路线中含有的必经点
};

Link operator+ (Link &lhs, Link&rhs);