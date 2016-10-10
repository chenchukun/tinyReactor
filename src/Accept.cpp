#include "Accept.h"
#include <assert.h>
#include <unistd.h>

namespace tiny
{

Accept::Accept(const Internet_addr &addr):_listen_socket(), backlog(100),
	_accept_event(_listen_socket.get_fd()), _callback(NULL), _listenning(false)
{
	_listen_socket.set_reuseaddr(true);
	_listen_socket.bind(addr);
	_listen_socket.set_nonblock(true);
	_accept_event.set_read_callback(std::bind(&Accept::handler, this));
}

bool Accept::listenning()
{
	return _listenning;
}

void Accept::listen()
{
	_listen_socket.listen(backlog);
	_accept_event.set_read_event(true);
	_listenning = true;
}

void Accept::set_newconnection_callback(Newconnection_callback cb)
{
	_callback = cb;
}

Event_handler* Accept::get_event_handler()
{
	return &_accept_event;
}

void Accept::handler()
{
	Internet_addr peer_addr;

	int connfd = _listen_socket.accept(&peer_addr);
	if(connfd >= 0)
	{
		if(_callback != NULL)
		{
			_callback(connfd, peer_addr);
		}
		else
		{
			::close(connfd);
		}
	}
}

}
