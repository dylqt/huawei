#include "tuihuo.h"



void Tuihuo::initTuohuo(Dijstra & dij)
{
	initMAP();
	dealNum = demandV.size() + 2;
	costMap = vector<vector<int>>(dealNum, vector<int>(dealNum));
	resultMap = vector<vector<Link>>(dealNum, vector<Link>(dealNum));
	workRoad = vector<int>(dealNum);

	vector<vector<Link>> demandCostMap = dij.getDemandCostMap();
	demandV.push_back(s);
	demandV.push_back(t);
	for (auto w : demandV)
	{
		//cout << MAP[w] << endl;
		for (auto e : demandV)
		{
			costMap[MAP[w]][MAP[e]] = demandCostMap[w][e].getCost();
			resultMap[MAP[w]][MAP[e]] = demandCostMap[w][e];
			//cout << costMap[MAP[w]][MAP[e]] << " | " ;
			//cout << resultMap[MAP[w]][MAP[e]].getCost() << endl;
		}
		//cout << endl;
	}
	demandV.pop_back();
	demandV.pop_back();

	T0 = INF;		//起始温度
	endT = 0;	//结束温度
	tmpTimes = 0;
	L = 10;		//每个温度点运行次数
	a = 0.99;		//每次降温比例
	q = 20;		//运行q次没有改变时退出
	costWay = 0;	//当前的cost
	noNewTimes = 0;
	costBest = 0;
	wayBest = vector<int>(dealNum);
}

void Tuihuo::doTuihuo(Dijstra &dij)
{
	initTuohuo(dij);
	for (int i = 0; i < workRoad.size(); i++)
	{
		workRoad[i] = i;
	}
	for (int i = 0; i < workRoad.size() - 1; i++)
	{
		int from = workRoad[i];
		int to = workRoad[i + 1];
		costWay += costMap[from][to];
	}

	costBest = costWay;
	wayBest = workRoad;
	double Ti = T0;
	while (Ti >= 0)
	{
		if (noNewTimes >= q)
		{
			if (costBest < INF && costBest > 0)
			{
				cout << costBest << endl;
				break;
			}
			else
			{
				costWay = costBest;
				workRoad = wayBest;
				Ti = T0;
			}
		}
		//if (noNewTimes >= q)
		//	break;

		//每个温度的循环
		for (int i = 0; i < L; i++)
		{
			int choose = rand() % 2;
			switch (choose)
			{
			case 0:
				tuihuoChangeTwo(Ti);
				break;
			case 1:
				tuihuoReverse(Ti);
				break;
			default:
				break;
			}
		}
		Ti = a * Ti;
	}

}




void Tuihuo::toString()
{
	int cost = 0;
	for (int i = 0; i < wayBest.size() - 1; i++)
	{
		int from = wayBest[i];
		int to = wayBest[i + 1];
		
		//scout << resultMap[from][to].getS() << " : " << resultMap[from][to].getT() << " | " ;
		resultMap[from][to].toString();
		cout << resultMap[from][to].getCost() << endl;
		cost += resultMap[from][to].getCost();
	}

//	cout << "cost : " << cost << endl;
}

void Tuihuo::initMAP()
{
	MAP[s] = 0;
	int index = 1;
	for (auto w : demandV)
	{
		MAP[w] = index;
		++index;
	}
	MAP[t] = index;
}

void Tuihuo::tuihuoChangeTwo(int Ti)
{
	//生成两个随机数
	srand(time(0));
	int ch1 = (rand() % (dealNum - 2)) + 1;
	int ch2 = (rand() % (dealNum - 2)) + 1;
	while (ch2 == ch1)
		ch2 = (rand() % (dealNum - 2)) + 1;

	int costNew = getNewCostBychange(ch1, ch2);
	double inc = costNew - costWay;
	if (inc <= 0 )
	{
		costWay = costNew;
		changeTwo(ch1, ch2);

		if (costNew < costBest)
		{
			wayBest = workRoad;
			costBest = costNew;
			//toString();
			//cout << costBest << endl;
		}		
		noNewTimes = 0;
	}
	else
	{
		double lhs = exp((-inc) / Ti);
		double rhs = (rand() % 777) / (double)777;
		if (lhs > rhs)
		{
			costWay = costNew;
			changeTwo(ch1, ch2);
			noNewTimes = 0;
		}
		else
		{
			noNewTimes++;
		}
	}
}

