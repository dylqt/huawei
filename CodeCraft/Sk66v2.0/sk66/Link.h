#pragma once
#include "Edge.h"

class Link {
public:
	Link() = default;
	Link(int from, int to, int allcost, vector<int> &way, vector<int> &id) :
		s(from), t(to), cost(allcost) , link(way), idLink(id) {};
	
	//sk
	Link(int from, int to, int allcost, vector<int> &way, vector<int> &id, set<int> dvsSet, int dvsNumber) :
		s(from), t(to), cost(allcost), link(way), idLink(id) 
		, hasDemandVs(dvsSet), dvsNum(dvsNumber){};
	vector<int> getLink() { return link; }
	vector<int> getID() { return idLink; }
	Link& operator= (Link &rhs);
	int getCost() { return cost; }
	int getS() { return s; }
	int getT() { return t; }

	/*sk*/

	int getDvsNum() { return dvsNum; }
	set<int> getDemandVs() { return hasDemandVs; }
	//判断必经点中是否存在s
	bool hasS(int s) { return hasDemandVs.find(s) != hasDemandVs.end(); }	

private:	
	int s;
	int t;
	int cost;
	vector<int> link;
	vector<int> idLink;
	set<int> hasDemandVs;	//保存路线中含有的必经点
	int dvsNum;	//含有必经点的个数
};

Link operator+ (Link &lhs, Link&rhs);