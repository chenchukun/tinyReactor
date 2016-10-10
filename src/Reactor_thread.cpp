#include "Reactor_thread.h"
#include <thread>
#include "Time_stamp.h"

namespace tiny
{

void Reactor_thread::run()
{
	_thread = new std::thread(std::bind(&Reactor_thread::thread_fun, this));
//	_thread->detach();
}

void Reactor_thread::thread_fun()
{
	_reactor->run();
}

void Reactor_thread::stop()
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->stop();
}

void Reactor_thread::run_at(Time_stamp ts, Timer::Timer_callback cb)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->run_at(ts, cb);
}

void Reactor_thread::run_after(Time_stamp ts, Timer::Timer_callback cb)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->run_after(ts, cb);
}

void Reactor_thread::run_everytime(Time_stamp ts, Timer::Timer_callback cb)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->run_everytime(ts, cb);
}

void Reactor_thread::add_event(Event_handler *eh)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->add_event(eh);
}

void Reactor_thread::del_event(Event_handler *eh)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_reactor->del_event(eh);
}

}
