/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/19 14:26:18 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Error.hpp"

Error::Error(): _lastError(0), _response_code(200), _last_response_code(200) {flagStringsInit();}

Error::Error( uint64_t error) : _lastError(0), _errorFlags(error), _response_code(200), _last_response_code(200){flagStringsInit();}

Error::~Error( void ) {}

Error::Error( const Error& other ):_errorFlags(other._errorFlags) {}

Error& Error::operator=( const Error& other ) {	
	if ( this != &other )
		*this = other;
	return ( *this );
}

void	Error::flagStringsInit()
{
	_flagStrings.push_back("None");
	_flagStrings.push_back(HEAD_END);
	_flagStrings.push_back(HEAD_SEP	);
	_flagStrings.push_back(METH_NOT_PERM);
	_flagStrings.push_back(METH_UNREC);
	_flagStrings.push_back(METH_UNSUP);
	_flagStrings.push_back(METH_CASE);
	_flagStrings.push_back(REQ_END);
	_flagStrings.push_back(URI_ENCOD);
	_flagStrings.push_back(URI_FRAG_REC);
	_flagStrings.push_back(URI_ILLEGAL_CHARS);
	_flagStrings.push_back(URI_PATH_INV);
	_flagStrings.push_back(URI_SCH_UNSUP);
	_flagStrings.push_back(URI_SCH_UNREC);
	_flagStrings.push_back(VER_NONE);
	_flagStrings.push_back(VER_UNREC);
	_flagStrings.push_back(VER_UNSUP);	
}
		
/* GETTERS */

/* Check if flag set for a given error */
bool	Error::isFlagSet(uint64_t flag) {
	return (_errorFlags & flag) != 0;
}

void	Error::addErrorFlag(uint64_t error) {
	_lastError = error;
	_errorFlags |= error;
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
	os << l14 << "Error Num " << r3 << " : " << errNum << std::endl;
	os << l14 << "Error Str1 " << r3 << " : " << getErrorStr1(str_to_int(errNum)) << std::endl;
	os << l14 << "Error Str2 " << r3 << " : " << _flagStrings.at(pos).substr(4) << std::endl << std::endl;

	return os;
}

std::string		Error::getErrorNum(int pos){
	std::string abc = _flagStrings[pos].substr(0,4);
	return abc;
}

int getPosition(uint64_t num){
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
	
	os << "\nError Class\n==========" << std::endl;
	while (it > 0 && it <= _errorFlags) {
		if (isFlagSet(it))
			getErrorInfo(pos - 1, os);
		it <<= 1;
		pos++;
	}

	os << "\nLast Error : \n" << std::endl;
	getErrorInfo(getPosition(_lastError), os) << std::endl;
	return os;
}

std::ostream&	operator<<(std::ostream& os, Error& error) {
	error.getErrorStream(os);
	return os;
}
