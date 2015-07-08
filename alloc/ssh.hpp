//
// ssh.hpp
// ~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_ALLOC_SSH_H
#define POP_ALLOC_SSH_H


#include "class/system.hpp"

#define MAX_STR 512

/// Class to allocate an object via ssh
namespace pop{

	class ssh_allocator : public allocator
	{
		public:
			ssh_allocator(const std::string& _url): url_(_url){}
			void allocate(const std::string& _obj_name, const pop::accesspoint& _callback) const
			{
				std::stringstream ss;
				ss << "ssh" << " " << url_ << " ./" << _obj_name << " " << _callback.host_name << " " << _callback.port;
				LOG(debug) << "Run object with :" << ss.str();

				/*Spawn a child to run the program.*/
				pid_t pid=fork();
				if (pid==0) { /* child process */
					char buf0[MAX_STR];
					std::stringstream ss1;
					std::stringstream ss2;
					snprintf(buf0, sizeof(buf0), "./%s", _obj_name.c_str());

					ss1 << _callback.host_name;
					ss2 << _callback.port;

					execlp("ssh", "ssh", url_.c_str(), buf0, ss1.str().c_str(), ss2.str().c_str(), (const char*)nullptr);
					perror("Error in execution of object file");

					throw std::runtime_error("Error while running " + ss.str());
				}
				else { /* pid!=0; parent process */
					// waitpid(pid,0,0); /* do not wait for child to exit */
				}
			}
		private:
			std::string url_;
	};
}

#endif
