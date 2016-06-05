#include "route.h"
#include "lib_io.h"
#include "lib_time.h"
#include "stdio.h"
#include <ctime>
int main(int argc, char *argv[])
{
	clock_t start, end;
	start = clock();
	//print_time("Begin");
	char *topo[5000];
	int edge_num;
	char *demand;
	int demand_num;

	char *topo_file = "./case2/topo.csv";
	edge_num = read_file(topo, 5000, topo_file);
	if (edge_num == 0)
	{
		printf("Please input valid topo file.\n");
		return -1;
	}
	char *demand_file = "./case2/demand.csv";
	demand_num = read_file(&demand, 1, demand_file);
	if (demand_num != 1)
	{
		printf("Please input valid demand file.\n");
		return -1;
	}

	search_route(topo, edge_num, demand);
/*
	char *result_file = argv[3];
	write_result(result_file);
	release_buff(topo, edge_num);
	release_buff(&demand, 1);

	//print_time("End");
*/
	end = clock();
	double duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf_s("time: %f s\n", duration);
	return 0;
}

