#include <stdio.h>
#include <string.h>
void insert_sort(char *input, int len) {
	for (int i = 1; i < len; i++) {
		int j = i;
		char bkup = input[i];
		while (bkup < input[j - 1]) {
			input[j] = input[j - 1];
			j--;
		}
		input[j] = bkup;
	}
	printf("%s\n", input);
	return;
}

int main()
{
	char input[1025] = { 0 };
	scanf("%s", input);
	int len = strlen(input);
	insert_sort(input, len);
	
}