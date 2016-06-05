#include "gurobi_c++.h"
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>


string itos(int i) {stringstream s; s << i; return s.str(); }





// Given an integer-feasible solution 'sol', find the smallest
// sub-tour.  Result is returned in 'tour', and length is
// returned in 'tourlenP'.

void
findsubtour(double** sol,
			vector<int> &vTour,
            vector<int> &idTour,
            int *cost,
            Graph &graph)
{
	int from = graph.start;
	int to = graph.terminial;
	int weight = 0;

	vTour.push_back(from);
	while(from != to){		
		for (int i = 0; i < graph.verNum; ++i)
		{
			if(sol[from][i] != 0){
				idTour.push_back(graph.idTable[from][i]);
				weight += graph.weightTable[from][i];
				from = i;
				vTour.push_back(from);
				break;
			}
		}
	}
	*cost = weight;
	assert(vTour.size() == idTour.size() + 1);
}

// Subtour elimination callback.  Whenever a feasible solution is found,
// find the smallest subtour, and add a subtour elimination constraint
// if the tour doesn't visit every node.

/*class subtourelim: public GRBCallback
{

  public:
    GRBVar** vars;
    Graph graph;
    subtourelim(GRBVar** xvars, Graph &graph) {
      vars = xvars;
      this->graph = graph;
    }
  protected:
    void callback() {
      	try {
        if (where == GRB_CB_MIPSOL) {
          // Found an integer feasible solution - does it visit every node?
          	double **x = new double*[graph.verNum];
         	vector<int> vTour;
          	vector<int> idTour;
          	int cost;
          
          	for (int i = 0; i < graph.verNum; i++)
            	x[i] = getSolution(vars[i], graph.verNum);
cout << 77 << endl;
          	findsubtour(x, vTour, idTour, &cost, graph);

          	cout << "get in callback " << endl;
            // Add subtour elimination constraint
	        GRBLinExpr expr = 0;
	        for (int i = 0; i < (int)idTour.size(); i++)
	          	expr += vars[vTour[i]][vTour[i + 1]] * x[i][i + 1];
	cout << 85 << endl;
            addLazy(expr <= cost);

          	for (int i = 0; i < graph.verNum; i++)
            	delete[] x[i];
          	delete[] x;
        	}
	    } catch (GRBException e) {
	        cout << "Error number: " << e.getErrorCode() << endl;
	        cout << e.getMessage() << endl;
	    } catch (...) {
	        cout << "Error during callback" << endl;
	    }
    }
};
*/

