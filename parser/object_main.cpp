#include "com/broker_combox.hpp"
// TODO: Merge with broker

int main(int _argc, char* _argv[])
{
	pop::system::instance(&_argc, _argv);

	try
	{
		// Check command line arguments.
		if (_argc < 3)
		{
			LOG(error) << "Usage: " << _argv[0] << " <hostname of interface> <port of interface>";
			return -1;
		}
		pop::remote::broker<_parclass_> brok;
		boost::asio::ip::tcp::resolver::query query(_argv[1], _argv[2]);
		pop::broker_combox<_parclass_> combox(brok, query);
		combox.run();
	}
	catch (std::exception& e)
	{
		LOG(error) << "Exception in object main: " << e.what();
		return 1;
	}
	catch (...)
	{
		LOG(error) << "Unknown exception in object main";
		return 1;
	}

	return 0;
}
