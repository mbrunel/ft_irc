#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include "IRC.hpp"
#include "system_error.hpp"

static int parsePort(const char *s)
{
	char *s2 = NULL;
	unsigned long port = std::strtoul(s, &s2, 10);
	return (s < s2 && !*s2 && port <= std::numeric_limits<uint16_t>::max() ? port : -1);
}

static int usage()
{
	std::cerr << "usage: nestor [HOST] [PORT] [PASS: optionnal]" << std::endl;
	return (EXIT_FAILURE);
}

class NestorBot
{
public:
	NestorBot(const char *hostname, uint16_t port, const std::string &pass) :
	_socket(-1), _n(0)
	{
		sockaddr_in serv_addr;
		hostent *host = gethostbyname(hostname);
		if (!host)
			throw ft::system_error("gethostbyname");
		if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw ft::system_error("socket");
		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy(&(serv_addr.sin_addr), host->h_addr, host->h_length);
		serv_addr.sin_port = htons(port);
		if (connect(_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
			throw ft::system_error("connect");
		if (pass.size())
			send((IRC::MessageBuilder("PASS") << pass));
		send((IRC::MessageBuilder("SERVICE") << "Nestor" << "*" << "*" << "*" << "*" << "Nestor bot"));
	}

	~NestorBot()
	{
		if (_socket >= 0)
			close(_socket);
	}

	void run()
	{
		while (readline())
		{
			std::cerr << "LINE: " << _line;
			IRC::Message m(_line);
			std::cerr << m << std::endl;
			if (m.command() == "PING")
				send((IRC::MessageBuilder("PONG") << m.params()[0]));
		}
	}

private:
	static const size_t BUFSIZE = 1024;

	int			_socket;
	char		_buf[BUFSIZE + 1];
	size_t		_n;
	std::string	_line;

	bool readline()
	{
		_line.clear();
		char *sep = strchr(_buf, '\n');
		if (!sep && !recv(0))
			return (0);
		if ((sep = strchr(_buf, '\n')))
		{
			char c = sep[1];
			sep[1] = '\0';
			_line = _buf;
			sep[1] = c;
			memmove(_buf, sep + 1, BUFSIZE - _line.size());
			_n -= _line.size();
		}
		return (1);
	}

	size_t recv(int flags = 0)
	{
		ssize_t n = ::recv(_socket, _buf + _n, BUFSIZE - _n, flags);
		if (n < 0)
			_n = 0;
		else
			_n += n;
		_buf[_n] = 0;
		return (n < 0 ? 0 : n);
	}

	void send(const IRC::MessageBuilder &message, int flags = 0) const
	{
		std::string buf = message.str() + '\n';
		const char *p = buf.c_str();
		size_t n = buf.size();
		while (n > 0)
		{
			ssize_t m = ::send(_socket, p, n, flags);
			if (m < 0)
				throw ft::system_error("send");
			p += m;
			n -= m;
		}
	}
};

int main(int ac, const char **av)
{
	int port;

	if (ac < 3 || (port = parsePort(av[2])) < 0)
		return usage();
	try
	{
		NestorBot bot(av[1], port, (ac > 3 ? av[3] : ""));
		try
		{
			bot.run();
		}
		catch (const std::exception &e)
		{
			std::cerr << "NestorBot: " << e.what() << std::endl;
		}
	}
	catch(const std::exception &e)
	{
		std::cerr << "NestorBot: " << e.what() << std::endl;
	}
	return 0;
}
