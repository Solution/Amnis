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
/*
 *
 */
int main(int argc, char** argv)
{
	try
	{
		Socket *server = new Socket(TCP);
		//server->bindOn("localhost", 4445);
		server->bindOn(ALL_AVAILABLE, 4445);
		server->start(1);

		cout << "Server is listenning on: "
			<< server->getSocketAddress()
			<< ":"
			<< server->getSocketPort() << endl;

		while(server->getNextPendingConnection() < 0)
		{

		}

		Socket *client = new Socket;
		client->setSocketDescriptor(server->getNextPendingConnection());
		client->write("Ahoj");

		delete server,
			client;
	}catch(SocketException  e)
	{
		cout << e.getMessage() << endl;
		cout << "error code: " << e.getCode() << endl;
	}


	sleep(10);
	return 0;
}

