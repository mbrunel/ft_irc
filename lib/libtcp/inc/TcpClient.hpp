#pragma once
#include "TcpSocket.hpp"
#include "SslContext.hpp"

namespace tcp
{

class TcpClient
{
  public:
	TcpClient(const std::string &host, const std::string &port, bool ipv6);
	TcpClient(const std::string &host, const std::string &port, const std::string &certificatePath, const std::string &keyPath, bool ipv6);
	~TcpClient();

	const TcpSocket &socket() const;
	void writeLine(const std::string &line);
	void send();
	bool canReadLine();
	bool readLine(std::string &line);

  private:
	TcpSocket *_socket;
	SslContext ctx;

	void connect(const std::string &host, const std::string &port, bool ipv6);

	TcpClient(const TcpClient &);
	TcpClient &operator=(const TcpClient &);
};

} /* end of namespace tcp */
