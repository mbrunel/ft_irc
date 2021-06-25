#include <arpa/inet.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include "IRC.hpp"
#include "libft.hpp"
#include "TcpClient.hpp"

bool alive = true;

void handler(int sig)
{
	(void)sig;
	alive = false;
}

struct Argument
{
	std::string name, description;

	Argument(const std::string &name, const std::string &description) :
	name(name), description(description)
	{}
};

typedef std::vector<Argument> Arguments;
typedef std::vector<std::string> Params;

template<class T>
struct Command
{
	T			command;
	std::string	description;
	Arguments	args, opts;

	Command(const T &command = T(), const std::string &description = "", const Arguments &args = Arguments(), const Arguments &opts = Arguments()) :
	command(command), description(description), args(args), opts(opts)
	{}

	std::string usage() const
	{
		std::stringstream buf;
		for (size_t i = 0; i < args.size(); ++i)
			buf << ' ' << args[i].name;
		for (size_t i = 0; i < opts.size(); ++i)
			buf << " [" << opts[i].name << ']';
		return (buf.str());
	}
};

class NestorBot : public tcp::TcpClient
{
	typedef int (NestorBot::*CommandPointer)(const IRC::Param &, const Params &);
	typedef Command<CommandPointer> Command;
	typedef std::map<std::string, Command> CommandsMap;

	static const size_t BUFSIZE = 1024;
	static const size_t CMDMAXSIZE = 20;
	static const size_t JOKEMAXSIZE = 256;

public:
	NestorBot(const std::string &jokesFile, const std::string &hostname, const std::string &port, const std::string &pass, const std::string &cert, const std::string &key) :
	tcp::TcpClient(hostname, port, false, cert, key),
	_curl(NULL)
	{
		Command cmd(&NestorBot::help, "View a list of all available commands");
		cmd.opts.push_back(Argument("COMMAND", "View the full description of a command"));
		_commands["HELP"] = cmd;
		_commands["JOKE"] = Command(&NestorBot::joke, "Ask for a joke");
		_commands["ROLL"] = Command(&NestorBot::roll, "Roll a dice");
		cmd = Command(&NestorBot::weather, "Request a weather report");
		cmd.args.push_back(Argument("LOCATION", "A city name, a country, GPS coordinates..."));
		_commands["WEATHER"] = cmd;

		ft::fileToData(jokesFile, _jokes, JOKEMAXSIZE);
		if (pass.size())
			TcpClient::writeLine((IRC::MessageBuilder("PASS") << pass).str());
		TcpClient::writeLine((IRC::MessageBuilder("SERVICE") << "Nestor" << "*" << "*" << "*" << "*" << "Nestor bot").str());
		flush();
		if (!(_curl = curl_easy_init()))
			throw ft::systemError("curl_easy_init");
	}

	~NestorBot()
	{
		if (_curl)
			curl_easy_cleanup(_curl);
	}

	void run()
	{
		std::string line;
		for (; waitForLine(line) && alive; flush())
		{
			IRC::Message m(line);
			#ifndef NDEBUG
				std::cout << m << std::endl;
			#endif
			if (m.command() == "PING" && m.params().size())
				TcpClient::writeLine((IRC::MessageBuilder("PONG") << m.params()[0]).str());
			else if (m.params().size() < 2)
				continue ;
			if (m.command() == "SQUERY")
			{
				Params args = split(m.params()[1]);
				if (args.empty())
				{
					writeError(m.prefix().nickname(), "No command given.");
					continue ;
				}
				std::string command = args[0];
				ft::toUpper(command);
				CommandsMap::const_iterator i = _commands.find(command);
				if (i == _commands.end())
				{
					if (command.size() > CMDMAXSIZE)
						command = command.substr(0, CMDMAXSIZE);
					writeError(m.prefix().nickname(), command + " - Command not found. Type /SQUERY Nestor HELP for more information.");
				}
				else if (args.size() - 1 < i->second.args.size() || (this->*(i->second.command))(m.prefix().nickname(), args) < 0)
					writeLine(m.prefix().nickname(), "USAGE: /SQUERY Nestor " + command + i->second.usage());
			}
		}
	}

private:
	CommandsMap					_commands;
	std::vector<std::string>	_jokes;
	CURL						*_curl;

