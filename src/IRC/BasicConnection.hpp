#pragma once
#include "TcpSocket.hpp"

// class that allows to interact with an undefined connection
class BasicConnection
{
  public:
	enum Type
	{
		USER
	};

	virtual ~BasicConnection();

	Type			type() const;
	TcpSocket		*socket() const;
	unsigned		hopcount() const;
	void			writeLine(const std::string &content);
	time_t			&clock();
	bool					&pongExpected();

  protected:
	TcpSocket	*_socket;
	Type		_type;
	unsigned	_hopcount;
	time_t		_clock;
	bool		_pongExpected;

	BasicConnection(TcpSocket *socket, Type type, unsigned hopcount = 0);
};
