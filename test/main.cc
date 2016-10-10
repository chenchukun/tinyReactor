#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <fcntl.h>
#include <string>
#include "Poll.h"
#include "Event_handler.h"
#include "Reactor.h"
#include "Socket.h"
#include "Time_stamp.h"
#include "Timer.h"
#include "queue"
#include "Timer_heap.h"
#include "Reactor_thread.h"
#include "Thread_pool.h"
using namespace std;
using namespace tiny;

class Echo_server
{
public:
	Echo_server(const char*ip, short port):_accept_handler(_socket.get_fd())
	{
		Internet_addr addr(ip, port);
		_socket.bind(addr);
		_socket.listen(10);		
		_accept_handler.set_read_event(true);
		_accept_handler.set_read_callback(bind(&Echo_server::connect_callback, this));

		_reactor.add_event(&_accept_handler);

	}

	~Echo_server()
	{
		for(auto eh_ptr : _eh_v)
		{
			delete eh_ptr;
		}
	}

	void start()
	{
		_reactor.run_after(Time_stamp(3), bind(&Echo_server::timer_callback, this, "run_after(3)"));
		_reactor.run_after(Time_stamp(5), bind(&Echo_server::timer_callback, this, "run_after(5)"));
		_reactor.run_at(Time_stamp::now()+Time_stamp(10), bind(&Echo_server::timer_callback, this, 
					"run_at(10)"));
		_reactor.run_everytime(Time_stamp(6), bind(&Echo_server::timer_callback, this, "run_everytime(6)"));

/*
		timer_heap.add_timer(Time_stamp::now()+Time_stamp(1, 500000),
				Time_stamp(0,0), bind(&Echo_server::timer_callback, this, "timer(1, 0)"));

		timer_heap.add_timer(Time_stamp::now()+Time_stamp(5, 0), 
				Time_stamp(3, 0), bind(&Echo_server::timer_callback, this, "timer(5, 3)"));

		timer_heap.add_timer(Time_stamp::now()+Time_stamp(0, 500000),
				Time_stamp(0,0), bind(&Echo_server::timer_callback, this, "timer(0.5, 0)"));

		_reactor.add_event(timer_heap.get_event_handler());
*/

		_reactor.run();	
	}

private:
	void connect_callback();
	void read_callback(Event_handler*);
	void write_callback(Event_handler*);

	void timer_callback(const char*);

	Socket _socket;
	Event_handler _accept_handler;
	Reactor _reactor;

//	Timer_heap timer_heap;

	vector<Event_handler*> _eh_v;
};

void Echo_server::timer_callback(const char *text)
{
	cout << text << endl;
}

void Echo_server::read_callback(Event_handler *eh)
{
	int fd = eh->get_fd();
	cout << fd << ": read..." << endl;
	string text;
	Socket sock(fd);
	int len = sock.recv(text);
	if(len == 0)
	{
		cout << "connect down" << endl;
		_reactor.del_event(eh);
		return;
	}
	cout << text << endl;
//	eh->set_read_event(false);
//	_reactor.update(eh->get_fd());
}

void Echo_server::write_callback(Event_handler *eh)
{
	cout << "write..." << endl;
	Socket sock(eh->get_fd());
	string text = "Hello Reactor";
	sock.send(text);
	eh->set_write_event(false);
	_reactor.update(eh->get_fd());
}

// 新连接到达
void Echo_server::connect_callback()
{
	Time_stamp stamp = Time_stamp::now();
	cout << "connect: " << stamp.to_formt() << endl;
	int fd = _socket.accept();
	Event_handler* eh_ptr = new Event_handler(fd);
	_eh_v.push_back(eh_ptr);
	eh_ptr->set_read_event(true);
	eh_ptr->set_read_callback(bind(&Echo_server::read_callback, this, eh_ptr));
	eh_ptr->set_write_event(true);
	eh_ptr->set_write_callback(bind(&Echo_server::write_callback, this, eh_ptr));
	_reactor.add_event(eh_ptr);
}

void thread_task()
{
	while(true)
	{
		cout << "thread_task: " << this_thread::get_id() << endl;
		sleep(1);
	}
}


int main()
{
	Reactor_thread r_thread;
	r_thread.run();
	r_thread.detach();

	Event_handler eh(STDIN_FILENO);
	eh.set_read_event(true);
	eh.set_read_callback(bind([]{string s; cin >> s; cout << "thread callback" << endl;}));
	r_thread.add_event(&eh);

	Thread_pool thread_pool(3);
	thread_pool.run(thread_task);

	Echo_server echo_server("0.0.0.0", 2345);
	echo_server.start();
	
	return 0;
}








/*
int main_2()
{
	Reactor reactor;

	Socket s;
	Internet_addr addr("0.0.0.0", 2345);
	s.bind(addr);
	s.listen(10);

	Event_handler event_handler(s.get_fd());
	
	event_handler.set_read_event(true);
//	event_handler.set_read_callback(fun);

	event_handler.set_write_event(true);
//	event_handler.set_write_callback(fun2);

	reactor.add_event(&event_handler);
	reactor.run();
	return 0;
}



int main_1()
{
	Poll loop;
	Event_handler eh(STDIN_FILENO);
	eh.set_read_event(true);
//	eh.set_read_callback(fun);

	loop.add_event(&eh);

	vector<Event_handler*> eh_v;
	cout << loop.wait(eh_v, -1) << endl;

	eh_v[0]->handler();
	return 0;
}

*/
