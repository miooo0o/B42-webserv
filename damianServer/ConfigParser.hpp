#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include "Config.hpp"
#include "Route.hpp"

class ConfigParser
{
	public:
		const std::string _filename;
	
		/* CONSTRUCTORS */
		ConfigParser( const std::string& filename );
	
		/* DESTRUCTORS */
		~ConfigParser( void );
	
		void	parseLocation(Config& config, std::ifstream& file, std::string& oldLine);
		Config* parseServerblocks();
	
	private:

		template<typename T>
		T parseValue(const std::string &line);

		std::string trim(const std::string &str);
		std::vector<std::string> parseMethods(const std::string &line);
		std::string parseLocationPath(const std::string &line);
};