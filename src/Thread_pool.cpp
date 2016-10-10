#include "Thread_pool.h"
#include <assert.h>

namespace tiny
{

Thread_pool::Thread_pool(int n): _stop(false),
	_thread_num(n)
{
	assert(_thread_num > 0);

	for(int i=0; i<_thread_num; ++i)
	{
		_thread_list.emplace_back(std::bind(&Thread_pool::task, this));
	}
}

void Thread_pool::run(Task_function task_fun)
{
	_task_queue.push(task_fun);
}

void Thread_pool::stop()
{
	_stop = true;
}

void Thread_pool::task()
{
	while(!_stop)
	{
		Task_function task = _task_queue.pop();
		task();
	}
}

}
