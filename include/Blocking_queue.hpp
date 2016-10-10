#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

namespace tiny
{

template<typename T>
class Blocking_queue
{
public:
	void push(T val);
	T pop();

private:
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<T> _queue;
};

template<typename T>
void Blocking_queue<T>::push(T val)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_queue.push(val);
	_cond.notify_one();
}

template<typename T>
T Blocking_queue<T>::pop()
{
	std::queue<T> *q = &_queue;
	std::unique_lock<std::mutex> locker(_mutex);
	_cond.wait(locker, [&q]{return !q->empty();});

	T ret = _queue.front();
	_queue.pop();
	return ret;
}



}

#endif
