#pragma once
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <set>
#include <time.h>
#include "config4cpp/Configuration.h"

#define SERVER_SCOPE "ServerConfig"
#define IRC_SCOPE "IrcConfig"

typedef config4cpp::Configuration Configuration;

struct Oper
{
	Oper();
	Oper(std::string login, std::string pass, std::string host);
	std::string login;
	std::string pass;
	std::string host;
};

class Config
{
  public:
	Config(int ac, char **av);
	void usage() const;
	void servername(std::string &);
	void shortinfo(std::string &);
	void motdfile(std::string &);
	void infofile(std::string &);
	std::string certFile();
	std::string keyFile();
	std::string tcpPort();
	std::string sslPort();
	std::string logfile();
	time_t ping();
	time_t pong();
	bool floodControl();
	int maxConnections();
	int maxChannels();
	int maxMasks();
	bool verbose();
	void opers(std::map<std::string, Oper> &opers);
	void fnicks(std::set<std::string> &fnicks);

  private:
	Configuration *cfg;
};
