#include "IrcServer.hpp"
#include "Config.hpp"
#include <iostream>

int main(int ac, char **av)
{
	IrcServer irc;
  try {
	irc.loadConfig(ac == 1 ? DEFAULT_CFG_FILE : av[1]);
	irc.run();
  } catch (std::exception &e) {
		irc.log() << e.what() << std::endl;
		return (1);
  } catch (config4cpp::ConfigurationException &e) {
		irc.log() << e.c_str() << std::endl;
		return (1);
	}
	return (0);
}
