/*
 * File:   main.cpp
 * Author: solution
 *
 * Created on 18. duben 2012, 1:13
 */

#include <iostream>
#include <fstream>
#include <errno.h>
#include <stdlib.h>

using namespace std;

#define BUFFER_SIZE 8196

void videoStream()
{
		cout << "Amnis started" << endl;
	char *buffer = new char[BUFFER_SIZE];

	ifstream videoStream("/srv/tmp.avi", ios::binary | ios::out | ios::ate);
	cout << "Opening the file: /srv/tmp.avi" << endl;

	int size = videoStream.tellg();
	cout << "Size of the file: " << size << endl;
	videoStream.seekg(0, ios::beg);

	while(!videoStream.eof())
	{
		videoStream.read(buffer, BUFFER_SIZE);
	}

	cout << "End of file" << endl;

	videoStream.close();
	delete buffer;
}

#include "Socket.h"


string extractLine(string& data, uint numLine)
{
	uint lines = 0, startPos = 0, pos;
	while((pos = data.find('\n', startPos)) != string::npos)
	{
		if(lines == numLine) return data.substr(startPos, pos);
		lines++, startPos = pos;
	}
	return "nothing";
}

string httpHeader()
{
	string header = "HTTP/1.1 200 OK\n \
				  Content-type: text/html; charset=UTF-8\n \
				  Content-encoding: gzip\n \
				  Server: Example server\n";
	return header;
}

// Http server - start

void readyRead(void *ptr)
{
	try
	{
		cout << "Client is sending me something" << endl;
		Socket *client = static_cast<Socket*>(ptr);

		string data = client->read(1024);
		string line = extractLine(data, 0);

		cout << "Client wants from me: "
				<< line << endl;

		cout << "I'll say no to him with h1 paragraph" << endl;

		string sendingData = httpHeader().append("\n<h1>No!</h1>");
		client->write(sendingData);

		cout << "And close the connection" << endl;
		client->close();
	}catch(SocketException e)
	{
		cout << e.getMessage() << endl;
	}
}


void newHttpConnection(void *ptr)
{
	try
	{
		cout << "New connection" << endl;
		Socket *http = static_cast<Socket*>(ptr);

		Socket *client = new Socket();
		client->setSocketDescriptor(http->getNextPendingConnection());
		client->bindEvent(ReadyRead, &readyRead);
	}catch(SocketException e)
	{
		cout << e.getMessage() << endl;
	}
}

void createSimpleHttpServer()
{
	try
	{
		Socket *http = new Socket(TCP);
		http->bindOn(ALL_AVAILABLE, 4444);

		cout << "Server start and wait for connection" << endl;
		http->bindEvent(NewConnection, &newHttpConnection);
		http->start(1);

		while(true) {}
	}catch(SocketException e)
	{
		cout << e.getMessage() << endl;
	}
}

// http server end

int main(int argc, char** argv)
{
	// run http server example
	createSimpleHttpServer();

	sleep(4);
	return 0;
}

