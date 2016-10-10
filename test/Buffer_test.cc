#include <iostream>
#include "Buffer.h"
using namespace std;
using namespace tiny;

int main()
{
	Buffer buffer;
	cout << buffer.size() << endl;
	buffer.write("Hello");
	cout << buffer.size() << endl;

	int x = 10;
	buffer.write(&x, sizeof(x));
	cout << buffer.size() << endl;

	char buf[1024];
	buffer.read(buf, 2);
	cout << buffer.size() << endl;

	string text;
	buffer.read(text, 3);
	cout << text << endl;
	cout << buffer.size() << endl;

	string num;
	buffer.read(num);
	int *tp = (int*)&num[0];
	int t = *tp;
	cout << t << endl;
	cout << buffer.size() << endl;

	return 0;
}
