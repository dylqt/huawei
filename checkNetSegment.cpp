#include <string>
#include <iostream>

using namespace std;

int checkNetSegment(string mask, string ip1, string ip2)
{
	int mask_i = 0, ip1_i = 0, ip2_i = 0;
	int mask_n = 0, ip1_n = 0, ip2_n = 0;
	int output = 0;
	do{

		while (mask[mask_i] != '.' && mask[mask_i] != NULL) {
			mask_n = mask_n * 10 + mask[mask_i] - '0';
			mask_i++;
		}
		while (ip1[ip1_i] != '.' && ip1[ip1_i] != NULL) {
			ip1_n = ip1_n * 10 + ip1[ip1_i] - '0';
			ip1_i++;
		}
		while (ip2[ip2_i] != '.' && ip2[ip2_i] != NULL) {
			ip2_n = ip2_n * 10 + ip2[ip2_i] - '0';
			ip2_i++;
		}
		if (mask_n > 255 || ip1_n > 255 || ip2_n > 255 ||
			mask_n < 0 || ip1_n < 0 || ip2_n < 0)
			return 1;
		if ((mask_n & ip1_n) != (mask_n & ip2_n))
			output = 2;
		
		mask_n = 0;
		ip1_n = 0;
		ip2_n = 0;
	} while (mask[mask_i++] != NULL && ip1[ip1_i++] != NULL && ip2[ip2_i++] != NULL);
	
	return output;
}

int main()
{
	string mask = "255.255.255.0";
	string ip1 = "192.168.1.1";
	string ip2 = "192.168.1.2";
	cin >> mask >> ip1 >> ip2;
	int result = checkNetSegment(mask, ip1, ip2);
	cout << result;
}