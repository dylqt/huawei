#include <stdio.h>
#include <stdlib.h>

void encrypt(char * key, char * data, char * encrypt) {
	// map A - Z to key
	char alpha_map[26] = { 0 };
	int alpha_marked[26] = { 0 };
	int index = 0;
	char alpha_left = 'A';
	while (*key != 0) {
		if (*key >= 'a' && *key <= 'z') {
			if (alpha_marked[*key - 'a'] == 0) {
				alpha_map[index] = *key - 'a' + 'A';
				alpha_marked[*key - 'a'] = 1;
				index++;
			}
		}
		if (*key >= 'A' && *key <= 'Z') {
			if (alpha_marked[*key - 'A'] == 0) {
				alpha_map[index] = *key;
				alpha_marked[*key - 'A'] = 1;
				index++;
			}
		}
		key++;
	}
	while (index < 26) {
		if (alpha_marked[alpha_left - 'A'] == 0) {
			alpha_map[index] = alpha_left;
			alpha_marked[alpha_left - 'A'] = 1;
			index++;
		}
		alpha_left++;
	}
	int i = 0;
	while (*data) {
		if (*data >= 'a' && *data <= 'z') {
			encrypt[i] = alpha_map[*data - 'a'] - 'A' + 'a';
		}
		else if (*data >= 'A' && *data <= 'A') {
			encrypt[i] = alpha_map[*data - 'A'];	
		}
		data++;
		i++;
	}
}
int main()
{
	char key[100] = {0};
	char data[100] = {0};
	char out[100] = {0};
	scanf("%s %s", key, data);
	encrypt(key, data, out);
	printf("%s", out);
}