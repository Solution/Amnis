/*
 * File:   Socket.h
 * Author: solution
 *
 * Created on 18. duben 2012, 10:35
 */

#ifndef SOCKET_H
#define	SOCKET_H

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "SocketException.h"

typedef unsigned int uint;

#define LOCALHOST_IP "127.0.0.1"
#define RECV_BUFFER_SIZE 512

enum Protocol { TCP, UDP };
enum SocketStop { READ, WRITE, ALL };
enum AddressType { LOCALHOST, ALL_AVAILABLE };
enum EventType { NewConnection, ReadyRead, Disconnect };

class Socket
{

public:
	Socket(); // create without any bindings
	Socket(int); // create the socket
	Socket(const Socket&);
	virtual ~Socket();

	const static uint MAXIMUM_CONNECTIONS = 5; //

	// Socket operations
	void bindOn(const std::string, // could be decimal format with dots, or hostname
	uint); // bind socket on concrete address, port
	void bindOn(int type, uint port);

	void start(uint); //
	bool isListenning(); // return if socket is listenning

	int getNextPendingConnection(); // return a connection

	void write(std::string);
	std::string read();
	std::string read(int);

	void stop(uint);

	int getSocketDescriptor() const;
	void setSocketDescriptor(int);
	std::string getSocketAddress() const;
	uint getSocketPort() const;

	void setPendingConnection(int);

	// Event control
	void bindEvent(int eventType, void(*event)(void *ptr));
	void emitEvent(int eventType);
	bool isReadyReadCalled;

	// Events
	void (*newConnectionEvent)(void *ptr);
	void (*readyReadEvent)(void *ptr);
	void (*disconnectEvent)(void *ptr);
	//

	void close();
	
private:
	int socketDescriptor;
	uint port;
	std::string address;
	struct sockaddr_in socketStruct;

	// Event control
	void setupReadyRead();
	pthread_t acceptingThread;

	int pendingConnection;

	bool isDecimalFormat(std::string ip);
};

#endif	/* SOCKET_H */

