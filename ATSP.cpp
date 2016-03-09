#include "ATSP.h"


void Atsp::BFS(diGraph &graph, int v)
{

	priority_queue<Edge> p1;
	for (auto w : graph.getAdj(v))
		p1.push(w);
	branch.push_back(p1);

	while (branch.back().size() != 0)
	{
		//�õ���һ��ֵ
		Edge tmp = branch.back().top();
		branch.back().pop();
		
		//�жϵ�ǰȨֵ�Ƿ��Ѿ���������
		if (goWeight + tmp.getWeight() < weight)
			doAtsp(graph, tmp);
		else
		{
			while (!branch.back().empty())
				branch.back().pop();
		}
					
	}
	branch.pop_back();
}

void Atsp::doAtsp(diGraph & graph, Edge v)
{
	//�Ƿ񱻱�ǹ�
	if (!allMarked[v.getTo()]) 
	{	
		//���ݸ���
		goOver.push_back(v);
		allMarked[v.getTo()] = true;
		goWeight += v.getWeight();

		//�Ƿ�Ϊ�ؾ���
		if (dV.find(v.getTo()) != dV.end())	//�õ��Ǳ�����ʵ�һ����
		{
			dVisite--;			
		}
		
		//�Ƿ�Ϊ�յ�
		if (v.getTo() != t)
			BFS(graph, v.getTo());
		else if (dVisite == 0)	//�ҵ�һ��·��
		{
			weight = goWeight;
			result.assign(goOver.begin(), goOver.end());
		}

		//��������
		goOver.pop_back();
		allMarked[v.getTo()] = false;
		goWeight -= v.getWeight();
		//�Ƿ�Ϊ�ؾ���
		if (dV.find(v.getTo()) != dV.end())	//�õ��Ǳ�����ʵ�һ����
		{
			dVisite++;
		}
		return;

	}
	else
		return;
	
}

void Atsp::toString()
{
	cout << "way :";
	for (int i = 0; i < result.size(); i++)
	{
		cout << " " << result[i].getFrom() << " -> " << result[i].getTo();
	}
	cout << endl;
	cout << "weight : " << weight << endl;
}
