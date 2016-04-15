
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"

int main(int argc, char* argv[])
{
	try
	{
		// Check command line arguments.
		if (argc != 4)
		{
			std::cerr << "Usage: ftp_server <address> <port> <ftp_root>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 21 .\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 21 .\n";
			return 1;
		}

		// Initialise the server.
		ftp::server::server s(argv[1], argv[2], argv[3]);

		// Run the server until stopped.
		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
}

