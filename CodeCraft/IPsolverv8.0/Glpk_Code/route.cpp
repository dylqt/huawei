#include "route.h"
#include "lib_record.h"
#include "glpk.h"
#include <stdio.h>
#include <sstream>

int start;
int terminial;
int verNum = 0;
int specialNum = 0;
vector<int> specialNodes;


int *dist; 		//dist[n*n];保存边的权值
glp_prob *P;	//建立库环境

int ioc(int i, int j)	//求[i,j]在矩阵中的索引
{     
    return i * verNum + j + 1;
}

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	initGraph(topo,edge_num,demand);
	//toString();
	build_prob();
	
	glp_iocp iocp;
	glp_init_iocp(&iocp);
	iocp.br_tech = GLP_BR_MFV; /* most fractional variable */
    iocp.bt_tech = GLP_BT_BLB; /* best local bound */
   // iocp.sr_heur = GLP_OFF;    /* disable simple rounding heuristic */
  //  iocp.gmi_cuts = GLP_ON;    /* enable Gomory cuts */
   // iocp.cb_func = cb_func;
    glp_intopt(P, &iocp);
    assert(glp_simplex(P,NULL) == 0);
    assert(glp_get_status(P) == GLP_OPT);
    int bestCost = glp_get_obj_val(P);
    cout << "bestcost " << bestCost << endl;
	
}


void initGraph(char *topo[5000], int edge_num, char *demand)
{
	verNum = 0;
	specialNum = 0;

	int index, from, to, weigh;
	char tmp;
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> index >> tmp >> from >> tmp >> to >> tmp >> weigh;

		if (from >= verNum)
			verNum = from + 1;
		if (to >= verNum)
			verNum = to + 1;
	}
	//cout << "46 "<<endl;
	dist = (int*)malloc((1+verNum*verNum)*sizeof(int));
	memset(dist,-1,(1+verNum*verNum)*sizeof(int));
	//for(int i=0;i<verNum*verNum+1;i++)
	//	cout << dist[i]<<" " ;
	cout <<endl;
	for (int i = 0; i < edge_num; i++) {
		stringstream ss;
		ss << topo[i];
		ss >> index >> tmp >> from >> tmp >> to >> tmp >> weigh;
		//cout << from << " -> " << to << " ioc "<< ioc(from,to)<<endl;
		if(dist[ioc(from,to)] != -1)	//如果有边了
		{
		//	cout << from << " -> " << to << " ioc "<< dist[ioc(from,to)]<<endl;
			if(dist[ioc(from,to)] > weigh)
				dist[ioc(from,to)] = weigh;
		}
		else
			dist[ioc(from,to)] = weigh;
	}
	//cout << "62 "<<endl;
	stringstream ss;
	ss << demand;
	ss >> start >> tmp >> terminial >> tmp;
	do {
		ss >> from;
		specialNodes.push_back(from);
		specialNum++;
	} while (ss >> tmp);
}

void toString()
{
	cout << "start " << start << "  "<<"terminial "<< terminial <<endl;
	cout << "verNum "<<verNum<<endl;
	cout << "specialNodes "<<endl;
	for(int i=0;i<specialNum;i++)
		cout << specialNodes[i] << "|";
	cout <<endl;
	for(int i=0;i<verNum;i++)
	{
		for(int j=0;j<verNum;j++)
		{
			if(dist[ioc(i,j)] != -1)
				cout << i << " -> " << j << " dist "<< dist[ioc(i,j)] <<endl;
		}
	}
}

/* 建立库环境 */
void build_prob(void)
{
	int k;
	int *ind;
	double *val;
	int colnum = 0,rownum=0;
	char name[50];
	P = glp_create_prob();
	glp_set_obj_dir(P, GLP_MIN);
	glp_set_prob_name(P, "huawei");
	/* add all binary variables 限制条件 6*/
	for(int i = 0;i < verNum;i++)
	{
		for(int j = 0;j<verNum;j++)
		{
			//if(i == j)
			//	continue;
			glp_add_cols(P,1);
			colnum++;
			//cout << "colnum "<<colnum<<endl;
			assert(colnum == ioc(i,j));
			sprintf(name,"x[%d,%d]",i,j);
			glp_set_col_name(P, colnum, name);
            glp_set_col_kind(P, colnum, GLP_BV);

            /*限制条件 c[k]是限制条件4 */
            glp_set_obj_coef(P, colnum, dist[ioc(i,j)]);
		}
	}

	ind = (int*)malloc((colnum+1)*sizeof(int));
	memset(ind,0,(colnum+1)*sizeof(int));
	val = (double*)malloc((colnum+1)*sizeof(double));
	
	/* 限制条件 5 */
	for(int i = 1; i <= verNum; i++)
	{
		for(int z=1;z<=colnum;z++)  //初始化ind
			ind[z] = z;
		memset(val,0.0,(colnum+1)*sizeof(double));
		rownum = glp_add_rows(P,1);
		assert(rownum == i);
		sprintf(name,"conStr_%d",i);
		glp_set_row_name(P,i,name);

		if(i-1 == start)
			glp_set_row_bnds(P,i,GLP_FX,1,1);
		else if(i-1 == terminial)
			glp_set_row_bnds(P,i,GLP_FX,-1,-1);
		else
			glp_set_row_bnds(P,i,GLP_FX,0,0);

		for(int j = 0;j < verNum;j++)
		{
			if(dist[ioc(i-1,j)] != -1)		//inDegree
				val[ioc(i-1,j)] = 1;
			else if(dist[ioc(j,i-1)] != -1)		//outDegree
				val[ioc(j,i-1)] = -1;
		}

#ifdef DEBUG
		//for(int i=1;i<=colnum;i++)
		//	cout << " ind "<< ind[i] << " val "<<val[i]<<endl;
#endif
		glp_set_mat_row(P,i,colnum,ind,val);
	}

	/*出度限制条件*/
	for(int i = 1;i<=verNum;i++)
	{
		for(int z=1;z<=colnum;z++)  //初始化ind
			ind[z] = z;	
		memset(val,0.0,(colnum+1)*sizeof(double));
		rownum = glp_add_rows(P,1);
		assert(rownum == i+verNum);
		sprintf(name,"conStr2_%d",rownum);
		glp_set_row_name(P,i+verNum,name);

		if(i-1 == start)
			glp_set_row_bnds(P,i+verNum,GLP_FX,1,1);
		else if(i-1 == terminial)
			glp_set_row_bnds(P,i+verNum,GLP_FX,0,0);
		else if(find(specialNodes.begin(),specialNodes.end(),i-1) != specialNodes.end()) //Spec Nodes
			glp_set_row_bnds(P,i+verNum,GLP_FX,1,1);
		else
			glp_set_row_bnds(P,i+verNum,GLP_DB,0,1);

		for(int j = 0;j < verNum;j++)
		{
			if(dist[ioc(i-1,j)] != -1)		//inDegree
				val[ioc(i-1,j)] = 1;
		}
		glp_set_mat_row(P,i+verNum,colnum,ind,val);
	}

	free(ind);
	free(val);
	return;
}

void build_res(void)
{

}