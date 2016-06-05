#include "Link.h"

Link operator+(Link & from, Link & to)
{
	vector<int> idLink(from.getID());
	vector<int> vLink(from.getLink());
	set<int> hasDvsSet(from.getDemandVs());
	int cost = from.getCost() + to.getCost();

	for (int i = 0; i < to.getID().size(); i++)
	{
		int id = to.getID()[i];
		idLink.push_back(id);
	}
	for (int i = 1; i < to.getLink().size(); i++ )
	{
		int v = to.getLink()[i];
		vLink.push_back(v);
	}
	
	if(to.getDemandVs().size() > 0)
		hasDvsSet.insert(to.getDemandVs().cbegin(), to.getDemandVs().cend());
	int demandNum = hasDvsSet.size();
	Link link(from.getS(), to.getT(), cost, vLink, idLink, hasDvsSet, demandNum);
	return link;
}

Link & Link::operator=(Link & rhs)
{
	this->cost = rhs.getCost();
	this->idLink = rhs.getID();
	this->link = rhs.getLink();
	this->s = rhs.getS();
	this->t = rhs.getT();
	this->dvsNum = rhs.getDvsNum();
	this->hasDemandVs = rhs.getDemandVs();
	return *this;
	// TODO: �ڴ˴����� return ���
}
