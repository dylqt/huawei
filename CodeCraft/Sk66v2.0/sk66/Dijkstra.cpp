#include "Dijkstra.h"

void Dijstra::doDijstra(Graph &graph, int s)
{
	edgeTo = vector<Edge>(graph.getVertexNum());
	dist = vector<int>(graph.getVertexNum(), INF);
	/* ��s�����ĵ������� */
	dist[s] = 0;
	for (auto e : graph.getAdj(s))
	{
		int index = e.getTo();
		edgeTo[index] = e;
		dist[index] = e.getCost();
		needVisited.push_back(index);
	}

	//�������
	while (!needVisited.empty()) 
	{
		int minDist = INF;
		auto minIndexP = needVisited.begin();
		int minIndex = 0;	//��Сֵ������

		//�ҵ���Сdist
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

		// ������СֵΪ�������б�
		for (auto e : graph.getAdj(minIndex))
		{
			// �ж���Щ���Ƿ���Ҫ�ɳ�
			if (e.getCost() + dist[e.getFrom()] < dist[e.getTo()])
			{
				dist[e.getTo()] = e.getCost() + dist[e.getFrom()];
				edgeTo[e.getTo()] = e;

				//�����������·��ֵĻ��Ǳ����Ѿ�����	//��Ϊset
				//���¶���
				auto it = find(needVisited.begin(), needVisited.end(), e.getTo());
				if(it != needVisited.end())	//�������Ѿ�����
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
		//��ÿ���ؾ������һ��ɨ��
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
		vector<int> way;	//����һ��·��
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
