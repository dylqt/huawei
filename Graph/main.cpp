#include "Graph.h"
#include "DFS.h"
#include "diDFS.h"
#include "BFS.h"
#include "Digraph.h"
#include "DFO.h"
#include "SCC.h"
#include "EdgeWeightedGraph.h"
int main()
{
	//ifstream is("tinyG.txt");
	//Graph gTiny(is);
	//gTiny.printByString();
	//gTiny.addEdge(1, 2);
	//gTiny.printByString();
	//DepthFirstSearch DFS(gTiny, 0);
	//DFS.printByString(4);
	//BreadthFirstSearch BFS(gTiny, 0);
	//BFS.printByString(4);
	//CC cc(gTiny);
	//cout << (cc.connected(7, 9) ? "yes" : "no") << endl;
	//Cycle cycle(gTiny);
	//cout << (cycle.isCycle() ? "yes" : "no") << endl;
	//TwoColor twocolor(gTiny);
	//cout << twocolor.isBipartite() << endl;

	/*Digraph*/
	//ifstream tinyDG("tinyDG.txt");
	//Digraph digraph(tinyDG);
	//digraph.toString();
	//Digraph reDigraph = digraph.reverse();
	//reDigraph.toString();
	//DirectedDFS didfs(digraph, 0);
	//didfs.toString(digraph);
	//DirectedCycle diCycle(digraph);
	//diCycle.cycleToString();
	//ifstream tinyDAG("tinyDAG.txt");
	//Digraph dag(tinyDAG);
	//DepthFirstOrder dfo(dag);
	//dfo.toTopological(dag);
	//SCC scc(digraph);
	//scc.conToString();

	ifstream tinyEWG("tinyEWG.txt");
	EdgeWeightedGraph ewg(tinyEWG);
	ewg.toString();
}