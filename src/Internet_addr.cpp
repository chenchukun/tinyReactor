#include "Internet_addr.h"
#include <cstring>

namespace tiny
{

Internet_addr::Internet_addr(sockaddr_in *sock)
{
	memcpy(&_addr, sock, sizeof(_addr));
}

Internet_addr::Internet_addr(const char *ip, short port)
{
	bzero(&_addr, sizeof(_addr));

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &_addr.sin_addr);
}

const sockaddr* Internet_addr::get_sockaddr() const
{
	return reinterpret_cast<const sockaddr*>(&_addr);
}

string Internet_addr::get_ip() const
{
	char ip[20];
	const char *ret = inet_ntop(AF_INET, (void*)&_addr.sin_addr, ip, 20);
	if(ret == NULL)
		return string();
	return string(ip, strlen(ip));
}

unsigned short Internet_addr::get_port() const
{
	unsigned short port = ntohs(_addr.sin_port);
	return port;
}

string Internet_addr::to_string() const
{
	string ret(get_ip());
	ret += ":";
	ret += std::to_string(get_port());
	return ret;
}

Internet_addr Internet_addr::operator=(const Internet_addr &right)
{
	memcpy(&_addr, &(right._addr), sizeof(_addr));
}

}
