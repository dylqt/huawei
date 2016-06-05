#include "Graph.h"
#include "ATSP.h"

int main() 
{
	clock_t start, finish;
	start = clock();
	/*
	diGraph dg(11);
	dg.addEdge(0, 1, 2);
	dg.addEdge(0, 2, 3);
	dg.addEdge(0, 3, 4);
	dg.addEdge(1, 2, 3);
	dg.addEdge(1, 5, 2);
	dg.addEdge(1, 4, 7);
	dg.addEdge(2, 5, 9);
	dg.addEdge(2, 6, 2);
	dg.addEdge(3, 6, 2);
	dg.addEdge(4, 7, 3);
	dg.addEdge(4, 8, 3);
	dg.addEdge(5, 6, 1);
	dg.addEdge(5, 8, 3);
	dg.addEdge(6, 9, 1);
	dg.addEdge(6, 8, 5);
	dg.addEdge(7, 10, 3);
	dg.addEdge(8, 10, 2);
	dg.addEdge(9, 8, 2);
	dg.addEdge(9, 10, 2);
	dg.toString();

	set<int> desire{ 5, 8 };
	int s = 0; 
	int t = 10;
	*/
	ifstream is("Text1.txt");
	diGraph dg(is);
	int s = 0;
	int t = 56;
	set<int> desire{ 5, 8 };
	//起点为必经点
	if (desire.find(s) != desire.end())	
	{
		desire.erase(s);
	}
	Atsp atsp(dg, s, t, desire);
	atsp.BFS(dg, s);
	atsp.toString();
	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("time:%f\n", duration);
}

// keep two array, to know if has visited
