#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <fcntl.h>
#include <unistd.h>

class Log
{
private:
	enum e_level
	{
		_		= 0,
	    DEBUG,
	    INFO,
	    WARNING,
	    ERROR,
	    CRITICAL
	};

    bool					_isInit;
	std::queue<std::string>	_logQueue;

	/**
	 * @brief key:value, server fd : log file fd
	 */
	std::map<int, int>		_fd;
    e_level					_level;
    bool					_useFile;
//	/* server conf */		*_config;


private:
    Log();
	~Log();
	std::string _getTimestamps();
	std::string _getLogFileName(int serverFd);
	void		_on();
	void		_off();

public:
	static Log	&getInstance();
	void		init( /* server conf */ );

};
