#pragma once
#include "Server.hpp"

class RemoteServer : public Server
{
public:
	RemoteServer(TcpSocket *socket, int hopcount);
	virtual ~RemoteServer();

private:
	RemoteServer(const RemoteServer &other);

	RemoteServer	&operator=(const RemoteServer &other);
};
