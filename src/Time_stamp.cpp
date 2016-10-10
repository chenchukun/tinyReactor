#include "Time_stamp.h"
#include <sys/time.h>
#include <assert.h>
#include <cstring>
#include <time.h>

namespace tiny
{
std::string Time_stamp::to_string() const
{
	std::string ret = std::to_string(_microsecond / MICRO_SECOND);
	ret += ".";
	ret += std::to_string(_microsecond % MICRO_SECOND);
	return ret;
}

std::string Time_stamp::to_formt() const
{
	char buf[27];
	time_t t = _microsecond/MICRO_SECOND;
	ctime_r(&t, buf);
	return std::string(buf, strlen(buf)-1);
}

uint64_t Time_stamp::get_microsecond() const
{
	return _microsecond;
}

bool Time_stamp::operator==(const Time_stamp &stamp) const 
{
	return _microsecond==stamp._microsecond;
}

bool Time_stamp::operator!=(const Time_stamp &stamp) const 
{
	return !(_microsecond==stamp._microsecond);
}

bool Time_stamp::operator>(const Time_stamp &stamp) const 
{
	return _microsecond>stamp._microsecond;
}

bool Time_stamp::operator>=(const Time_stamp &stamp) const
{
	return _microsecond>=stamp._microsecond;
}

bool Time_stamp::operator<(const Time_stamp &stamp) const
{
	return _microsecond<stamp._microsecond;
}

bool Time_stamp::operator<=(const Time_stamp &stamp) const
{
	return _microsecond<=stamp._microsecond;
}

Time_stamp Time_stamp::now()
{
	struct timeval tv;
	int ret = gettimeofday(&tv, NULL);
	assert(ret != -1);
	return Time_stamp(tv.tv_sec, tv.tv_usec);
}
}
