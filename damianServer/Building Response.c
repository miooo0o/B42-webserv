Building Response
{

}

/* NEED TO DECIDE HOW TO HANDLE PARTIAL PATHS
	1.
		Route : /home/records 
		Request: /records
	
		Do we search within routes for a partial match, and then return a resource?
		Or are we strict, and only allow exact matches?
	2. 
		Route : /home/records/ 
		Request: /home/records/abc/
		Would we return a default file here if abc exists?
		Or are we strict, and only allow exact matches?			
	3.
		Route : /test
		Request: /test/otherFolder/text.html
		Would we return the resource if the path exists, and an error if not? 
		Or are we strict, and only allow exact matches?
		Or do we redirect to /test?
		Or do we alway redirect to /home if the path is invalid?
		
	Options :
		Exact Path Matching	: Only return exact matches
		Dynamic Matching	: Allow partial matches, or treat route path as a base, and allow searches
		Static file routing	: If base matches, and then "parameters" match exactly, return resource
		Default handler		: No match, return default page
		URL rewrite			: If partial match, URL rewrite (example 1 : return test/default_file)

	*/