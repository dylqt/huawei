#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_beautiful_name(char *name) {
	//save name to alph[26]
	int alph[26] = { 0 };
	int len = strlen(name);
	for (int i = 0; i < len; i++) {
		if (name[i] >= 'a' && name[i] <= 'z') {
			int index = name[i] - 'a';
			alph[index]++;
		}
		else if (name[i] >= 'A' && name[i] <= 'Z') {
			int index = name[i] - 'A';
			alph[index]++;
		}
		else break;
	}
	// sort the array
	for (int i = 1; i < 26; i++) {
		int tmp = alph[i];
		int j = i;
		while (j > 0 && tmp > alph[j - 1]) {
			alph[j] = alph[j - 1];
			j--;
		}
		alph[j] = tmp;
	}
	int start_nb = 26;
	int output = 0;
	for (int i = 0; i < len; i++) {
		output += alph[i] * start_nb;
		start_nb--;
	}
	printf("%d\n", output);
}

int main() {
	int num = 0;
	scanf_s("%d", &num);
	char **input = (char **)malloc(num * sizeof(char *));
	for (int i = 0; i < num; i++) {
		input[i] = (char *)malloc(1000 * sizeof(char));
	}
	for (int i = 0; i < num; i++) {
		scanf_s("%s", input[i], 1000);
	}
	for (int i = 0; i < num; i++) {
		get_beautiful_name(input[i]);
	}
}