void Tuihuo::changeTwo(int lhs, int rhs)
{
	int ch1 = workRoad[lhs];
	int ch2 = workRoad[rhs];
	workRoad[rhs] = ch1;
	workRoad[lhs] = ch2;
}

int Tuihuo::getNewCostBychange(int lhs, int rhs)
{
	int cost = costWay;

	int ch1 = workRoad[lhs];
	int ch2 = workRoad[rhs];
	int ch1Left = workRoad[lhs - 1];
	int ch1Right = workRoad[lhs + 1];
	int ch2Left = workRoad[rhs - 1];
	int ch2Right = workRoad[rhs + 1];
	cost -= costMap[ch1Left][ch1];
	cost -= costMap[ch1][ch1Right];
	cost -= costMap[ch2Left][ch2];
	cost -= costMap[ch2][ch2Right];

	cost += costMap[ch1Left][ch2];
	cost += costMap[ch2][ch1Right];
	cost += costMap[ch2Left][ch1];
	cost += costMap[ch1][ch2Right];

	return cost;
}

void Tuihuo::tuihuoReverse(int Ti)
{
	//生成两个随机数
	srand(time(0));
	int ch1 = (rand() % (dealNum - 2)) + 1;
	int ch2 = (rand() % (dealNum - 2)) + 1;
	while (ch2 == ch1)
		ch2 = (rand() % (dealNum - 2)) + 1;

	int costNew = getNewCostByReverse(ch1, ch2);
	double inc = costNew - costWay;
	if (inc <= 0)
	{
		costWay = costNew;
		reverseTwo(ch1, ch2);
		int cost = 0;
		for (int i = 0; i < workRoad.size() - 1; i++)
		{
			int from = workRoad[i];
			int to = workRoad[i + 1];
			cost += resultMap[from][to].getCost();
		}

		if (costNew < costBest)
		{
			wayBest = workRoad;
			costBest = costNew;
		}
		noNewTimes = 0;
	}
	else
	{
		double x = (-inc) / Ti;
		double lhs = exp((-inc) / Ti);
		double rhs = (rand() % 777) / (double)777;
		if (lhs > rhs)
		{
			costWay = costNew;
			reverseTwo(ch1, ch2);
			noNewTimes = 0;
		}
		else
			noNewTimes++;
	}
}

int Tuihuo::getNewCostByReverse(int ch1, int ch2)
{
	int cost = 0;
	int from = (ch1 > ch2 ? ch2 : ch1);
	int to = (ch1 < ch2 ? ch2 : ch1);
	
	for (int i = 0; i < from; i++)
	{
		cost += costMap[workRoad[i]][workRoad[i + 1]];
	}
	cost += costMap[workRoad[from]][workRoad[to - 1]];
	for (int i = to - 1; i > from + 1; i--)
	{
		cost += costMap[workRoad[i]][workRoad[i - 1]];
	}
	cost += costMap[workRoad[from + 1]][workRoad[to]];
	for (int i = to; i < workRoad.size() - 1; i++)
	{
		cost += costMap[workRoad[i]][workRoad[i + 1]];
	}
	return cost;
}

void Tuihuo::reverseTwo(int ch1, int ch2)
{
	int from = (ch1 > ch2 ? ch2 : ch1);
	int to = (ch1 < ch2 ? ch2 : ch1);
	int dif = to - from;
	int mid = dif / 2;
	for (int i = 1; i <= mid; i++)
	{	
		int tmp = workRoad[from + i];
		workRoad[from + i] = workRoad[to - i];
		workRoad[to - i] = tmp;	
	}
}

void Tuihuo::check()
{
	set<int> chk;
	Link output;
	for (int i = 0; i < wayBest.size() - 1; i++)
	{
		int sizeOld = output.getDemandVs().size() + resultMap[wayBest[i]][wayBest[i + 1]].getDemandVs().size();
		output = output + resultMap[wayBest[i]][wayBest[i + 1]];
		if (sizeOld > output.getDemandVs().size())
		{
			vector<int> list = output.getLink();
			for (auto w : list)
			{
				cout << w << " ";
			}
			return;
		}

	}
}