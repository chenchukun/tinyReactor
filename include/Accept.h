#ifndef ACCEPT_H
#define ACCEPT_H

#include "Internet_addr.h"
#include "Socket.h"
#include "Event_handler.h"
#include <functional>

namespace tiny
{

class Accept
{
public:
	typedef std::function<void(int, const Internet_addr&)> Newconnection_callback;

	Accept(const Internet_addr&);
	~Accept()
	{
		_listen_socket.shutdown();
	}
	bool listenning();
	void listen();
	void set_newconnection_callback(Newconnection_callback);
	Event_handler* get_event_handler();

private:
	void handler();

	Socket _listen_socket;
	const uint32_t backlog = 100;
	Event_handler _accept_event;
	Newconnection_callback _callback;
	bool _listenning;
};

}

#endif
