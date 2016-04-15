#include "std.h"

int main()
{
	char str[128] = {0};
	int count = 0;
	int i = 0;
	char ch = 0;
	do 
	{
		scanf("%s", str);
	} while ((ch = getchar() )== ' ');
		
	printf("%d", strlen(str));
	return 0;
}