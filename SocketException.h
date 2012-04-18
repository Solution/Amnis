/*
 * File:   SocketException.h
 * Author: solution
 *
 * Created on 18. duben 2012, 14:34
 */

#ifndef SOCKETEXCEPTION_H
#define	SOCKETEXCEPTION_H

#include <iostream>

class SocketException
{
	public:
		SocketException(const char*);
		SocketException(const char*, int);
		//SocketException(const char *message, int code);
		const char* getMessage() const;
		const int getCode() const;

	private:
		const char *message;
		int code;
};



#endif	/* SOCKETEXCEPTION_H */

