#include "Dijkstra.h"

void Dijstra::doDijstraTwo(Graph & graph, int s)
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

		if (!isDemandV(minIndex)) {
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
					if (it != needVisited.end())	//�������Ѿ�����
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
	//����3��ͼ
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
		vector<int> way;	//����һ��·��
		vector<int> id;	//����ID
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
