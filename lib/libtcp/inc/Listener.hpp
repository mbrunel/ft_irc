#pragma once
#include "TcpSocket.hpp"

namespace tcp
{

/**
 * A passive Socket
 */
class Listener : public Socket
{
  public:
	/**
	* Calls Socket::socket() method
	*/
	Listener(int family);
	virtual ~Listener() throw();

	/**
	 * Creates a new TcpSocket
	 */
	virtual TcpSocket*	accept();

	/**
	 * Listen() system call
	 * \param maxQueulen maximum number of simultanate connections
	 */
	void				listen(int maxQueuLen = 3);

  private:
	Listener();
	Listener(const Listener &);
	Listener &operator=(const Listener &);
};

} /* end of namespace tcp */
