#include "../../include/Location.hpp"

/*============================================================================*/
/* SECTION:               Constructors and destructor                         */
/*============================================================================*/

Location::Location( void ) : ConfigBase(),
	_route(""),
	_alias(ALIAS_DEFAULT)
{}

Location::Location( std::string const& route ) : ConfigBase(),
	_route(route),
	_alias(ALIAS_DEFAULT)
{}

Location::~Location( void )
{
	/* TODO */
}

/*==========*/
/* !SECTION */
/*============================================================================*/
/* SECTION:                   Operator overloading                            */
/*============================================================================*/

/***********************/
/* NOTE: '<<' operator */
/***********************/

std::string		Location::print( void ) const
{
	std::string	buffer = "";

	buffer += "\t[ LOCATION ] " + _route + "\n";
	buffer += "\t\t· Alias: \"" + _alias + "\"\n";
	buffer += static_cast<ConfigBase const&>(*this).print("\t");
	return buffer;
}

std::ostream&	operator<<( std::ostream& os, Location const& printObject )
{
	os << printObject.print();
	return (os);
};

/*==========*/
/* !SECTION */
/*============================================================================*/
/* SECTION:                    Getters and setters                            */
/*============================================================================*/

std::string	Location::get_route( void ) const { return _route; }
void		Location::set_route( std::string route ) { _route = route; }

std::string	Location::get_alias( void ) const { return _alias; }
void		Location::set_alias( std::string alias ) { _alias = alias; }

/*==========*/
/* !SECTION */
/*============================================================================*/
/* SECTION:                      Object features                              */
/*============================================================================*/

void Location::inherit( ConfigBase const& src)
{
	/* Get the location full path */
	std::string path = (_inicializated[ROOT_INDEX] ? _root : src.get_root());
	path += (_alias != "" ? _alias : _route);

	/* Repush te error pages */
	for (std::map<int, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); it++)
	{
		if (path[path.size() - 1] == '/')
			it->second = path + it->second.substr(1, it->second.size());
		else
			it->second = path + it->second;
	}

	/* Repush te error indexes */
	for (std::vector<std::string>::iterator it = _indexes.begin(); it != _indexes.end(); it++)
	{
		if (path[path.size() - 1] == '/')
			*it = path + *it;
		else
			*it = path + std::string("/") + *it;
	}

	static_cast<ConfigBase*>(this)->inherit( src );
}
/*==========*/
/* !SECTION */
/*==========*/
/*============================================================================*/
/* SECTION:                        Exceptions                                 */
/*============================================================================*/

/*==========*/
/* !SECTION */
/*==========*/
