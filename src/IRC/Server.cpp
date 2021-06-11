#include "Server.hpp"

Server::Server(TcpSocket *socket):
BasicConnection(socket, SERVER)
{}

Server::~Server() {}

const std::string &Server::name() const
{
	return _name;
}

void Server::setName(const std::string &name)
{
	_name = name;
}
