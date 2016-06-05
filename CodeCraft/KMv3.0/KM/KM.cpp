#include "KM.h"

void KM::doKM(Dijstra &dij)
{
	initKM(dij);
	for (int u = 0; u < dealNum - 1; u++)
	{
		slack = vector<int>(dealNum, INF);
		while (true)
		{
			visitX = vector<bool>(dealNum, false);
			visitY = vector<bool>(dealNum, false);

			if (findPath(u))
			{
				visited[u] = true;
				break;
			}

			int inc = INF;
			for (int v = 0; v < dealNum; v++)
			{
				if (!visitY[v] && slack[v] < inc)
					inc = slack[v];
			}
			for (int i = 0; i < dealNum; i++)
			{
				if (visitX[i])
				{
					Lx[i] -= inc;
				}
				if (visitY[i])
				{
					Ly[i] += inc;
				}
			}
		}

	}
}

void KM::toString()
{
	
	int from = MAP[s];
	int to = MAP[t];
	int cost = 0;
	result = vector<int>(dealNum);
	while (match[to] != -1)
	{
		//indexMap[match[to]][to].toString();
		result.push_back(indexMap[match[to]][to].getT());
		cost += indexMap[match[to]][to].getCost();
		to = match[to];
	}
	result.push_back(s);
	reverse(result.begin(), result.end());
	for (int i = 0; i < dealNum; i++)
	{
		cout << result[i] << " ";
	}
	cout << endl;
	cout << "cost : " << cost << endl;
	
	for (int i = 0; i < dealNum; i++)
	{
		cout << match[i] <<  " : "<< i << endl;
	}
}

void KM::initKM(Dijstra & dij)
{
	//建立map
	int index = 0;
	MAP[s] = index;
	++index;	
	for (auto w : demandV)
	{
		MAP[w] = index;
		++index;
	}
	MAP[t] = index;

	//各种初始化
	visited = vector<bool>(dealNum, false);
	Lx = vector<int>(dealNum, -INF);
	Ly = vector<int>(dealNum, 0);
	vector<vector<Link>> costTmp = dij.getDemandCostMap();

	demandV.push_back(s);
	demandV.push_back(t);
	for (auto w : demandV)
	{
		for (auto e : demandV)
		{
			indexMap[MAP[w]][MAP[e]] = costTmp[w][e];
			costMap[MAP[w]][MAP[e]] = -costTmp[w][e].getCost();
			if (Lx[MAP[w]] < costMap[MAP[w]][MAP[e]])
				Lx[MAP[w]] = costMap[MAP[w]][MAP[e]];
		//	if(MAP[e] == 0)
		//		cout << costMap[MAP[w]][MAP[e]] << " ";
		}
		//cout << endl;
	}
	demandV.pop_back();
	demandV.pop_back();
}

bool KM::findPath(int u)
{
	visitX[u] = true;
	for (int v = 0; v < dealNum; v++)
	{
		if (!visitY[v] && costMap[u][v] != -INF && (match[u] == -1 || !visited[u] ))
		{
		
				int dTmp = Lx[u] + Ly[v] - costMap[u][v];
				if (dTmp == 0)
				{
					cout << "u : " << u << " v : " << v << endl;
					visitY[v] = true;
					if (match[v] == -1 )
					{
						match[v] = u;
						return true;
					}
					else 
					{
						visited[match[v]] = false;
						
						if (findPath(match[v]))
						{
							match[v] = u;
							return true;
						}
					}
				}
				else
					slack[v] = (slack[v] > dTmp ? dTmp : slack[v]);
		}
	}
	return false;
}
