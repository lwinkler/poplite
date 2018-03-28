//
// system.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POPLITE_SYSTEM_H
#define POPLITE_SYSTEM_H


#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "parser/defs.hpp"

// Lines for boost log
#define BOOST_LOG_DYN_LINK
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#define LOG BOOST_LOG_TRIVIAL

namespace pop
{
#ifndef POP_BINARY_SERIALIZATION
	// use text serialization
	typedef boost::archive::text_iarchive bufin;
	typedef boost::archive::text_oarchive bufout;
#else
	// use binary serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
	typedef boost::archive::binary_iarchive bufin;
	typedef boost::archive::binary_oarchive bufout;
#endif
	
	/// A class that contains all the parameters of one executable (either the main or an object executable)
	class system
	{
		public:
			system():empty_(true){}
			system(int argc, char **argv) : empty_(false) {(void)argc;(void)argv;} // TODO: Are arguments used ?

			static const system& instance(int argc = 0, char **argv = nullptr)
			{
				static system inst;
				if(argc)
				{
					// Declare the supported options.
					boost::program_options::options_description desc("Allowed options");

					try
					{
						if(!inst.is_empty())
						{
							LOG(error) << "pop::system can only be initialized once";
							return inst;
						}

						desc.add_options()
							("pop-help"        , "produce help message")
							("pop-hostname"    , boost::program_options::value<std::string>(&inst.host_name_),
								"override the hostname of your computer")
							("pop-domain"      , boost::program_options::value<std::string>(&inst.domain_),
								"the domain name of your network (e.g. local) , to be added to hostname")
							("pop-log"         , boost::program_options::value<int>(&inst.log_level)->default_value(2), 
								"set logging level 0 to 5")
							;

						boost::program_options::variables_map vm;
						// boost::program_options::store(boost::program_options::parsers::basic_command_line_parser(argc, argv, desc), vm);
						boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
						boost::program_options::notify(vm);    

						if (vm.count("pop-help")) {
							std::cout << desc << std::endl;
							exit(0);
						}

						boost::log::core::get()->set_filter(boost::log::trivial::severity >= inst.log_level);
					}
					catch(std::exception &e)
					{
						LOG(error) << "Error in arguments : " << e.what();
						std::cout << desc << std::endl;
					}
				}
				return inst;
			}

			std::string host_name() const
			{
				if(domain_.empty())
				{
					if(domain_.empty())
						return boost::asio::ip::host_name();
					else
						return host_name_;
				}
				else
				{
					if(domain_.empty())
						return boost::asio::ip::host_name() + "." + domain_;
					else
						return host_name_ + "." + domain_;
				}
			}

			bool is_empty() const {return empty_;}

			// System options
			int log_level;

		private:
			bool empty_;
			std::string domain_;
			std::string host_name_;
	};
}


#endif
