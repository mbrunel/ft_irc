#pragma once
#include <sys/socket.h>
#include <stdint.h>
#include <netdb.h>
#include "exceptions.hpp"
#include "TcpSocket.hpp"

class Listener : public Socket
{
  public:
	Listener(int family);
	virtual ~Listener() throw();

	virtual TcpSocket*	accept();
	void				listen(int maxQueuLen = 3);

  private:
	Listener();
	Listener(const Listener &);
	Listener &operator=(const Listener &);
};
