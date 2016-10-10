#ifndef EPOLL_H
#define EPOLL_H

#include "Poller.h"
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <assert.h>

namespace tiny
{
/*
 * 对Poll I/O复用系统调用的封装, 非线程安全
 */ 

class Event_handler;

class Epoll : public Poller
{
public:
	Epoll(const Epoll&) = delete;
	Epoll& operator=(const Epoll&) = delete;
	Epoll():_fd_to_handler()
	{
		_epfd = epoll_create(100);
		assert(_epfd != -1);
	}
	virtual ~Epoll(){}
	
	void add_event(Event_handler*);
	void del_event(Event_handler*);
	int wait(std::vector<Event_handler*> &v, int timeout=-1);

	void update(int fd);

private:
	int _epfd;
	std::map<int, Event_handler*> _fd_to_handler;
	std::vector<struct epoll_event> _event_list;
};
}

#endif