	int help(const IRC::Param &nick, const Params &args)
	{
		if (args.size() < 2)
		{
			writeLine(nick , "< Nestor Bot - HELP >");
			writeLine(nick , "Use /SQUERY Nestor HELP [COMMAND] to view more information about a specific command.");
			writeLine(nick , "Commands list:");
			for (CommandsMap::const_iterator i = _commands.begin(); i != _commands.end(); ++i)
				writeLine(nick , i->first + ": " + i->second.description);
			return 0;
		}
		std::string command = args[1];
		ft::toUpper(command);
		CommandsMap::const_iterator i = _commands.find(command);
		if (i == _commands.end())
			return (-1);
		const Command &c = i->second;
		writeLine(nick , "< Nestor Bot - HELP - " + i->first + " >");
		writeLine(nick , '\t' + c.description);
		writeLine(nick , "Usage:");
		writeLine(nick , '\t' + i->first + c.usage());
		if (c.args.size())
		{
			writeLine(nick , "Arguments list:");
			for (size_t i = 0; i < c.args.size(); ++i)
				writeLine(nick , '\t' + c.args[i].name + ": " + c.args[i].description);
		}
		if (c.opts.size())
		{
			writeLine(nick , "Optional arguments list:");
			for (size_t i = 0; i < c.opts.size(); ++i)
				writeLine(nick , '\t' + c.opts[i].name + ": " + c.opts[i].description);
		}
		return (0);
	}

	int joke(const IRC::Param &nick, const Params &)
	{
		writeLine(nick, _jokes[std::rand() % _jokes.size()]);
		return (0);
	}

	int roll(const IRC::Param &nick, const Params &)
	{
		writeLine(nick, ft::toString((std::rand() % 6) + 1));
		return (0);
	}

	int weather(const IRC::Param &nick, const Params &args)
	{
		const std::string set = "\\;{}()*&$%?#/!=";
		for (size_t i = 0; i < set.size(); ++i)
			if (std::memchr(args[1].c_str(), set[i], args[1].size()))
			{
				writeError(nick, "WEATHER - Illegal argument");
				return (0);
			}
		const std::string location = "https://wttr.in/" + args[1] + "?0AT&lang=en";
		std::stringstream ss;
		curl_easy_setopt(_curl, CURLOPT_URL, location.c_str());
		curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &NestorBot::writeCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &ss);
		CURLcode r = curl_easy_perform(_curl);
		if (r != CURLE_OK)
			writeError(nick, "WEATHER - Service temporary unavailable");
		else
			for (std::string line; std::getline(ss, line);)
				writeLine(nick, line);
		return (0);
	}

	void writeError(const IRC::Param &nick, const std::string &content)
	{
		writeLine(nick, "ERROR: " + content);
	}

	void writeLine(const IRC::Param &nick, const std::string &content)
	{
		TcpClient::writeLine((IRC::MessageBuilder("NOTICE") << nick << content).str());
	}

	Params split(const std::string &s) const
	{
		std::stringstream ss;
		Params args;

		ss << s;
		for (std::string word; ss >> word;)
			args.push_back(word);
		return (args);
	}

	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t n = size * nmemb;
		static_cast<std::stringstream *>(userp)->write(static_cast<const char *>(contents), n);
		return n;
	}
};

int main(int ac, const char **av)
{
	signal(SIGINT, handler);
	signal(SIGPIPE, SIG_IGN);
	siginterrupt(SIGINT, 1);
	if (ac < 4)
	{
		std::cerr << "usage: nestor JOKES_FILE HOST PORT [PASS] [ssl-cert ssl-key]" << std::endl;
		return (EXIT_FAILURE);
	}
	std::srand(getpid() * ::time(NULL));
	try
	{
		NestorBot bot(av[1], av[2], av[3], (ac > 4 ? av[4] : ""), (ac > 5 ? av[5] : ""), (ac > 6 ? av[6] : ""));
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
