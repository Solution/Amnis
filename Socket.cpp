/*
 * File:   Socket.cpp
 * Author: solution
 *
 * Created on 18. duben 2012, 10:35
 */

#include "Socket.h"

Socket::Socket()
	:pendingConnection(-1)
{

}

Socket::Socket(int type)
	:pendingConnection(-1),
	socketType(type)
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
		socketStruct.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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
									 &Socket::acceptSockets,
									 static_cast<void*>(this));

	// the thread will be terminated by another thread
	pthread_join(acceptingThread, NULL);

}

std::string Socket::read()
{
	char *buffer = new char[RECV_BUFFER_SIZE]; // default 512 bytes
	recv(socketDescriptor, buffer, RECV_BUFFER_SIZE, 0);
	return std::string(buffer);
}

std::string Socket::read(int size)
{
	char *buffer = new char[size];
	recv(socketDescriptor, buffer, size, 0);
	return std::string(buffer);
}

void Socket::write(std::string msg)
{
	send(socketDescriptor, msg.c_str(), static_cast<size_t>(msg.size()), 0);
}

void Socket::stop(uint what)
{
	switch(what)
	{
		case ALL:
			shutdown(socketDescriptor, 2);
			break;
		case WRITE:
			shutdown(socketDescriptor, 1);
			break;
		case READ:
			shutdown(socketDescriptor, 0);
			break;
		default:
			throw SocketException("Unknown stop option code", what);
	}
}

void Socket::close()
{
	//stop(ALL);
	::close(socketDescriptor);
}

// slots
void *Socket::acceptSockets(void* ptr)
{
	Socket *serverSocket = static_cast<Socket*>(ptr);
	int socket;

	while(true)
	{
		if((socket = accept(serverSocket->getSocketDescriptor(),
									(sockaddr*) NULL,
									NULL)) != -1)
		{
			serverSocket->setPendingConnection(socket);
			serverSocket->emitEvent(NewConnection);
		}
	}
	return NULL;
}

void *Socket::readyReadSockets(void* ptr)
{
	Socket *serverSocket = static_cast<Socket*>(ptr);
	int controlSize = 0, cTemp = 0;

	while(controlSize != (cTemp = recv(serverSocket->getSocketDescriptor(), NULL, -1, MSG_PEEK)))
	{
		controlSize = cTemp;
		serverSocket->emitEvent(ReadyRead);
	}
	return NULL;
}

void Socket::setPendingConnection(int socket)
{
	pendingConnection = socket;
}

int Socket::getNextPendingConnection()
{
	return pendingConnection;
}

void Socket::setSocketDescriptor(int socket)
{
	socketDescriptor = socket;
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

// Event control
void Socket::bindEvent(int eventType, void(*event)(void *ptr))
{
	switch(eventType)
	{
		case NewConnection:
			newConnectionEvent = event;
			break;
		case ReadyRead:
			readyReadEvent = event;
			setupReadyRead();
			break;
		case Disconnect:
			disconnectEvent = event;
			break;
		default:
			throw SocketException("Unknow event type", 0);
	}
}

void Socket::emitEvent(int eventType)
{
		switch(eventType)
	{
		case NewConnection:
			if(newConnectionEvent) newConnectionEvent(static_cast<void*>(this));
			break;
		case ReadyRead:
			if(readyReadEvent) readyReadEvent(static_cast<void*>(this));
			break;
		case Disconnect:
			if(disconnectEvent) disconnectEvent(static_cast<void*>(this));
			break;
		default:
			throw SocketException("Unknow event type", 0);
	}
}

void Socket::setupReadyRead()
{
	// start watching readyRead, createThread for watching
	pthread_t readyReadThread;

	readyReadThread = pthread_create(&readyReadThread,
									 NULL,
									 &Socket::readyReadSockets,
									 static_cast<void*>(this));

	// the thread will be terminated by another thread
	pthread_join(readyReadThread, NULL);

}

bool Socket::isDecimalFormat(std::string ip)
{

}

Socket::Socket(const Socket& orig)
{
}

Socket::~Socket()
{
	::close(socketDescriptor);
//	delete //acceptingThread,
//			socketStruct;
}

