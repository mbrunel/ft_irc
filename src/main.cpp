#include "IrcServer.hpp"
#include "Config.hpp"
#include <iostream>

int main(int ac, char **av)
{
	IrcServer irc;
  try {
	Config cfg(ac, av);
	SslContext ctx(cfg.certFile().c_str(), cfg.keyFile().c_str());
	irc.setConfig(cfg);
	irc.listen(cfg.tcpPort().c_str());
	irc.listen(cfg.sslPort().c_str(), ctx.ctx());
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
