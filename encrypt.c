#include <stdio.h>
#include <string.h>
#define LEN 102
void Encrypt(char aucPassword[], char aucResult[]) {
	int i = 0;
	while (aucPassword[i] != 0){
		if (aucPassword[i] >= 'a' && aucPassword[i] < 'z') 
			aucResult[i] = aucPassword[i] - 'a' + 'A' + 1;
		else if(aucPassword[i] >= 'A' && aucPassword[i] < 'Z')
			aucResult[i] = aucPassword[i] - 'A' + 'a' + 1;
		else if(aucPassword[i] >= '0' && aucPassword[i] < '9')
			aucResult[i] = aucPassword[i] + 1;
		else if(aucPassword[i] == 'z')
			aucResult[i] = 'A';
		else if(aucPassword[i] == 'Z')
			aucResult[i] = 'a';
		else if(aucPassword[i] == '9')
			aucResult[i] = '0';
		else aucResult[i] = aucPassword[i];
		i++;
	}
	aucResult[i] = 0;
	printf("%s\n", aucResult);
	return;
}

int unEncrypt(char result[], char password[]) {
	int i = 0;
	while (result[i] != 0) {
		if (result[i] > 'a' && result[i] <= 'z')
			password[i] = result[i] - 'a' + 'A' - 1;
		else if (result[i] > 'A' && result[i] <= 'Z')
			password[i] = result[i] - 'A' + 'a' - 1;
		else if (result[i] > '0' && result[i] <= '9')
			password[i] = result[i] - 1;
		else if (result[i] == 'a')
			password[i] = 'Z';
		else if (result[i] == 'A')
			password[i] = 'z';
		else if (result[i] == '0')
			password[i] = '9';
		else password[i] = result[i];
		i++;
	}
	password[i] = 0;
	printf("%s\n", password);
	return 1;
}

int main()
{
	//printf("key in unencrypt password and encrypt password\n");
	char aucPassword[LEN] = { 0 };
	char result[LEN] = { 0 };
	char aucResult[LEN] = { 0 };
	char password[LEN] = { 0 };
	fgets(aucPassword, LEN, stdin);
	fgets(result, LEN, stdin);
	aucPassword[strlen(aucPassword) - 1] = 0;
	result[strlen(result) - 1] = 0;
	//scanf("%s %s", aucPassword, result);
	//printf("answer\n");
	Encrypt(aucPassword, aucResult);
	unEncrypt(result, password);
}