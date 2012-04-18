/*
 * File:   Socket.cpp
 * Author: solution
 *
 * Created on 18. duben 2012, 10:35
 */

#include "Socket.h"

void *acceptSocket(void* ptr)
{
	Socket *serverSocket = (Socket*)ptr;
	for( ; ; )
	{
		serverSocket->setPendingConnection(accept(serverSocket->getSocketDescriptor(),
									(sockaddr*) NULL,
									NULL));
	}
}


Socket::Socket(int type)
	:pendingConnection(-1)
{
	if(type == TCP)
	{
		if((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw SocketException("Can't create the TCP protocol based socket");
	}else if(type == UDP)
	{
		if((socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			throw SocketException("Can't create the UDP protocol based socket");
	}else{
		throw SocketException("Protocol is not supported yet.",
				0 // 0 no more actions
				);
	}
}

void Socket::bindOn(const std::string host, uint tport)
{
	address = host;
	port = tport;
	// Creating a struct sockaddr_in, structure with socket info
	socketStruct.sin_family = AF_INET;
	socketStruct.sin_port = htons(port);

	// Host translation
	struct hostent *socketHostent = gethostbyname(host.c_str());

	socketStruct.sin_addr.s_addr = inet_addr(socketHostent->h_addr_list[0]);

	int bindError;
	if((bindError = bind(socketDescriptor, (struct sockaddr*) &socketStruct, sizeof(socketStruct))) == -1)
		throw SocketException("Cant bind the socket", bindError);
}

void Socket::bindOn(int host, uint tport)
{
	port = tport;
	// Creating a struct sockaddr_in, structure with socket info
	socketStruct.sin_family = AF_INET;
	socketStruct.sin_port = htons(port);

	// Host translation
	if(host == LOCALHOST)
	{
		socketStruct.sin_addr.s_addr = INADDR_LOOPBACK;
	}else{
		socketStruct.sin_addr.s_addr = INADDR_ANY;
	}

	int bindError;
	if((bindError = bind(socketDescriptor, (struct sockaddr*) &socketStruct, sizeof(socketStruct))) == -1)
		throw SocketException("Cant bind the socket", bindError);
}

void Socket::start(uint connections = 1)
{
	// start listenning
	if(listen(socketDescriptor, connections) == -1)
		throw SocketException("Cant start listening on the socket", 0);

	// create thread for pending connections
	acceptingThread = pthread_create(&acceptingThread,
									 NULL,
									 acceptSocket,
									 static_cast<void*>(this));

	// the thread will be terminated by another thread
	pthread_join(acceptingThread, NULL);

}

void Socket::setPendingConnection(int socket)
{
	pendingConnection = socket;
}

int Socket::getNextPendingConnection()
{
	return pendingConnection;
}

int Socket::getSocketDescriptor() const
{
	return socketDescriptor;
}

std::string Socket::getSocketAddress() const
{
	return address;
}

uint Socket::getSocketPort() const
{
	return port;
}

bool Socket::isDecimalFormat(std::string ip)
{

}

Socket::Socket(const Socket& orig)
{
}

Socket::~Socket()
{
	close(socketDescriptor);
//	delete //acceptingThread,
//			socketStruct;
}

