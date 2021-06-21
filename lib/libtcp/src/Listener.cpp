#include "Listener.hpp"
#include "systemError.hpp"
#include <sys/socket.h>

namespace tcp
{

Listener::Listener(int family):Socket() { socket(family); }

Listener::~Listener() throw() { close(); }

void Listener::listen(int maxQueueLen)
{
	if (::listen(_fd, maxQueueLen) == -1)
		throw ft::systemError("listen");
}

TcpSocket *Listener::accept() { return (new TcpSocket(_fd)); }

} /* end of namespace tcp */
