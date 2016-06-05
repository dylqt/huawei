
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include "Graph.h"
#include "Dijkstra.h"
#include "KM.h"
//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	Graph graph(topo, edge_num, demand);
	int s = graph.getS();
	int t = graph.getT();
	
	//graph.toString();
	Dijstra dij(graph);
	//dij.dijstraSK(graph, s, t);
	//dij.dijstraSK66(graph, s, t);
	dij.dijstraTwo(graph, s, t);
	//dij.toString(s, t);
	//KM km(graph);
	//km.doKM(dij);
	//km.toString();
	//two.toString();
	
	//two.toString();
	//dij.toString();
	//dij.toString(s, t);
	//Sk66 sk66(graph);
	//sk66.doSk66(dij, s, t);
	//sk66.toString(s);
	/*Link lhs(0, 3, 3, vector<int>{0, 2, 3}, vector<int>{1, 6});
	Link rhs(3, 1, 1, vector<int>{3, 1}, vector<int>{4});
	Link res = lhs + rhs;
	vector<int> link = res.getID();
	for (auto w : link)
	{
		cout << w << " ";
	}
	cout << res.getCost() << endl;
	*/
	
	
	//unsigned short result[] = { 2, 6, 3 };//示例中的一个解

	//for (int i = 0; i < 3; i++)
	//	record_result(result[i]);
}
