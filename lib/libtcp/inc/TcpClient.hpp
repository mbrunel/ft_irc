#pragma once
#include "TcpSocket.hpp"
#include "SslContext.hpp"

namespace tcp
{

/**
 * A client using tcp which can be herited
 */
class TcpClient
{
  public:

	/**
	 * Creates either a TcpSocket or an SslSocket then connect them to host and port
	 * \param host Either an ip or a domain name to a server
	 * \param port Port number to connect to
	 * \param ipv6 if true connect with an ipv6 address
	 * \param certificatePath Path to the ssl certificate file
	 * \param keyPath Path to the sll key file
	 */
	TcpClient(const std::string &host, const std::string &port, bool ipv6,
				const std::string &certificatePath, const std::string &keyPath);
	virtual ~TcpClient();

	/**
	 * Calls TcpSocket::readLine() in a nonblocking way
	 * \param line the line which is writte
	 */
	bool tryReadLine(std::string &line);

	/**
	 * Calls TcpSocket::readLine() in a blocking way
	 * \param line the line which is writte
	 */
	bool waitForLine(std::string &line);

	/**
	 * Calls TcpSocket::writeLine()
	 * \param line the line which is writte
	 */
	void writeLine(const std::string &line);

	/**
	 * Calls TcpSocket::flush() or SslSocket::flush() depending
	 */
	void flush();

  private:
	TcpSocket *_socket;
	SslContext ctx;

	const TcpSocket &socket() const;
	void connect(const std::string &host, const std::string &port, bool ipv6);

	TcpClient(const TcpClient &);
	TcpClient &operator=(const TcpClient &);
};

} /* end of namespace tcp */
