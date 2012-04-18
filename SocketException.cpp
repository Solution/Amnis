#include "SocketException.h"

SocketException::SocketException(const char *data)
	:message(data),
		code(-1)
{
}

SocketException::SocketException(const char *data, int code = 1)
		:message(data),
		code(code)
{
}

const char* SocketException::getMessage() const
{
	return message;
}

const int SocketException::getCode() const
{
	return code;
}