#include "Dijkstra.h"

void Dijstra::doDijstra(Graph &graph, int s)
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
				if(it != needVisited.end())	//队列中已经存在
					break;
				else
					needVisited.push_back(e.getTo());
			}
		}
	}
}

void Dijstra::dijstraSK66(Graph & graph, int s, int t)
{
	demandSet.push_back(s);
	demandSet.push_back(t);
	for (auto e : demandSet)
	{
		doDijstra(graph, e);
		//对每个必经点进行一次扫描
		for (auto w : demandSet)
		{
			addLink(graph, e, w);
		}
	}	
}

void Dijstra::addLink(Graph &graph, int s, int t)
{	
		int destiny = t;
		dist[s] = INF;
		vector<int> way;	//保存一条路径
		vector<int> id;
		if(dist[t] != INF && s != t)
		{
			way.push_back(t);
			id.push_back(edgeTo[t].getID());
			while (edgeTo[destiny].getFrom() != s)
			{
				way.push_back(edgeTo[destiny].getFrom());				
				destiny = edgeTo[destiny].getFrom();
				id.push_back(edgeTo[destiny].getID());
			}
		}
		way.push_back(s);
		//id.push_back(edgeTo[s].getID());
		reverse(way.begin(), way.end());
		reverse(id.begin(), id.end());
		Link link(s, t, dist[t], way, id);
		demandCostMap[s][t] = link;
}

void Dijstra::toString(int s, int t)
{
	/*cout << "start " << s << " : "<< endl;
	for (int i = 0; i < edgeTo.size(); i++)
	{
		cout << edgeTo[i].getID() << " "
			<< edgeTo[i].getFrom() << " "
			<< edgeTo[i].getTo() << " "
			<< edgeTo[i].getCost() << endl;
	}*/
	/*
	cout << s << " " << t << " : "
		<< demandCostMap[s][t].getCost() << endl;
	vector<int> link = demandCostMap[s][t].getLink();
	for (int i = 0; i < link.size(); i++)
	{
		cout << link[i];
	}
	cout << endl;
	*/
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
}
