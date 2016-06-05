#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

#define MAX_INT 96000
#define MAX_NODES 600
void search_route(char *topo[5000], int edge_num, char *demand);

/*build Graph*/
class Graph{
public:
	/* Graph */
	int verNum;					// ver number
	int specialNum;				// special number
	int start;					// s
	int terminial;				// t
	vector<int> specialNodes;	//	special vertex set
	vector<vector<int> > weightTable;	//
	vector<vector<int> > idTable;

	void initGraph(char *topo[5000], int edge_num, char *demand); //¹¹½¨Í¼
	void toString();
};


#endif
