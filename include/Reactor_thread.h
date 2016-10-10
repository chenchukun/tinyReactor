#ifndef REACTOR_THREAD
#define REACTOR_THREAD
#include "Reactor.h"
#include <thread>
#include <memory>
#include <iostream>
#include <mutex>

namespace tiny
{


class Reactor_thread
{
public:
	Reactor_thread():_reactor(new Reactor()){}

	~Reactor_thread()
	{
		delete _reactor;
		delete _thread;
	}
	void run();
	void thread_fun();
	void join()
	{
//		std::cout << "Reactor_thread debug: join()" << std::endl;
		_thread->join();
	}
	void detach()
	{
		_thread->detach();
	}

	void stop();

	void run_at(Time_stamp ts, Timer::Timer_callback cb);

	void run_after(Time_stamp ts, Timer::Timer_callback cb);

	void run_everytime(Time_stamp ts, Timer::Timer_callback cb);

	void add_event(Event_handler *eh);

	void del_event(Event_handler *eh);


private:
//	std::shared_ptr<Reactor> _reactor;
//	std::shared_ptr<std::thread> _thread;
	Reactor *_reactor;
	std::thread *_thread;
	std::mutex _mutex;
};

}

#endif
