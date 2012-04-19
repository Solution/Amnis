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
#include "Base64.h"
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
	string header = "HTTP/1.1 200 OK\n "
				  // "Content-encoding: gzip\n"
				  "Server: Amnis HTTP\n";
	return header;
}

vector<string> splitString(string& data, char delimiter)
{
	uint startPos = 0, pos;
	vector<string> splittedStrings;

	while((pos = data.find(delimiter, startPos)) != string::npos)
	{
		splittedStrings.push_back(data.substr(startPos, (pos - startPos)));
		startPos = pos+1;
	}
	splittedStrings.push_back(data.substr(startPos, string::npos));
	return splittedStrings;
}

string getFileName(string& data)
{
	vector<string> fields = splitString(data, 0x20);
	return fields[1];
}

#define PATH_TO_DOCUMENTS "/var/www"
#define ROOT_ITEM "root.pad"

bool isItResource(string &fileName)
{
	vector<string> nameParts = splitString(fileName, '.');
	if(nameParts[1].compare("jpg") == 0 || nameParts[1].compare("JPG") == 0
	|| nameParts[1].compare("png") == 0 || nameParts[1].compare("PNG") == 0
	|| nameParts[1].compare("gif") == 0 || nameParts[1].compare("GIF") == 0)
	{
		return true;
	}
	else{
		return false;
	}
}

string getFileContent(string &fileName)
{
	string path = PATH_TO_DOCUMENTS, returnedData;
	if(fileName.compare("/") == 0) fileName.append(ROOT_ITEM);
	path.append(fileName);
	cout << path << endl;
	ifstream fileStream(path.c_str(), ios::binary);

	if(fileStream.is_open())
	{
		fileStream.seekg(0, ios::end);
		int fileLength = fileStream.tellg();
		cout << fileLength << endl;
		fileStream.seekg(0, ios::beg);
		char *buffer = new char[fileLength];

		fileStream.read(buffer, fileLength);
		if(isItResource(fileName))
		{
			cout << "It is resource" << endl;
			returnedData.append(base64_encode(reinterpret_cast<const unsigned char*>(buffer), fileLength));
		}else{
			cout << "Its not resource" << endl;
			returnedData.append(buffer);
		}

		cout << "It works" << endl;

		fileStream.close();
		delete buffer;
	}else{
		returnedData.append("\n<h1>File not found</h1>");
	}
	return returnedData;
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

		//cout << "Client wants from me(hole request): "
		//		<< line << endl;

		cout << "Only file name: "
				<< requestedFile << endl;

		//cout << "I'll say no to him with h1 paragraph" << endl;
		// create a http response
		string sendingData = httpHeader();
		vector<string> nameParts = splitString(requestedFile, '.');
		cout << nameParts[1] << endl;
		if(nameParts[1].compare("jpg") == 0 || nameParts[1].compare("JPG") == 0)
		{
			sendingData.append("Content-type: image/jpeg;\n\n");
			sendingData.append(base64_decode(getFileContent(requestedFile)));
		}else if(nameParts[1].compare("png") == 0 || nameParts[1].compare("PNG") == 0)
		{
			sendingData.append("Content-type: image/png;\n\n");
			sendingData.append(base64_decode(getFileContent(requestedFile)));
		}
		else if(nameParts[1].compare("gif") == 0 || nameParts[1].compare("GIF") == 0)
		{
			sendingData.append("Content-type: image/gif;\n\n");
			sendingData.append(base64_decode(getFileContent(requestedFile)));
		}
		else{
			sendingData.append("Content-type: text/html; charset=UTF-8\n\n");
			sendingData.append(getFileContent(requestedFile));
		}

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

