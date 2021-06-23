#include "IrcServer.hpp"
#include "Config.hpp"
#include "libft.hpp"
#include <iostream>
#include <libgen.h>
#include <unistd.h>

#define DEF_CONFIG_PATH "/etc/ircserv/ircserv.conf"

static void usage()
{
	std::cout << "usage : ./ircserv <configpath>" << std::endl;
}

static bool getConfigPath(int ac, char **av, std::string &configFile)
{
	char workdir[PATH_MAX];
	char filename[PATH_MAX];

	const char *path = ac == 2 ? av[1] : DEF_CONFIG_PATH;
	strncpy(workdir, path, PATH_MAX);
	if (chdir(dirname(workdir)))
	{
		std::cerr << ft::systemError(workdir).what() << std::endl;
		usage();
		return false;
	}
	strncpy(filename, path, PATH_MAX);
	configFile = basename(filename);
	return true;
}

int main(int ac, char **av)
{
	std::string configFile;
	IrcServer::State state = IrcServer::ALIVE;
	
	if (!getConfigPath(ac, av, configFile))
		return (1);
	while (state != IrcServer::DIE)
	{
		IrcServer irc;
		try {
			irc.loadConfig(configFile.c_str());
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
