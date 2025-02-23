#include "Log.hpp"

Log::Log()
: _isInit(false), _useFile(false)
{
}
Log::~Log()
{
	_off();
}

void	Log::_on()
{


	std::string filename = "log " + _getTimestamps();
	
	_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

	// open
	
	// write fd to _fd

}

void	Log::_off()
{
	// if (_fd != -1)
	// {
	// 	close(_fd);
	// 	_fd = -1;
	// }
}

void	Log::init()
{
	if (_isInit)
		return ;
	// _config = config;
	_on();
	_isInit = true;
}

std::string	Log::_getTimestamps()
{
    char	buffer[64];
    time_t	now = time(NULL);
    struct	tm *timeinfo = localtime(&now);

    if (timeinfo)
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    else
        return ("Unknown Time");

    return (std::string(buffer));
}

std::string Log::_getLogFileName(int serverFd)
{
    char	buffer[128];
    time_t	now = time(NULL);
    struct	tm *timeinfo = localtime(&now);
    
    if (timeinfo)
        strftime(buffer, sizeof(buffer), "server_%d_%Y-%m-%d_%H:%M:%S.log", timeinfo);
    else
        return ("server_unknown.log");

    return (std::string(buffer));
}