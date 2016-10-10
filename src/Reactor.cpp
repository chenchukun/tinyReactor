#include "Reactor.h"
#include "Event_handler.h"
#include "Poll.h"
#include "Epoll.h"
#include <iostream>

namespace tiny
{

Reactor::Reactor()
	: _stop(false), _poller(new Epoll())
{
}

void Reactor::run()
{
	this->add_event(_timer_heap.get_event_handler());
	while(!_stop)
	{
		int n = _poller->wait(_activity_event_list);		
//		std::cout << "Reactor debug: wait return" << std::endl;
		if(n > 0)
		{
			handler_event();
		}
	}
}

void Reactor::stop()
{
	_stop = true;
}
void Reactor::add_event(Event_handler *eh)
{
	_poller->add_event(eh);
}
void Reactor::del_event(Event_handler *eh)
{
	_poller->del_event(eh);
}

void Reactor::handler_event()
{
	for(auto ae : _activity_event_list)
	{
		ae->handler();
	}
}

void Reactor::update(int fd)
{
	_poller->update(fd);
}

void Reactor::run_timer(Time_stamp rt, Time_stamp it, Timer_callback cb)
{
}

void Reactor::run_at(Time_stamp rt, Timer_callback cb)
{
	_timer_heap.add_timer(rt, 0, cb);
}

void Reactor::run_after(Time_stamp rt, Timer_callback cb)
{
	_timer_heap.add_timer(Time_stamp::now()+rt, 0, cb);
}

void Reactor::run_everytime(Time_stamp it, Timer_callback cb)
{
	_timer_heap.add_timer(Time_stamp::now()+it, it, cb);
}

}

