#pragma once
#include "TcpSocket.hpp"
#include "SslContext.hpp"

namespace tcp
{

class TcpClient
{
  public:
	TcpClient(const std::string &host, const std::string &port, bool ipv6);
	TcpClient(const std::string &host, const std::string &port, bool ipv6,
				const std::string &certificatePath, const std::string &keyPath);
	~TcpClient();

	const TcpSocket &socket() const;
	bool tryReadLine(std::string &line);
	bool waitForLine(std::string &line);
	void writeLine(const std::string &line);
	void flush();

  private:
	TcpSocket *_socket;
	SslContext ctx;

	void connect(const std::string &host, const std::string &port, bool ipv6);

	TcpClient(const TcpClient &);
	TcpClient &operator=(const TcpClient &);
};

} /* end of namespace tcp */
