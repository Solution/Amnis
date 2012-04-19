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
#include <vector>

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

vector<string> splitString(string& data, char delimiter)
{
	uint startPos = 0, pos;
	vector<string> splittedStrings;

	while((pos = data.find(delimiter, startPos)) != string::npos)
	{
		splittedStrings.push_back(data.substr(startPos, pos));
		startPos = pos;
	}
	return splittedStrings;
}

string getFileName(string& data)
{
	vector<string> fields = splitString(data, 0x20);
	return fields[2];
}

// Http server - start

void readyRead(void *ptr)
{
	try
	{
		cout << "Client is sending me something" << endl;
		// cast void ptr to our socket
		Socket *client = static_cast<Socket*>(ptr);

		// get data from socket
		string data = client->read(1024);
		// extract line number 0 from request
		string line = extractLine(data, 0);
		string requestedFile = getFileName(line);

		cout << "Client wants from me(hole request): "
				<< line << endl;

		cout << "Only file name: "
				<< requestedFile << endl;

		cout << "I'll say no to him with h1 paragraph" << endl;
		// create a http response
		string sendingData = httpHeader().append("\n<h1>No!</h1>");
		// and send it to the client
		client->write(sendingData);

		cout << "And close the connection" << endl;
		// this will close the socket, which is neccesary
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
		// Cast void pointer to Socket
		Socket *http = static_cast<Socket*>(ptr);

		// Create a new Socket client
		Socket *client = new Socket();
		// Set socket descriptor of last connection to our client
		client->setSocketDescriptor(http->getNextPendingConnection());
		// and bind readyread event to our function
		// readyread is event, which is omitted, when is something to read
		// on the socket descriptor
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
		// Create a TCP protocol based Socket
		Socket *http = new Socket(TCP);
		// Bind to all available address and port 4444, you know *:4444
		http->bindOn(ALL_AVAILABLE, 4444);

		cout << "Server start and wait for connection" << endl;
		// Bind the new connection event, to our function
		http->bindEvent(NewConnection, &newHttpConnection);
		// And start listening, 1 = maximum connections
		http->start(1);
		string input;
		// some il
		while(input.compare("stop") != 0)
		{
			cin >> input;
		}
		http->close();
		delete http;
	}catch(SocketException e)
	{
		cout << e.getMessage() << endl;
	}
}

// http server end

// UDP messager(client, client)


int main(int argc, char** argv)
{
	// run http server example
	createSimpleHttpServer();

	sleep(4);
	return 0;
}

