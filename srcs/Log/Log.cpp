#include "Log.hpp"

Log::Log()
: _isInit(false), _useFile(false)
{
}
Log::~Log()
{
	_closeAllFds();
}

int	Log::_getNewLogFile(int serverFd)
{
	std::string filename = _getLogFileName(serverFd);
    int logFd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (logFd == -1)
        std::cerr << "Error opening log file for FD: " << serverFd << std::endl;
    
    return (logFd);
}

void	Log::_closeAllFds()
{
	for (std::map<int, int>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->second != -1)
			close(it->second);
	}
	_fds.clear();
}

void	Log::init( /* server conf class */ )
{
	if (_isInit)
		return ;
	// _config = config;  /* server conf class */
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
	
	if (!timeinfo)
		return ("server_unknown.log");
	strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H:%M:%S.log", timeinfo);
	
	// FIXME: better generate logic
	return ("server_" + serverFd +  std::string(buffer));
}

void	Log::autoUpdateFDs( /* server conf class, server class */ )
{
	// TODO: need to get this from server side
	
	init();
    std::vector<int> activeFds = getServerFDs();
	_findNewConnections(activeFds);
	_closeNotAtiveFds(activeFds);
}

void	Log::_findNewConnections(std::vector<int>& activeFds)
{
	for (size_t i = 0; i < activeFds.size(); i++)
    {
        if (_fds.find(activeFds[i]) == _fds.end())
		{
			int logFd = _getNewLogFile(activeFds[i]);
            if (logFd != -1)
			{
				_fds[activeFds[i]] = logFd;
				writeLog(activeFds[i], INFO, "New connection started.");
			}
			else
			{
				// TODO: update error message
				writeLog(activeFds[i], ERROR, "error message");
			}
		}
    }
}

void	Log::_closeNotAtiveFds(std::vector<int>& activeFds)
{
	std::map<int, int>::iterator it = _fds.begin();
	while (it != _fds.end())
	{
		if (std::find(activeFds.begin(), activeFds.end(), it->first) == activeFds.end())
		{
			writeLog(it->first, INFO, "Connection closed.");
			if (close(it->second) == -1)
			{
				// TODO: update error message
				writeLog(it->first, ERROR,  "error message");
			}

			it = _fds.erase(it);
		}
		else
			++it;
	}
}
