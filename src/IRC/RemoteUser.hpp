#pragma once
#include "User.hpp"

class RemoteUser : public User
{
public:
	RemoteUser(TcpSocket *socket, int hopcount);
	virtual ~RemoteUser();

private:
	RemoteUser(const RemoteUser &other);

	RemoteUser	&operator=(const RemoteUser &other);
};
