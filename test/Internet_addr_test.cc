#include "Internet_addr.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace tiny;

int main()
{
	struct sockaddr_in addr;
	const char *ip = "192.168.70.1";
	short port = 2345;

	bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);

	Internet_addr internet_addr(&addr);
	cout << internet_addr.get_ip() << endl;
	cout << internet_addr.get_port() << endl;

	Internet_addr ad(ip, port); 
	
	cout << ad.get_ip() << endl;
	cout << ad.get_port() << endl;

	ad = internet_addr;

	cout << ad.get_ip() << endl;
	cout << ad.get_port() << endl;

	return 0;
}
