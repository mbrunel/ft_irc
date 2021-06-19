#pragma once
#include "TcpSocket.hpp"

// class that allows to interact with an undefined connection
class BasicConnection
{
  public:
	enum Type
	{
		SERVICE,
		USER
	};

	virtual ~BasicConnection();

	Type			type() const;
	tcp::TcpSocket	*socket() const;
	unsigned		hopcount() const;
	void			setType(const Type type);
	void			writeLine(const std::string &content);
	time_t			&clock();
	bool			&pongExpected();

  protected:
	tcp::TcpSocket	*_socket;
	Type			_type;
	unsigned		_hopcount;
	time_t			_clock;
	bool			_pongExpected;

	BasicConnection(tcp::TcpSocket *socket, Type type, unsigned hopcount = 0);
};
