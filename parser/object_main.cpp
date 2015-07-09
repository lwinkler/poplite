#include "com/broker_combox.hpp"

int main(int argc, char* argv[])
{

	boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info); // TODO: parameters
	// pop::system::instance(argc, argv);

	try
	{
		// Check command line arguments.
		if (argc != 3)
		{

			LOG(error) << argc << "Usage: " << argv[0] << " <hostname of interface> <port of interface>";
			return -1;
		}
		pop::remote::broker<_parclass_> brok;
		boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
		pop::broker_combox<_parclass_> combox(brok, query);
		combox.run();
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
		return 1;
	}

	return 0;
}
