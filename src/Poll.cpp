#include "Poll.h"
#include "Event_handler.h"
#include <poll.h>
#include <assert.h>
#include <iostream>

namespace tiny
{

void Poll::add_event(Event_handler* eh)
{
	struct pollfd pfd;
	pfd.fd = eh->get_fd();

	// 判断是否重复添加
	assert(_fd_to_handler.count(pfd.fd) == 0);
	
	pfd.events = Event_to_poll(eh->get_event());

	_event_list.push_back(pfd);

	_fd_to_handler[pfd.fd] = eh;

	_fd_to_index[pfd.fd] = _event_list.size() - 1;
	
}

void Poll::del_event(Event_handler* eh)
{
	assert(_fd_to_handler.count(eh->get_fd()) != 0);

	for(auto it=_event_list.begin(); it!=_event_list.end(); ++it)
	{
		if(it->fd == eh->get_fd())
		{
			_event_list.erase(it);
			break;
		}
	}
	_fd_to_handler.erase(eh->get_fd());
	_fd_to_index.erase(eh->get_fd());
}

int Poll::wait(std::vector<Event_handler*> &eh_v, int timeout)
{
	int fdn = poll(&_event_list[0], _event_list.size(), timeout);

	eh_v.clear();
	if(fdn <= 0)
	{
		return fdn;
	}

	int ret = fdn;

	for(int i=0; i<_event_list.size() && fdn>0; ++i)
	{
		struct pollfd &pfd = _event_list[i];
		if(pfd.revents != 0)
		{
			assert(_fd_to_handler.count(pfd.fd) != 0);
			Event_handler *eh = _fd_to_handler[pfd.fd];
			eh->set_revent(Poll_to_event(pfd.revents));
			eh_v.push_back(eh);
			--fdn;
		}
		pfd.revents = 0;
	}

	return ret;
}

void Poll::update(int fd)
{
	assert(_fd_to_index.count(fd) > 0);
//	std::cout << "Poll debug: update" << std::endl;

	size_t index = _fd_to_index[fd];
	Event_handler *eh_ptr = _fd_to_handler[fd];
	struct pollfd &pfd = _event_list[index];
//	std::cout << "Poll debug: update " << pfd.events << std::endl;
	pfd.events = Event_to_poll(eh_ptr->get_event());
//	std::cout << "Poll debug: update " << pfd.events << std::endl;
}

}

