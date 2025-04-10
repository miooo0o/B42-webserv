/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:07:26 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/10 08:56:40 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"

enum status	split( std::string& str, std::string delim, std::string& dest, status status){
	
	if (str.empty()) { return EMPTY;}

	int pos = str.find( delim );
	
	if ( pos == std::string::npos ) {
		switch (status) {
			case LINE_SPLIT : { return NO_EOL; }
			case KEY_SPLIT 	: { return NO_KEY; }
			case VAL_SPLIT 	: { return NO_DELIM; }
			default			: { return NO_STATUS; }
		}
	}
	else if ( pos == 0 ) {
		switch (status) {
			case LINE_SPLIT : { str.erase(0, pos + delim.length()); return EOL; }
			case KEY_SPLIT 	: { return NO_KEY; }
			case VAL_SPLIT 	: { return NO_VAL; }
			case EOL		: { return EOS; }
			default			: { return NO_STATUS; }
		}
	}
	else {
		dest = str.substr(0, pos);
		str.erase(0, pos + delim.length());
		switch (status) {
			case LINE_SPLIT : { return EOL; }
			case KEY_SPLIT 	: { return VAL_SPLIT; }
			case VAL_SPLIT 	: { return VAL_SPLIT; }
			default			: { return NO_STATUS; }
		}
	}
	return NO_STATUS;
}

template <class T>
void	parseToMap(T& map, std::string& str, enum mapType type){

	std::string line;
	std::string key;
	std::string val;
	std::string delim;
	enum status status = LINE_SPLIT;

	while (status != EOS) {
		switch (status) {
			case LINE_SPLIT : { delim = LINE_DELIM;	status = split (str, delim, line, status); continue; }
			case KEY_SPLIT 	: { delim = KEY_DELIM; status = split (line, delim, key, status); continue; }
			case VAL_SPLIT 	: { break; }
			case EOL		: {	status = split( line, delim, key, status ); if (status == EOL) return; continue; }
			case EMPTY		: { return; } //error(EMPTY); }
			case NO_KEY		: { return; } //error(NO_MATCH);}
			case NO_VAL		: { return; } //error(NO_MATCH);}
			default			: { return; } //error(UNKNOWN);}
		}
		switch ( type ) {
			case MAPSTRSTR 	: {	map[key] += line;  continue; }
			case MAPSTRVEC 	: {	delim = VAL_DELIM; status = split (line, delim, val, status); map[key].push_back(val); continue; }
			default 		: { return ; }
		}
	}
	return ;
}

/* 
void  parseToVector(std::vector<std::string>&, std::string& str, enum mapType){

	while (!str.empty()){
		std::string line = getLine (str, delim_str);
		if (line.compare(str) == 0)
			return;
		std::string key = getLine(line, delim_char);
		map[key] = line;
	}
	return;
}
*/

int main(){
	std::map<std::string, std::string> mapStrStr;
	std::map<std::string, std::vector<std::string> > mapStrVec;

	std::string str = "GET /index.html HTTP/1.1\r\nHost: example.com\r\nUser-Agent: curl/7.68.0\r\n\r\n";

	parseToMap(mapStrStr, str, MAPSTRSTR);

	for (std::map<std::string, std::string>::iterator it = mapStrStr.begin(); it != mapStrStr.end(); it++){
		std::cout << it->first << " : " << it->second << std::endl;
	}
}
