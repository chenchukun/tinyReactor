#include <poll.h>
#include "Event.h"
#include <sys/epoll.h>

namespace tiny
{

short Event_to_poll(Event ev)
{
	short ret = 0;
	if(ev & EVENTIN)
		ret |= POLLIN;
	if(ev & EVENTOUT)
		ret |= POLLOUT;
	if(ev & EVENTERR)
		ret |= POLLERR;
	if(ev & EVENTHUP)
		ret |= POLLHUP;
	if(ev & EVENTRDHUP)
		ret |= POLLRDHUP;
	if(ev & EVENTPRI)
		ret |= POLLPRI;

	return ret;
}

short Event_to_epoll(Event ev)
{
	short ret = 0;
	if(ev & EVENTIN)
		ret |= EPOLLIN;
	if(ev & EVENTOUT)
		ret |= EPOLLOUT;
	if(ev & EVENTERR)
		ret |= EPOLLERR;
	if(ev & EVENTHUP)
		ret |= EPOLLHUP;
	if(ev & EVENTRDHUP)
		ret |= EPOLLRDHUP;
	if(ev & EVENTPRI)
		ret |= EPOLLPRI;

	return ret;
}

Event Epoll_to_event(short ev)
{
	Event ret = 0;
	if(ev & EPOLLIN)
		ret |= EVENTIN;
	if(ev & EPOLLOUT)
		ret |= EVENTOUT;
	if(ev & EPOLLERR)
		ret |= EVENTERR;
	if(ev & EPOLLHUP)
		ret |= EVENTHUP;
	if(ev & EPOLLRDHUP)
		ret |= EVENTRDHUP;
	if(ev & EPOLLPRI)
		ret |= EVENTPRI;

	return ret;
}

short Poll_to_event(short ev)
{
	Event ret = 0;
	if(ev & POLLIN)
		ret |= EVENTIN;
	if(ev & POLLOUT)
		ret |= EVENTOUT;
	if(ev & POLLERR)
		ret |= EVENTERR;
	if(ev & POLLHUP)
		ret |= EVENTHUP;
	if(ev & POLLRDHUP)
		ret |= EVENTRDHUP;
	if(ev & POLLPRI)
		ret |= EVENTPRI;

	return ret;
}

}
