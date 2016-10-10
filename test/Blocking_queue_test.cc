#include <iostream>
#include <thread>
#include "Blocking_queue.hpp"
using namespace std;

tiny::Blocking_queue<int> que;

void fun1()
{
	int n;
	while(cin >> n)
	{
		for(int i=0; i<n; ++i)
		{
			cout << "fun1: " << i << endl;
			que.push(i);
		}
	}
}

void fun2()
{
	while(true)
	{
		int x = que.pop();
		cout << "fun2: " << x << endl;
	}
}

void fun3()
{
	while(true)
	{
		int x = que.pop();
		cout << "fun3: " << x << endl;
	}
}

int main()
{
	thread t1(fun1);	
	thread t2(fun2);	
	thread t3(fun3);	
	t2.detach();
	t3.detach();
	t1.join();
	return 0;
}
