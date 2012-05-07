/*
 * File:   UndefinedOffsetException.h
 * Author: solution
 *
 * Created on 7. květen 2012, 11:42
 */

#ifndef UNDEFINEDOFFSETEXCEPTION_H
#define	UNDEFINEDOFFSETEXCEPTION_H

class UndefinedOffsetException
{

public:

	UndefinedOffsetException(const char*);
	UndefinedOffsetException(const char*, int);
	const char* getMessage() const;
	const int getCode() const;
	virtual ~UndefinedOffsetException();

private:
	const char* message;
	int code;
};

#endif	/* UNDEFINEDOFFSETEXCEPTION_H */

