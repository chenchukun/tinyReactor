#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <memory>
#include "Timer_heap.h"
#include "Time_stamp.h"

namespace tiny
{
/*
 * 事件循环类，包含一个事件循环
 * 该类为非线程安全
 */ 

class Poller;
class Event_handler;

class Reactor
{
public:
	typedef std::function<void(void)> Timer_callback;

	Reactor();
	Reactor(const Reactor&) =delete;
	Reactor& operator=(const Reactor&) = delete;

	~Reactor(){}

	void run();
	void stop();
	void add_event(Event_handler*);
	void del_event(Event_handler*);
	void update(int fd);

	void run_at(Time_stamp, Timer_callback);
	void run_after(Time_stamp, Timer_callback);
	void run_everytime(Time_stamp, Timer_callback);

private:

	void handler_event();
	void run_timer(Time_stamp, Time_stamp, Timer_callback);

	std::vector<Event_handler*> _activity_event_list;
	std::shared_ptr<Poller> _poller;
	bool _stop;
	Timer_heap _timer_heap;
};

}

#endif
