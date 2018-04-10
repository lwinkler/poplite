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
#include <boost/thread/shared_mutex.hpp>
#include <boost/filesystem.hpp>

#define LOG BOOST_LOG_TRIVIAL

namespace pop
{
#ifndef POP_BINARY_SERIALIZATION
	// use text serialization
	using bufin = boost::archive::text_iarchive;
	using bufout = boost::archive::text_oarchive;
#else
	// use binary serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
	using bufin = boost::archive::binary_iarchive;
	using bufout = boost::archive::binary_oarchive;
#endif

	// Utils
	using noncopyable = boost::noncopyable;

	// Types for thread locks // TODO maybe create a util file
	using lock = boost::shared_mutex;
	using write_lock = boost::unique_lock<lock>;
	using read_lock =  boost::shared_lock<lock>;

	
	/// A class that contains all the parameters of one executable (either the main or an object executable)
	class system
	{
		public:
			system(int* _pargc, char **_argv) : empty_(true) {
				if(_pargc == nullptr || _argv == nullptr) {
					LOG(error) << "pop::system must be initialized at the beginning of main routine";
					exit(1);
				}

				for(int i = 1 ; i < *_pargc ; i++)
					args_.push_back(_argv[i]);
			}

			static const system& instance(int* _pargc = nullptr, char **_argv = nullptr)
			{
				static system inst(_pargc, _argv);
				if(_pargc) {
					assert(_argv != nullptr);
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
							// note: setting host name manually would be a problem for a remote object creating another object
							// ("pop-hostname"    , boost::program_options::value<std::string>(&inst.host_name_),
								// "override the hostname of your computer")
							("pop-domain"      , boost::program_options::value<std::string>(&inst.domain_),
								"the domain name of your network (e.g. local), to be added to hostname")
							("pop-cwd"         , boost::program_options::value<std::string>(&inst.cwd_),
								"set the current working directory for the main and all objects")
							("pop-path"         , boost::program_options::value<std::string>(&inst.path_)->default_value("."),
								"set the directory containing the main and objects executables")
							("pop-log"         , boost::program_options::value<int>(&inst.log_level)->default_value(2), 
								"set logging level 0 to 5")
							;

						boost::program_options::variables_map vm;
						boost::program_options::store(boost::program_options::command_line_parser(*_pargc, _argv).options(desc).allow_unregistered().run(), vm);
						boost::program_options::notify(vm);    

						if (vm.count("pop-help")) {
							exit(0);
						}

						// Remove arguments starting with --pop-
						for(int i = 1 ; i < *_pargc ; ) {
							char shift = 0;
							if(strncmp(_argv[i], "--pop-", 6) == 0) {
								// note: do not remove 2 for single arguments
								shift = 2;
							}
							if(shift) {
								while(shift) {
									// shift all args by 1
									for(int j = i ; j < *_pargc - 1 ; j++) {
										_argv[j] = _argv[j+1];
									}
									(*_pargc)--;
									_argv[*_pargc] = nullptr;
									shift--;
								}
							}
							else i++;
						}
					}
					catch(std::exception &e)
					{
						LOG(error) << "Error in arguments : " << e.what();
						std::cout << desc << std::endl;
						exit(1);
					}

					// Change working dir and set logs
					boost::log::core::get()->set_filter(boost::log::trivial::severity >= inst.log_level);
					if(!inst.cwd_.empty())
						boost::filesystem::current_path(inst.cwd_);
				}
				return inst;
			}

			std::string host_name() const
			{
				if(domain_.empty())
				{
					// if(host_name_.empty())
						return boost::asio::ip::host_name();
					// else
						// return host_name_;
				}
				else
				{
					// if(host_name_.empty())
						return boost::asio::ip::host_name() + "." + domain_;
					// else
						// return host_name_ + "." + domain_;
				}
			}

			bool is_empty() const {return empty_;}

			// System options
			int log_level;

			void print_args(std::ostream& _os) const { // TODO: MAybe replace
				for(const auto& arg: args_) {
					_os << ' ' << arg;
				}
			}

			const std::vector<std::string>& get_args() const {return args_;}

			/* Static methods */
			static char* create_string(const std::string& _str) {
				char* pc = (char*) malloc(sizeof(char) * (_str.size() + 1));
				snprintf(pc, _str.size() + 1, "%s", _str.c_str());
				return pc;
			}

			static void append_to_args(char** _args, const std::vector<std::string>& _new_args) {
				for(const auto& el : _new_args) {
					*_args = create_string(el);
					_args++;
				}
			}

			inline const std::string& path() const {return path_;}
			inline const std::string& workdir() const {return cwd_;}

		private:
			bool empty_;
			std::string domain_;
			// std::string host_name_;
			std::string cwd_;
			std::string path_;

			std::vector<std::string> args_;
	};
}


#endif
