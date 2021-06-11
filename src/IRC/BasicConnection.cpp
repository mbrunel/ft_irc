#include "BasicConnection.hpp"

BasicConnection::~BasicConnection()
{}

BasicConnection::Type BasicConnection::type() const
{
	return (_type);
}

TcpSocket *BasicConnection::socket() const
{
	return (_socket);
}

unsigned BasicConnection::hopcount() const
{
	return (_hopcount);
}

void BasicConnection::writeLine(const std::string &content)
{
	_socket->writeLine(content);
}

time_t &BasicConnection::clock()
{
	return _clock;
}

bool &BasicConnection::pongExpected()
{
	return _pongExpected;
}

BasicConnection::BasicConnection(TcpSocket *socket, Type type, unsigned hopcount) :
_socket(socket),
_type(type),
_hopcount(hopcount),
_clock(time(NULL)),
_pongExpected(false)
{}
