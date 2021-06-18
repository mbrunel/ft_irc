#pragma once
#include "TcpSocket.hpp"

class TcpClient : public TcpSocket
{
  public:
	TcpClient();
	~TcpClient();
  private:
	TcpSocket *socket;
};
