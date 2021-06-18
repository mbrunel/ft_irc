#include "IrcServer.hpp"
#include "Config.hpp"
#include <iostream>

static void usage()
{
	std::cout << "usage : ./ircserv <configpath>" << std::endl;
}

int main(int ac, char **av)
{
	IrcServer::State state = IrcServer::ALIVE;

	if (ac != 2)
	{
		usage();
		return 1;
	}
	while (state != IrcServer::DIE)
	{
		IrcServer irc;
		try {
			irc.loadConfig(av[1]);
			irc.run();
			state = irc.state();
		}
		catch (std::exception &e) {
			irc.log() << e.what() << std::endl;
			return (1);
		}
		catch (config4cpp::ConfigurationException &e) {
			irc.log() << e.c_str() << std::endl;
			return (1);
		}
	}
	return (0);
}
