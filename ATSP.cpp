#include "ATSP.h"


void Atsp::BFS(diGraph &graph, int v)
{

	priority_queue<Edge> p1;
	for (auto w : graph.getAdj(v))
		p1.push(w);
	branch.push_back(p1);

	while (branch.back().size() != 0)
	{
		//得到第一个值
		Edge tmp = branch.back().top();
		branch.back().pop();
		
		//判断当前权值是否已经大于上限
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
	//是否被标记过
	if (!allMarked[v.getTo()]) 
	{	
		//数据更新
		goOver.push_back(v);
		allMarked[v.getTo()] = true;
		goWeight += v.getWeight();

		//是否为必经点
		if (dV.find(v.getTo()) != dV.end())	//该点是必须访问的一个点
		{
			dVisite--;			
		}
		
		//是否为终点
		if (v.getTo() != t)
			BFS(graph, v.getTo());
		else if (dVisite == 0)	//找到一个路径
		{
			weight = goWeight;
			result.assign(goOver.begin(), goOver.end());
		}

		//调回数据
		goOver.pop_back();
		allMarked[v.getTo()] = false;
		goWeight -= v.getWeight();
		//是否为必经点
		if (dV.find(v.getTo()) != dV.end())	//该点是必须访问的一个点
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
