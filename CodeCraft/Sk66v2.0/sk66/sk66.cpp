#include "sk66.h"

void Sk66::doSk66(Dijstra &dij, int s, int t)
{
	vector<vector<Link>>D(dij.getDemandCostMap());
	// f vi 0;
	for (auto w : demandVertexSet)
	{
		fn[0][w] = D[w][t];
	}

	//iterative
	vector<int> viSet(demandVertexSet);
	viSet.push_back(s);
	for (int n = 1; n <= VsNum - 1; n++)
	{
		for (auto vi : viSet)
		{
			int minCost = INF;
			Link minLink;
			for (auto vl : demandVertexSet)
			{
				
				if (D[vi][vl].getCost() + fn[n - 1][vl].getCost() < minCost)
				{
					minCost = D[vi][vl].getCost() + fn[n - 1][vl].getCost();
					minLink = D[vi][vl] + fn[n - 1][vl];
				/*	toString(D[vi][vl]);
					toString(fn[n - 1][vl]);
					toString(minLink);*/
				}
			}
			fn[n][vi] = minLink;
		}
		//toString(n);
	}
	// last time
	int minCost = INF;
	Link minLink;
	for (auto vl : demandVertexSet)
	{

		if (D[s][vl].getCost() + fn[VsNum - 1][vl].getCost() < minCost)
		{
			minCost = D[s][vl].getCost() + fn[VsNum - 1][vl].getCost();
			minLink = D[s][vl] + fn[VsNum - 1][vl];
			
		}
	}
	fn[VsNum][s] = minLink;
	
}

/*void Sk66::toString(int n)
{
	for (int vi = 0; vi < VsNum + 2; vi++)
	{
		cout << "f" << n << " form " << vi << " : " << endl;
		cout << "link : ";
		vector<int> link = fn[n][vi].getLink();
		for (auto w : link)
		{
			cout << w << " ";
		}
		cout << endl;
		
		vector<int> id = fn[n][vi].getID();
		cout << "ID : ";
		for (auto w : id)
		{
			cout << w << " ";
		}
		cout << endl;
		cout << "cost : " << fn[n][vi].getCost() << endl;
		cout << "-------------" << endl;
	}
}*/
void Sk66::toString(int s)
{
	vector<int> link = fn[VsNum][s].getLink();
	cout << "link : ";
	for (auto w :link)
	{
		cout << w << " ";
	}
	cout << endl;
	cout << "ID : ";
	vector<int> id = fn[VsNum][s].getID();
	for (auto w : id)
	{
		cout << w << " ";
	}
	cout << endl;
	cout << "cost : " << fn[VsNum][s].getCost();
	cout << endl;
}

void Sk66::toString(Link & link)
{
	vector<int> lk = link.getLink();
	for (auto w : lk)
	{
		cout << w << " ";
	}
	cout << endl;

	vector<int> id = link.getID();
	cout << "ID : ";
	for (auto w : id)
	{
		cout << w << " ";
	}
	cout << endl;
	cout << "cost : " << link.getCost() << endl;
	cout << "-------------" << endl;
}
