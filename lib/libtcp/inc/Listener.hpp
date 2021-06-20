#pragma once
#include "TcpSocket.hpp"

namespace tcp
{

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

} /* end of namespace tcp */
