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
	set<int> tmp(to.getDemandVs());
	if(to.getDemandVs().size() > 0)
		hasDvsSet.insert(tmp.begin(), tmp.end());
	
	Link link(from.getS(), to.getT(), cost, vLink, idLink, hasDvsSet);
	return link;
}

Link & Link::operator=(Link & rhs)
{
	this->cost = rhs.getCost();
	this->idLink = rhs.getID();
	this->link = rhs.getLink();
	this->s = rhs.getS();
	this->t = rhs.getT();

	this->hasDemandVs = rhs.getDemandVs();
	return *this;
	// TODO: 在此处插入 return 语句
}

void Link::toString()
{
	cout << s << " to " << t << endl;
	cout << "link : ";
	for (auto w : link)
	{
		cout << w << " ";
	}
	cout << endl << "id : ";
	for (auto w : idLink)
	{
		cout << w << " ";
	}
	cout << endl << "cost : " << cost;
	cout << endl << "---------------" << endl;
}
