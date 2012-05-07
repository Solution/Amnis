/*
 * File:   HttpResponse.cpp
 * Author: solution
 *
 * Created on 7. květen 2012, 9:31
 */

#include "HttpResponse.h"
#include "Exceptions/UndefinedOffsetException.h"

HttpResponse::HttpResponse() {}


void HttpResponse::addField(uint fieldType, string value)
{
	if(count(fieldStringTypes) < fieldType)
	{
		throw UndefinedOffsetException("The field type is out of field types array range");
	}else
	{
		fields.insert(pair<string, string>(fieldStringTypes[fieldType], value));
}
}

void HttpResponse::addField(string fieldType, string value)
{
	fields.insert(pair<string, string>(fieldType, value));
}

string HttpResponse::headerToString()
{
	string header;
	map<string, string>::iterator it;

	for(it = fields.begin(); it != fields.end(); it++)
	{
		header.append((*it).first)
			  .append(" ")
			  .append((*it).second)
			  .append("\n");
	}

	header.append("\n\n");
	return header;
}

HttpResponse::~HttpResponse() {}

