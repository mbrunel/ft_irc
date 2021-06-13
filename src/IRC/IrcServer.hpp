#pragma once
#include "IrcNumeric.hpp"
#include "Network.hpp"
#include "TcpServer.hpp"
#include "Message.hpp"
#include "Config.hpp"

struct IrcServerConfig
{
	std::string	version;
	std::string servername;
	std::string shortinfo;
	size_t		maxChannels;
	size_t		maxMasks;
	time_t		ping;
	time_t		pong;
	bool		flood;
	std::string	motdfile;
	std::string infofile;
	std::vector<std::string> motd;
	std::vector<std::string> info;

	IrcServerConfig();
	IrcServerConfig(Config &cfg);
};

struct CommandStats
{
	size_t	count;
	size_t 	byteCount;

	CommandStats();
};

class IrcServer
{
  public:
	IrcServer();
	~IrcServer();

	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	std::ostream	&log() throw();
	void			run() throw();
	void			setConfig(Config &cfg);

  private:
	typedef int (IrcServer::*UserCommandPointer)(User &, const Message &);
	typedef std::map<std::string, UserCommandPointer> userCommandsMap;
	typedef std::map<std::string, CommandStats> commandsStatsMap;
	typedef std::vector<Param> Params;

	time_t				creation;
	std::string			creationDate;
	IrcServerConfig		config;
	TcpServer			srv;
	Network				network;
	userCommandsMap		userCommands;
	commandsStatsMap	commandsStats;

	int	away(User &sender, const Message &msg);
	int info(User &sender, const Message &msg);
	int	invite(User &sender, const Message &msg);
	int	join(User &sender, const Message &msg);
	int	kick(User &sender, const Message &msg);
	int	kill(User &sender, const Message &msg);
	int	list(User &sender, const Message &msg);
	int	lusers(User &sender, const Message &msg);
	int	mode(User &sender, const Message &msg);
	int	motd(User &sender, const Message &msg);
	int	names(User &sender, const Message &msg);
	int	nick(User &sender, const Message &msg);
	int	notice(User &sender, const Message &msg);
	int	oper(User &sender, const Message &msg);
	int	part(User &sender, const Message &msg);
	int	ping(User &sender, const Message &msg);
	int	pong(User &sender, const Message &msg);
	int	privmsg(User &sender, const Message &msg);
	int	quit(User &sender, const Message &msg);
	int	stats(User &sender, const Message &msg);
	int	time(User &sender, const Message &msg);
	int	topic(User &sender, const Message &msg);
	int	user(User &sender, const Message &msg);
	int	version(User &sender, const Message &msg);
	int	who(User &sender, const Message &msg);
	int whois(User &sender, const Message &msg);
	int whowas(User &sender, const Message &msg);

	void	disconnect(TcpSocket *socket, const std::string &reason) throw();
	void	disconnect(User &user, const std::string &reason, bool notifyUserQuit = false) throw();
	int		exec(BasicConnection *sender, const Message &msg);
	void	writeMessage(User &dst, const std::string &command, const std::string &content);
	int		writeNum(User &dst, const IrcNumeric &response);
	void	writeWelcome(User &user);
	void	writeMotd(User &user);
	void	writeError(TcpSocket *s, std::string reason);
	void	Police();
	bool	floodControl(User &u);

	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
