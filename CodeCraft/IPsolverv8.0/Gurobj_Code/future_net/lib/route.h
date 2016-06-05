#ifndef  __ROUTE_H__
#define __ROUTE_H__

#include "gurobi_c++.h"
#include "gurobi_c.h"
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;
#define MAX_NODES 600
#define MAX_INT 9600

void search_route(char *graph[5000], int edge_num, char *condition);

#endif