void Gurobi(Graph &graph)
{
	/* initial */
	int verNum = graph.verNum;

	clock_t start = 0;
	start = clock();

	GRBEnv *env = NULL;
  	GRBVar **idVars = NULL;
  	GRBVar **weightVars = NULL;
cout << 54 << endl;
  	idVars = new GRBVar*[verNum];
  	weightVars = new GRBVar* [verNum];
  	for (int i = 0; i < verNum; i++){
    	idVars[i] = new GRBVar[verNum];
    	weightVars[i] = new GRBVar[verNum];
  	}

	try{
		env = new GRBEnv();
    	GRBModel model = GRBModel(*env);

		// Must set LazyConstraints parameter when using lazy constraints
		
		model.getEnv().set(GRB_IntParam_LazyConstraints, 1);

		// Create binary decision variables & weight variables
cout << 71 << endl;
	    for (int i = 0; i < verNum; i++) {
	      for (int j = 0; j < verNum; j++) {
	        if(graph.idTable[i][j] == -1)
				idVars[i][j] = model.addVar(0, 0, 0, GRB_BINARY, "id_"+itos(i)+"_"+itos(j));
			else
				idVars[i][j] = model.addVar(0, 1, 0, GRB_BINARY, "id_"+itos(i)+"_"+itos(j));
			weightVars[i][j] = model.addVar(0, 0, graph.weightTable[i][j], GRB_CONTINUOUS, "weight_"+itos(i)+"_"+itos(j));
	      }
	    }
	    model.update();
	    model.update();
	    // Degree constraints 1 (5)
cout << 85 << endl;
	    for (int i = 0; i < verNum; i++) {
      		GRBLinExpr exprLhs = 0;
      		GRBLinExpr exprRhs = 0;
		    for (int j = 0; j < verNum; j++){
		        exprLhs += idVars[i][j];
		        exprRhs += idVars[j][i];
		    }
		    if (i == graph.start)
		    	model.addConstr(exprLhs - exprRhs == 1, "constr1.1_"+itos(i));
		    else if (i == graph.terminial)
		    	model.addConstr(exprLhs - exprRhs == -1, "constr1.2_"+itos(i));
		    else
		    	model.addConstr(exprLhs - exprRhs == 0, "constr1.3_"+itos(i));
		    model.addConstr(exprLhs <= 1, "constr1.4_"+itos(i));
	    }

	    // Degree constraints 2 (6)
cout << 103 << endl;
	     for (int i = 0; i < graph.specialNum; i++) {
      		GRBLinExpr expr = 0;
      		int index = graph.specialNodes[i];   		
		    for (int j = 0; j < verNum; j++){
		        expr += idVars[index][j];
		    }
		    model.addConstr(expr == 1, "constr2_"+itos(i));
	    }



	    // Degree constraints 3 (1)
cout << 173 << endl;
		GRBLinExpr expr = 0;
	     for (int i = 0; i < verNum; i++) {   		
		    for (int j = 0; j < verNum; j++){
		        expr += idVars[i][j] * graph.weightTable[i][j];
		    } 
	    }
		model.setObjective(expr, GRB_MINIMIZE);

		// Set callback function

 		//subtourelim cb = subtourelim(weightVars, graph);
 		//model.setCallback(&cb);
	    
	    model.optimize();
	    	
      
		if (model.get(GRB_IntAttr_SolCount) > 0) {
	    	double **sol = new double*[verNum];
	      	for (int i = 0; i < verNum; i++){
	        	sol[i] = model.get(GRB_DoubleAttr_X, idVars[i], verNum);
			}

			vector<int> vTour;
			vector<int> idTour;
			int cost;
		    findsubtour(sol, vTour, idTour, &cost, graph);

		    cout << "cost: " << cost << endl;
		    cout << "vTour: ";
		    for (int i = 0; i < (int)vTour.size(); i++)
		        cout << vTour[i] << " ";
		    cout << endl;
		    cout << "idTour: ";
		    for (int i = 0; i < (int)idTour.size(); i++)
		        cout << idTour[i] << " ";
		    cout << endl;

		    for (int i = 0; i < verNum; i++)
		        delete[] sol[i];
		    delete[] sol;
	    }
	    


	}catch (GRBException e) {
    	cout << "Error number: " << e.getErrorCode() << endl;
    	cout << e.getMessage() << endl;
  	} catch (...) {
    	cout << "Error during optimization" << endl;
  	}

  	for (int i = 0; i < verNum; i++){
    	delete[] idVars[i];
    	delete[] weightVars[i];
  	}
  	delete[] idVars;
  	delete[] weightVars;
  	delete env;

}

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	Graph deal;
	deal.initGraph(topo, edge_num, demand);
	//deal.toString();
	Gurobi(deal);
		//unsigned short result[] = { 2, 6, 3 };//示例中的一个解

		//for (int i = 0; i < 3; i++)
		//	record_result(result[i]);
}

void Graph::initGraph(char * topo[5000], int edge_num, char * demand)
{
	verNum = 0;
	specialNum = 0;

	int index, from, to, dist;
	char tmp;
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> index >> tmp >> from >> tmp >> to >> tmp >> dist;

		if (from >= verNum)
			verNum = from + 1;
		if (to >= verNum)
			verNum = to + 1;
	}

	weightTable = vector<vector<int> >(verNum, vector<int>(verNum, MAX_INT));
	idTable = vector<vector<int> >(verNum, vector<int>(verNum, -1));
	
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> index >> tmp >> from >> tmp >> to >> tmp >> dist;
		
		weightTable[from][to] = dist;
		idTable[from][to] = index;
	}

	stringstream ss;
	ss << demand;
	ss >> start >> tmp >> terminial >> tmp;
	do {
		ss >> from;
		specialNodes.push_back(from);
		specialNum++;
	} while (ss >> tmp);
}

void Graph::toString()
{
	cout << "verNum : " << verNum << endl;					//所有顶点的数目
	cout << "specialNum : " << specialNum << endl;;				//必经节点的数目
	cout << "start : " << start << endl;					//起点
	cout << "terminial : " << terminial << endl;				//终点
	cout << "specialNodes :";

	assert(specialNum == (int)specialNodes.size());
	for (int i = 0; i < specialNum; i++)	//必经点集合
	{
		cout << " " << specialNodes[i];
	}
	cout << endl;

	cout << "edgeTable : " << endl;		//存储图的邻接表
	for (int i = 0; i < verNum; i++)
	{
		for (int j = 0; j < verNum; j++)
		{
			cout << idTable[i][j] << " : " << weightTable[i][j] << " | ";
		}
		cout << endl;
	}
}

