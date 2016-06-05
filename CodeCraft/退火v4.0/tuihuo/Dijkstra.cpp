#include "Dijkstra.h"

void Dijstra::doDijstraTwo(Graph & graph, int s)
{
	edgeTo = vector<Edge>(graph.getVertexNum());
	dist = vector<int>(graph.getVertexNum(), INF);
	/* 与s相连的点加入队列 */
	dist[s] = 0;
	for (auto e : graph.getAdj(s))
	{
		int index = e.getTo();
		edgeTo[index] = e;
		dist[index] = e.getCost();
		needVisited.push_back(index);
	}

	//处理队列
	while (!needVisited.empty())
	{
		int minDist = INF;
		auto minIndexP = needVisited.begin();
		int minIndex = 0;	//最小值的索引

							//找到最小dist
		for (auto minIndexPtmp = minIndexP; minIndexPtmp != needVisited.end(); minIndexPtmp++)
		{
			if (dist[*minIndexPtmp] < minDist)
			{
				minDist = dist[*minIndexPtmp];
				minIndex = *minIndexPtmp;
				minIndexP = minIndexPtmp;
			}
		}
		needVisited.erase(minIndexP);

		if (!isDemandV(minIndex)) {
			// 遍历最小值为起点的所有边
			for (auto e : graph.getAdj(minIndex))
			{

				// 判断这些边是否需要松弛
				if (e.getCost() + dist[e.getFrom()] < dist[e.getTo()])
				{
					dist[e.getTo()] = e.getCost() + dist[e.getFrom()];
					edgeTo[e.getTo()] = e;

					//检查这个点是新发现的还是本身已经存在	//改为set
					//更新队列
					auto it = find(needVisited.begin(), needVisited.end(), e.getTo());
					if (it != needVisited.end())	//队列中已经存在
						break;
					else
						needVisited.push_back(e.getTo());

				}
			}
		}
	}
}

void Dijstra::dijstraTwo(Graph & graph, int s, int t)
{
	//创建3个图
	doDijstraTwo(graph, s);
	for (auto e : demandSet)
	{
		addLink(graph, s, e);
	}
	for (auto e : demandSet)
	{
		doDijstraTwo(graph, e);
		for (auto f : demandSet)
		{
			addLink(graph, e, f);
		}
		addLink(graph, e, t);
	}	
}

void Dijstra::addLink(Graph &graph, int s, int t)
{	
		int destiny = t;
		dist[s] = INF;
		vector<int> way;	//保存一条路径
		vector<int> id;	//保存ID
		set<int> hasDemandSet;

		if(dist[t] != INF && s != t)
		{
			way.push_back(t);
			id.push_back(edgeTo[t].getID());

			hasDemandSet.insert(t);
			while (edgeTo[destiny].getFrom() != s)
			{
				way.push_back(edgeTo[destiny].getFrom());
				destiny = edgeTo[destiny].getFrom();
				id.push_back(edgeTo[destiny].getID());
				
				if (find(demandSet.cbegin(), demandSet.cend(), destiny) != demandSet.cend())
				{
					hasDemandSet.insert(destiny);
				}
			}
		}
		way.push_back(s);
		
		hasDemandSet.insert(s);

		reverse(way.begin(), way.end());
		reverse(id.begin(), id.end());
		Link link(s, t, dist[t], way, id, hasDemandSet);
		demandCostMap[s][t] = link;
}

void Dijstra::toString(int s, int t)
{
	for (auto w : demandSet)
	{
		vector<int> link = demandCostMap[s][w].getLink();
		vector<int> id = demandCostMap[s][w].getID();
		cout << s << " -> " << w << " :";
		for (auto f : link)
		{
			cout << " " << f;
		}
		cout << endl << " id : ";
		for (auto f : id)
		{
			cout << " " << f;
		}
		cout << endl << " cost : " << demandCostMap[s][w].getCost() << endl;
	}
	cout << "--------------------------" << endl;
	for (auto e : demandSet) {
		for (auto w : demandSet)
		{
			vector<int> link = demandCostMap[e][w].getLink();
			vector<int> id = demandCostMap[e][w].getID();
			cout << e << " -> " << w << " :";
			for (auto f : link)
			{
				cout << " " << f;
			}
			cout << endl << " id : ";
			for (auto f : id)
			{
				cout << " " << f;
			}
			cout << endl <<  " cost : " << demandCostMap[e][w].getCost() << endl;
		}
	}
	cout << "--------------------------" << endl;
	for (auto e : demandSet)
	{
		vector<int> link = demandCostMap[e][t].getLink();
		vector<int> id = demandCostMap[e][t].getID();
		cout << e << " -> " << t << " :";
		for (auto f : link)
		{
			cout << " " << f;
		}
		cout << endl << " id : ";
		for (auto f : id)
		{
			cout << " " << f;
		}
		cout << endl << " cost : " << demandCostMap[e][t].getCost() << endl;
	}
	cout << "--------------------------" << endl;
}

void Dijstra::toString()
{
	for (int i = 0; i < demandCostMap.size(); i++)
	{
		cout << i << " to ";
		for (int j = 0; j < demandCostMap[i].size(); j++)
		{
			vector<int> link = demandCostMap[i][j].getLink();
			cout << j << " : ";
			for (auto w : link)
			{
				cout << w << " ";
			}
			vector<int> id = demandCostMap[i][j].getID();
			cout << " | id : ";
			for (auto w : id)
			{
				cout << w << " ";
			}
			cout << " | cost : " << demandCostMap[i][j].getCost() << endl;

		}
	}
}
