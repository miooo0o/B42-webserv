/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/08 00:31:56 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Error.hpp"

Error::Error(): _lastError(0), _response_code(200), _last_response_code(200), _error_log(NULL) {flagStringsInit();}

Error::Error( uint64_t error) : _lastError(0), _errorFlags(error), _response_code(200), _last_response_code(200), _error_log(NULL) {flagStringsInit();}

Error::~Error( void ) {}

Error::Error( const Error& other ):_errorFlags(other._errorFlags) {
}

Error& Error::operator=( const Error& other ) {
	if ( this != &other ) {
		*this = other;
	}
	return ( *this );
}

void	Error::flagStringsInit()
{
	_flagStrings.push_back(NO_ERROR);
	_flagStrings.push_back(HEAD_CONT_UNS);
	_flagStrings.push_back(HEAD_END);
	_flagStrings.push_back(HEAD_OMI_CONT);
	_flagStrings.push_back(HEAD_HOST);
	_flagStrings.push_back(HEAD_SEP);
	_flagStrings.push_back(METH_NOT_PERM);
	_flagStrings.push_back(METH_UNREC);
	_flagStrings.push_back(METH_UNSUP);
	_flagStrings.push_back(METH_CASE);
	_flagStrings.push_back(REQ_END);
	_flagStrings.push_back(URI_ENCOD);
	_flagStrings.push_back(URI_FRAG_REC);
	_flagStrings.push_back(URI_HOST_RES);
	_flagStrings.push_back(URI_ILLEGAL_CHARS);
	_flagStrings.push_back(URI_LENGTH);
	_flagStrings.push_back(URI_LOOP);
	_flagStrings.push_back(URI_PATH_INV);
	_flagStrings.push_back(URI_PATH_LEN);
	_flagStrings.push_back(URI_PATH_TRAV);
	_flagStrings.push_back(URI_PATH_UNSAFE);
	_flagStrings.push_back(URI_PORT_INV);
	_flagStrings.push_back(URI_QUERY_CONF);
	_flagStrings.push_back(URI_QUERY_INV);
	_flagStrings.push_back(URI_SCH_UNSUP);
	_flagStrings.push_back(URI_SCH_UNREC);
	_flagStrings.push_back(VER_NONE);
	_flagStrings.push_back(VER_UNREC);
	_flagStrings.push_back(VER_UNSUP);

	_flagStrings.push_back(HEAD_HOST_MIS);
	_flagStrings.push_back(HEADERS_MULTI_LEN);
	_flagStrings.push_back(HEADERS_EMPTY_VAL);
	_flagStrings.push_back(HEADER_TR_ENCOD_NOT);
}
		
/* GETTERS */

/* Check if flag set for a given error */
bool	Error::isFlagSet(uint64_t flag) {
	return (_errorFlags & flag) != 0;
}

/* PASSES ERROR INFO TO LOGS WHERE IT IS SAVED TO FILE*/
void	Error::logError(){
	
	std::ostream& os = std::cout;
	getErrorInfo(getPosition(_lastError), os);
	if (!_error_log)
		_error_log = new Logs();
	log_t tmp (&os, STREAM, LOGS_ERR );
	_error_log->logHandler(tmp, APPEND);
}

bool	Error::addErrorFlag(uint64_t error) {
	_lastError = error;
	_errorFlags |= error;

	logError();
	return false;
}

std::string	Error::getErrorStr1(int num) {
	switch (num){
		case 400 : return CODE400;
		case 403 : return CODE403;
		case 404 : return CODE404;
		case 405 : return CODE405;
		case 415 : return CODE415;
		case 501 : return CODE501;
		default  : return "None"; 
	}
}

std::ostream& Error::getErrorInfo(int pos, std::ostream& os){
	std::string errNum = getErrorNum(pos);
	os << std::endl;
	os << l14 << "Error Num " << r3 << " : " << errNum << std::endl;
	os << l14 << "Error Str1 " << r3 << " : " << getErrorStr1(str_to_int(errNum)) << std::endl;
	os << l14 << "Error Str2 " << r3 << " : " << _flagStrings.at(pos).substr(4) << std::endl;

	return os;
}

std::string		Error::getErrorNum(int pos){
	std::string errorNum = _flagStrings[pos].substr(0,4);
	return errorNum;
}

int Error::getPosition(uint64_t num){
	int	pos = 0;
	
	if (num == 0)
		return 0;
	while (num != 1){
		num >>= 1;
		pos++;
	}
	return pos;
}

std::ostream&	Error::getErrorStream(std::ostream& os){

	uint64_t it = 1;
	int	pos = 1;
	
	os << "\nError Class\n===========";
	while (it > 0 && it <= _errorFlags) {
		if (isFlagSet(it))
			getErrorInfo(pos - 1, os);
		it <<= 1;
		pos++;
	}

	os << "\nLast Error\n==========";
	getErrorInfo(getPosition(_lastError), os);
	return os;
}

int	Error::getLastResponseCode(){
	if (_lastError == 0)
		return 200;

	int pos = getPosition(_lastError);
	std::string errNum = getErrorNum(pos);
	return str_to_int(errNum);
}

std::ostream&	operator<<(std::ostream& os, Error& error) {
	error.getErrorStream(os);
	return os;
}
