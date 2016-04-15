#include <stdio.h>
#define N 1000
int removeNum(int input[], int num) {
	int *left = (int *)malloc(num * sizeof(int));
	int *right = (int *)malloc(num * sizeof(int));
	int output = 0;
	left[0] = 0;
	right[num - 1] = 0;
	if (num < 3)
		return 0;
	// when the one is max, get the less nums
	for (int i = 1; i < num; i++) {
	// check all ; get the max less num
		int max_num = 0;
		for (int j = i - 1; j >= 0; j--) {
			if (input[i] > input[j]) {
				if (max_num < left[j] + 1) {
					max_num = left[j] + 1;
					
				}
			}
		}
		left[i] = max_num;
	}
	for (int i = num - 2; i >= 0; i--) {
		// check all ; get the max less num
		int max_num = 0;
		for (int j = i + 1; j < num; j++) {
			if (input[i] > input[j]) {
				if (max_num < right[j] + 1) {
					max_num = right[j] + 1;
				}
			}
		}
		right[i] = max_num;
	}
	for (int i = 0; i < num; i++) {
		int max_num = output;
		if (left[i] + right[i] > max_num) {
			max_num = left[i] + right[i];
			output = max_num;
		}
	}
	return num - output - 1;
}
int main()
{
	int input[N] = {0};
	int len = 0;
	int num = 0;
	int tmp = scanf("%d", &num);
	do
	{
		scanf("%d", input + len);
		len++;
	} while (getchar() == ' ');
	printf("%d\n", removeNum(input, len));
}