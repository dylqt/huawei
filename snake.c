#include <stdio.h>
#include <stdlib.h>

void GetResult(int Num, int * pResult) {
	int first_num = 1;
	int index = 0; // index

	// row from 0 to Num 
	for (int row = 0; row < Num; row++) {
		// col from 0 to Num - row
		int inc_factor = 0;
		int work = first_num;
		for (int col = 0; col < Num - row; col++) {
			// reset a increase factor : 2 + row + col
			pResult[index] = work + inc_factor;
			inc_factor = 2 + row + col;
			work = pResult[index];
			if (col != Num - row - 1)
				printf("%d ", pResult[index]);
			else
				printf("%d", pResult[index]);
			index++;
		}
		// reset first_num : last first_num + row
		first_num += row + 1;
		if(row != Num - 1)
			printf("\n");
	}
}

int main() 
{
	int Num = 0;
	scanf("%d", &Num);
	int sum_Num = (Num + 1) * Num / 2;
	int *pResult = (int *)calloc(sum_Num, sizeof(int));
	GetResult(Num, pResult);
	free(pResult);
	return 0;
}