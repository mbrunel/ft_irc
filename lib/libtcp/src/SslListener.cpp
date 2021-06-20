#include "SslListener.hpp"
#include "SslSocket.hpp"

namespace tcp
{

SslListener::SslListener(int family, SSL_CTX *ctx):Listener(family), _ctx(ctx) {}

SslListener::~SslListener() throw() {}

TcpSocket *SslListener::accept() { return (new SslSocket(_fd, _ctx)); }

} /* end of namespace tcp */
