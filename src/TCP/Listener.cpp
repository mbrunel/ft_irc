#include "Listener.hpp"

Listener::Listener(int family):Socket() { socket(family); }

Listener::~Listener() throw() { close(); }

void Listener::listen(int maxQueueLen)
{
	if (::listen(_fd, maxQueueLen) == -1)
		throw ft::system_error("listen");
}

TcpSocket *Listener::accept() { return (new TcpSocket(_fd)); }
