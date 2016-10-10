#include "Tcp_server.h"
#include <iostream>
using namespace std;
using namespace tiny;

void connection(Tcp_connection_ptr conn_ptr)
{
	Internet_addr *peeraddr = conn_ptr->get_peeraddr();
	cout << peeraddr->to_string() << endl;
}

void message(Tcp_connection_ptr conn_ptr)
{
	conn_ptr->send("Hello echo", 10);
}

void error(Tcp_connection_ptr conn_ptr)
{
	cout << "shutdown" << endl;
}

int main()
{
	Reactor reactor;

	Internet_addr addr("0.0.0.0", 2345);
	Tcp_server server(&reactor, addr, "test");
	server.set_connection_callback(connection);
	server.set_message_callback(message);
	server.set_shutdown_callback(error);

	Internet_addr addr2("0.0.0.0", 6666);
	Tcp_server server2(&reactor, addr2, "hello");
	server2.set_connection_callback(connection);
	server2.set_message_callback(message);
	server2.set_shutdown_callback(error);

	server.start();
	server2.start();

	reactor.run();
	return 0;
}
