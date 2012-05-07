/*
 * File:   UndefinedOffsetException.cpp
 * Author: solution
 *
 * Created on 7. květen 2012, 11:42
 */

#include "UndefinedOffsetException.h"

UndefinedOffsetException::UndefinedOffsetException(const char* msg)
{
	message = msg;
}

UndefinedOffsetException::UndefinedOffsetException(const char* msg, int icode)
{
	message = msg;
	code = icode;
}

const char* UndefinedOffsetException::getMessage() const
{
	return message;
}

const int UndefinedOffsetException::getCode() const
{
	return code;
}

UndefinedOffsetException::~UndefinedOffsetException()
{
	delete message;
}

