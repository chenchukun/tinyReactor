#include "Accept.h"
#include "Reactor.h"
#include <iostream>
using namespace std;
using namespace tiny;

void newconnection(int connfd, const Internet_addr &addr)
{
	cout << "new connection" << endl;
}

int main()
{
	Reactor reactor;

	Internet_addr addr("0.0.0.0", 2345);
	Accept accept(addr);
	accept.listen();
	accept.set_newconnection_callback(newconnection);

	reactor.add_event(accept.get_event_handler());

	reactor.run();
	return 0;
}
