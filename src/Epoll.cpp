#include "Epoll.h"
#include "Event_handler.h"

namespace tiny
{

void Epoll::add_event(Event_handler *eh)
{
	struct epoll_event event;
	event.data.fd = eh->get_fd();
	event.events = Event_to_epoll(eh->get_event());

	assert(_fd_to_handler.count(event.data.fd) == 0);

	_fd_to_handler[event.data.fd] = eh;
	
	int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, event.data.fd, &event);
	
	assert(ret != -1);
	_event_list.resize(_event_list.size() + 1);
}

void Epoll::del_event(Event_handler *eh)
{
	assert(_fd_to_handler.count(eh->get_fd()) != 0);

	_fd_to_handler.erase(eh->get_fd());
	
	int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, eh->get_fd(), NULL);

	assert(ret != -1);

	_event_list.resize(_event_list.size() - 1);
}

int Epoll::wait(std::vector<Event_handler*> &eh_v, int timeout)
{
	int fdn = epoll_wait(_epfd, &_event_list[0], _event_list.size(), timeout);

	eh_v.clear();
	if(fdn <= 0)
	{
		return fdn;
	}
	int ret = fdn;

	for(int i=0; i<fdn; ++i)
	{
		struct epoll_event &pfd = _event_list[i];
		assert(_fd_to_handler.count(pfd.data.fd) != 0);
		Event_handler *eh = _fd_to_handler[pfd.data.fd];
		eh->set_revent(Epoll_to_event(pfd.events));
		eh_v.push_back(eh);
	}
	return ret;
}

void Epoll::update(int fd)
{
	assert(_fd_to_handler.count(fd) != 0);

	Event_handler *eh_ptr = _fd_to_handler[fd];

	struct epoll_event event;
	event.data.fd = fd;
	event.events = Event_to_epoll(eh_ptr->get_event());

	int ret = epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event);

	assert(ret != -1);
}

}
