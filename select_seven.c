#include <stdio.h>
#define TOP 30000
int* select_seven() {
	int *seven = (int *)malloc(TOP * sizeof(int));
	int counts = 0;
	for (int i = 1; i < TOP; i++) {
		//i is about seven: times of seven or include seven
		int include_seven = 0;
		if (i / 7.0 == i / 7) {
			include_seven = 1;
		}
		else {
			int tmp = i;
			while (tmp != 0) {
				if (tmp % 10 == 7) {
					include_seven = 1;
					break;
				}
				else {
					tmp /= 10;
				}
			}
		}
		counts = counts + include_seven;
		seven[i] = counts;
	}
	return seven;
}

int main() {
	int *seven = select_seven();
	int  input = 0;
	scanf("%d", &input);
	printf("%d\n", seven[input]);
}