#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "Blocking_queue.hpp"
#include <functional>
#include <vector>
#include <thread>

namespace tiny
{

class Thread_pool
{
public:
	typedef std::function<void(void)> Task_function;

	Thread_pool(int n);
	Thread_pool(const Thread_pool&) = delete;
	Thread_pool& operator=(const Thread_pool&) = delete;
	~Thread_pool(){}

	void run(Task_function);
	void stop();

	void task();

private:
	Blocking_queue<Task_function> _task_queue;
	int _thread_num;
	std::vector<std::thread> _thread_list;
	bool _stop;
};

}

#endif
