#include <iostream>
#include <string>
#include <functional>
#include "Tcp_server.h"
#include "Internet_addr.h"
using namespace std;
using namespace tiny;

class Echo_server
{
public:
	Echo_server(Reactor *r, Internet_addr &addr)
		:_reactor(r), _server(r, addr, "echo")
	{
		_server.set_message_callback(bind(&Echo_server::message, this, placeholders::_1));
		_server.set_shutdown_callback(bind(&Echo_server::offline, this, placeholders::_1));
	}

	void start()
	{
		_server.start();
	}

private:
	Reactor *_reactor;
	Tcp_server _server;

	void message(Tcp_connection_ptr);
	void offline(Tcp_connection_ptr);
};

void Echo_server::offline(Tcp_connection_ptr conn_ptr)
{
	cout << conn_ptr->get_peeraddr()->to_string() << ": offline" << endl;
}

void Echo_server::message(Tcp_connection_ptr conn_ptr)
{
	string msg;
	conn_ptr->read_buffer().read(msg);

	cout << conn_ptr->get_peeraddr()->to_string() << ": " << msg << endl;
	conn_ptr->send(msg);
}

int main()
{
	Reactor reactor;
	Internet_addr addr("0.0.0.0", 2345);

	Echo_server echo(&reactor, addr);
	echo.start();

	reactor.run();

	return 0;
}
