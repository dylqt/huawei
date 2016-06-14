#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "assert.h"
#include "stdlib.h"
#include "memory.h"

using namespace std;

#define MAX_INT 96000
#define MAX_NODES 600
#define DEBUG
void search_route(char *topo[5000], int edge_num, char *demand);
void toString();
void initGraph(char * topo[5000], int edge_num, char * demand);
int ioc(int i, int j);	//求[i,j]在矩阵中的索引
void build_prob(void);

#endif
