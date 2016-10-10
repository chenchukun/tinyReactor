#include <iostream>
#include <string>
#include "Tcp_server.h"
#include "Internet_addr.h"
#include "Reactor.h"
using namespace std;
using namespace tiny;


void message(Tcp_connection_ptr conn_ptr)
{
	string msg;
	conn_ptr->read_buffer().read(msg);
	conn_ptr->send(msg);
}


int main()
{
	Reactor reactor;
	Internet_addr addr("0.0.0.0", 2345);

	Tcp_server server(&reactor, addr, "pingpong");
	server.set_message_callback(message);
	server.start();

	reactor.run();
	return 0;
}
