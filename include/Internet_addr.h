#ifndef INTERNET_ADDR_H
#define INTERNET_ADDR_H
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
using std::string;

namespace tiny
{

class Internet_addr
{
public:
	Internet_addr() = default;
	Internet_addr(sockaddr_in *sock);
	Internet_addr(const char *ip, short port);

	Internet_addr operator=(const Internet_addr&);

	const sockaddr* get_sockaddr() const;
	string get_ip() const;
	unsigned short get_port() const;
	string to_string() const;

private:
	sockaddr_in _addr;
};

}

#endif
