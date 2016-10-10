#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <string>
#include <functional>

namespace tiny
{

class Buffer
{
public:

	Buffer():_init_len(512), _buf(_init_len)
		,_begin_index(0), _end_index(0), _write_callback(NULL){}
	~Buffer(){}
	Buffer(const Buffer&) = delete;
	Buffer operator=(const Buffer&) = delete;

	size_t size();
	char* head();
	void discard_nbyte(size_t n);

	void read(void *buf, size_t n);
	void write(const void *buf, size_t n);

	void read(std::string &buf);
	void write(const std::string &buf);
	
	void read(std::string &buf, size_t n);

	void set_write_callback(std::function<void(void)> cb)
	{
		_write_callback = cb;
	}

private:
	const size_t _init_len;
	std::vector<char> _buf;
	size_t _begin_index;
	size_t _end_index;
	std::function<void(void)> _write_callback;

};

}

#endif
