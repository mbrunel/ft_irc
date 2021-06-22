#pragma once
#include "Config.hpp"
#include "IRC.hpp"
#include "Network.hpp"
#include "TcpServer.hpp"
#include <fstream>

struct IrcServerConfig
{
	std::string					configfile;
	std::string					servername;
	std::string					shortinfo;
	size_t						maxChannels;
	size_t						maxMasks;
	time_t						ping;
	time_t						pong;
	bool						flood;
	std::string					pass;
	std::vector<std::string>	motd;

	IrcServerConfig();
	IrcServerConfig(Config &cfg);
	~IrcServerConfig();
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

	enum State
	{
		ALIVE,
		RESTART,
		DIE
	};

	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	std::ostream	&log() throw();
	State			state() const;
	void			run();
	void			loadConfig(const std::string &);

  private:

	typedef int (IrcServer::*UserCommandPointer)(User &, const IRC::Message &);
	typedef std::map<std::string, UserCommandPointer> userCommandsMap;
	typedef std::map<std::string, UserCommandPointer> serviceCommandsMap;
	typedef std::map<std::string, CommandStats> commandsStatsMap;
	typedef IRC::Message::Params Params;

	static const std::string _version;
	static const size_t maxLineSize;

	State				_state;
	time_t				creation;
	std::ofstream		logfile;
	std::ostream		_log;
	bool				init;
	std::string			creationDate;
	IrcServerConfig		config;
	tcp::TcpServer			srv;
	Network				network;
	userCommandsMap		userCommands;
	serviceCommandsMap	serviceCommands;
	commandsStatsMap	commandsStats;

	int	away(User &sender, const IRC::Message &msg);
	int	die(User &sender, const IRC::Message &msg);
	int	info(User &sender, const IRC::Message &msg);
	int	invite(User &sender, const IRC::Message &msg);
	int	join(User &sender, const IRC::Message &msg);
	int	kick(User &sender, const IRC::Message &msg);
	int	kill(User &sender, const IRC::Message &msg);
	int	list(User &sender, const IRC::Message &msg);
	int	lusers(User &sender, const IRC::Message &msg);
	int	mode(User &sender, const IRC::Message &msg);
	int	motd(User &sender, const IRC::Message &msg);
	int	names(User &sender, const IRC::Message &msg);
	int	nick(User &sender, const IRC::Message &msg);
	int	notice(User &sender, const IRC::Message &msg);
	int	oper(User &sender, const IRC::Message &msg);
	int	part(User &sender, const IRC::Message &msg);
	int	pass(User &sender, const IRC::Message &msg);
	int	ping(User &sender, const IRC::Message &msg);
	int	pong(User &sender, const IRC::Message &msg);
	int	privmsg(User &sender, const IRC::Message &msg);
	int	quit(User &sender, const IRC::Message &msg);
	int rehash(User &sender, const IRC::Message &msg);
	int restart(User &sender, const IRC::Message &msg);
	int	service(User &sender, const IRC::Message &msg);
	int	servlist(User &sender, const IRC::Message &msg);
	int	squery(User &sender, const IRC::Message &msg);
	int	stats(User &sender, const IRC::Message &msg);
	int	time(User &sender, const IRC::Message &msg);
	int	topic(User &sender, const IRC::Message &msg);
	int	user(User &sender, const IRC::Message &msg);
	int userhost(User &sender, const IRC::Message &msg);
	int	version(User &sender, const IRC::Message &msg);
	int	who(User &sender, const IRC::Message &msg);
	int whois(User &sender, const IRC::Message &msg);
	int whowas(User &sender, const IRC::Message &msg);

	void	disconnect(tcp::TcpSocket *socket, const std::string &reason) throw();
	void	disconnect(User &user, const std::string &reason, bool notifyUserQuit = false) throw();
	int		exec(BasicConnection *sender, const IRC::Message &msg);
	void	writeMessage(User &dst, const std::string &command, const std::string &content);
	int		writeNum(User &dst, const IRC::Numeric &response);
	void	writeWelcome(User &user);
	void	writeMotd(User &user);
	void	writeError(tcp::TcpSocket *s, std::string reason);
	void	pingpongProbe();
	void	flushAndCloseZombies();
	bool	floodControl(User &u);

	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
