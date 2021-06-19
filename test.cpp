#include "TcpClient.hpp"
#include <iostream>

int main()
{
	try {
		tcp::TcpClient client("localhost", "6667", true);
		client.writeLine("nick hop");
		client.writeLine("user dklfj 0 * lkdjf");
		client.send();
		std::string line;
		while (client.readLine(line))
		{
			std::cout << line << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
