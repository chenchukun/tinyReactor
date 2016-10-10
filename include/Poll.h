#ifndef POLL_H
#define POLL_H

#include "Poller.h"
#include <poll.h>
#include <map>
#include <vector>

namespace tiny
{
/*
 * 对Poll I/O复用系统调用的封装, 非线程安全
 */ 

class Event_handler;

class Poll : public Poller
{
public:
	Poll(const Poll&) = delete;
	Poll& operator=(const Poll&) = delete;
	Poll():_fd_to_handler(){};
	virtual ~Poll(){}
	
	void add_event(Event_handler*);
	void del_event(Event_handler*);
	int wait(std::vector<Event_handler*> &v, int timeout=-1);

	void update(int fd);

private:
	std::vector<struct pollfd> _event_list;
	std::map<int, size_t> _fd_to_index;
	std::map<int, Event_handler*> _fd_to_handler;
};
}

#endif
