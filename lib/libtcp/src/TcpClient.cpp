#include "TcpClient.hpp"
#include "SslSocket.hpp"
#include "libft.hpp"
#include <netdb.h>
#include <signal.h>

namespace tcp
{

TcpClient::TcpClient(const std::string &host, const std::string &port, bool ipv6,
					const std::string &certificatePath, const std::string &keyPath):ctx(SslContext::CLIENT)
{
	signal(SIGPIPE, SIG_IGN);
	if (certificatePath.empty())
		_socket = new TcpSocket();
	else
	{
		ctx.load(certificatePath.c_str(), keyPath.c_str());
		_socket = new SslSocket(ctx.ctx());
	}
	try {
		connect(host, port, ipv6);
		if (!certificatePath.empty())
		{
			_socket->unsetNonblock();
			(static_cast<SslSocket *>(_socket))->SSL_connect();
		}
	}
	catch (std::exception &e) { delete _socket; throw; }
}

TcpClient::~TcpClient() { delete _socket; }

void TcpClient::connect(const std::string &host, const std::string &port, bool ipv6)
{
	addrinfo hint, *head, *node;
	bzero(&hint, sizeof(hint));
	hint.ai_family = ipv6 ? AF_INET6 : AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(host.c_str(), port.c_str(), &hint, &head))
		throw ft::systemError("getaddrinfo");
	try {
		for (node = head; node != NULL; node = node->ai_next)
		{
			_socket->socket(node->ai_family);
			if (_socket->fd() == -1)
				continue ;
			if (::connect(_socket->fd(), node->ai_addr, node->ai_addrlen) != -1)
				break;
			_socket->close();
		}
	}
	catch (std::exception &e) {
		freeaddrinfo(head);
		throw ;
	}
	freeaddrinfo(head);
}

void TcpClient::writeLine(const std::string &line)
{
	_socket->writeLine(line);
}

void TcpClient::flush()
{
	_socket->_isWriteable = true;
	_socket->unsetNonblock();
	_socket->flush();
}

bool TcpClient::tryReadLine(std::string &line)
{
	_socket->_isReadable = true;
	_socket->setNonblock();
	return _socket->readLine(line);
}

bool TcpClient::waitForLine(std::string &line)
{
	_socket->_isReadable = true;
	_socket->unsetNonblock();
	return _socket->readLine(line);
}

} /* end of namespace tcp */
