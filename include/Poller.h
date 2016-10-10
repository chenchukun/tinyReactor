#ifndef POLLER_H
#define POLLER_H

#include <map>
#include <vector>

namespace tiny
{

class Event_handler;

class Poller
{
public:
	Poller(const Poller&) = delete;
	Poller& operator=(const Poller&) = delete;
	Poller(){}
	virtual ~Poller(){}
	
	virtual void add_event(Event_handler*) = 0;
	virtual void del_event(Event_handler*) = 0;
	virtual int wait(std::vector<Event_handler*> &v, int t=-1) = 0;
	virtual void update(int fd) = 0;

	/*
protected:
	std::map<int, Event_handler*> _fd_to_handler;
	*/
};
}

#endif
