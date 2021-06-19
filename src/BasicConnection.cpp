#include "BasicConnection.hpp"

BasicConnection::~BasicConnection()
{}

BasicConnection::Type BasicConnection::type() const
{
	return (_type);
}

tcp::TcpSocket *BasicConnection::socket() const
{
	return (_socket);
}

unsigned BasicConnection::hopcount() const
{
	return (_hopcount);
}

void BasicConnection::setType(const Type type)
{
	_type = type;
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

BasicConnection::BasicConnection(tcp::TcpSocket *socket, Type type, unsigned hopcount) :
_socket(socket),
_type(type),
_hopcount(hopcount),
_clock(time(NULL)),
_pongExpected(false)
{}
