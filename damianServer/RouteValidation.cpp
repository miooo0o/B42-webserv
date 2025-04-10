/* Route Validation */

/* 
	This is partial validation, to be performed on each Route.

	The overall purpose is to have upfront check to :
		: build the path for the endpoint (path)
		: check that the path is accessible
		: save path permissions
		: check if path is a file, or a directory

	If we modify or create files, maybe we have to refresh this for each Route
	
	Otherwise, it is done at outset, and doesn't need to be checked again for every
	request.

	If a Route is used with a DELETE or POST request, then we should refresh the Route
	Should we maintain a map of Routes for all servers to make this easier to manage?

*/ 

/*
	This struct has been added to the Route class
	An instance should be used as a private variable in the Route class
*/

struct targetInfo_s
{
	bool inaccessible;		// set to true if path can"t be accessed
	bool is_dir;			// set to true if path is a dir
	bool is_file;			// set to true if path is a file
	bool not_found;			// set to true if file not found
	__mode_t permissions;	// the file permissions
	std::string path;		// the path (root + '/' endpoint )

	targetInfo_s() : inaccessible(false), is_dir(false), is_file(false), not_found(false), permissions(0), path("") {}
};

/* 
	void Route::appendPathToRoot();

	Function to append endpoint to route->root 
	Saved to _target.path

	The _target_path is to be used when accessing endpoints during requests
*/

void Route::appendPathToRoot()
{
	str_t r = root_dir;
	str_t p = path;

	if (r[r.size() - 1] == '/')
		trimEndChar(r, '/');
	if (p[0] == '/')
		trimLeadingChar(p, '/');
	_target.path = r + '/' + p;
}

/* 
	void Route::getTargetPathPermissions();

	Function to determine :
		file type ( file or dir)
		accessibility/permissions
	
	targetInfo_s struct is updated as appropriate
	
*/

void Route::getTargetPathPermissions(){
	struct stat fileStat;

	if (stat(_target.path.c_str(), &fileStat) == 0)
	{
		_target.is_dir = S_ISDIR(fileStat.st_mode);
		_target.is_file = S_ISREG(fileStat.st_mode);
		_target.permissions = fileStat.st_mode;
	}
	else if(errno == ENOENT)
		_target.not_found = true;
	else if(errno == EACCES)
		_target.inaccessible = true;
}

/* 
	void Route::setRouteTarget()
	
	Calls functions to build the Route path and check type nad accessibility
 */
void Route::setRouteTarget()
{
	appendPathToRoot();
	getTargetPathPermissions();
	/* 
		Need to decide if we throw error if path inaccessible or not found
		 or just log it?
	*/
	return;
}

/* Helper Funcitons */
void trimLeadingChar(std::string &str, char target)
{
	while (str[0] == target)
		str.erase(0, 1);
}

void trimEndChar(std::string &str, char target)
{
	while (str[str.size() - 1] == target)
		str.erase(str.size() - 1);
}
