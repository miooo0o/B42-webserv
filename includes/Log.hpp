#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <map>


/**
 * @brief singleton patten class for logging webserver.
 */
class Log
{
private:
	enum e_level
	{
		_NONE	= 0,
	    DEBUG,
	    INFO,
	    WARNING,
	    ERROR,
	    CRITICAL
	};

    bool					_isInit;
	std::queue<std::string>	_logQueue;

 	/**
     * @brief Maps server file descriptors (server_fd)
	 * 		  to log file descriptors (log_fd).
     */
	std::map<int, int>		_fds;
    e_level					_level;
    bool					_useFile;
//	/* server conf class */		*_config;


private:
    Log();
	~Log();
	std::string _getTimestamps();
	std::string _getLogFileName(int serverFd);
	int			_getNewLogFile(int serverFd);
	void		_closeAllFds();
	void		_closeNotAtiveFds(std::vector<int>& activeFds);
	void		_findNewConnections(std::vector<int>& activeFds);


public:
	static Log	&getInstance();
	void		init( /* server conf class */ );
	void		logMessage(int serverFd, e_level level, const std::string& message);
	void		flushLogs();
	void		autoUpdateFDs();
};
