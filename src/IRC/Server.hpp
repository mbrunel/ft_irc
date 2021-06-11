#pragma once
#include "BasicConnection.hpp"

class Server : public BasicConnection
{
  public:
	Server(TcpSocket *socket);
	virtual ~Server();

	void setName(const std::string &name);
	const std::string &name() const;

  private:

	std::string _name;

	Server(const Server &other);
	Server	&operator=(const Server &other);
};
