//
// ssh.hpp
// ~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _POP_PARSER_
#ifndef POP_ALLOC_SSH_H
#define POP_ALLOC_SSH_H

#include "class/system.hpp"
#include "alloc/alloc.hpp"

/// Class to allocate an object via ssh
namespace pop {

class ssh_allocator : public allocator {
public:
	ssh_allocator(const std::string& _url): url_(_url) {}
	void allocate(const std::string& _executable, const std::string& _class_name, const pop::accesspoint& _callback) const {
		std::stringstream ss;
		const auto& popsys = pop::system::instance();
		ss << "ssh" << " " << url_ << " " << popsys.path() << "/" << _executable << " '" << _class_name << "'" << " " << _callback.host_name << " " << _callback.port;
		popsys.print_args(ss);
		LOG(debug) << "Run object with: " << ss.str();

		/*Spawn a child to run the program.*/
		pid_t pid=fork();
		if (pid==0) { /* child process */
			size_t s = 6 + popsys.get_args().size() + 1;
			char** arg_arr = (char**) malloc(sizeof(char*) * s);
			arg_arr[0] = pop::system::create_string("ssh");
			arg_arr[1] = pop::system::create_string(url_);
			arg_arr[2] = pop::system::create_string(popsys.path() + "/" + _executable);
			arg_arr[3] = pop::system::create_string(_class_name);
			arg_arr[4] = pop::system::create_string(_callback.host_name);
			arg_arr[5] = pop::system::create_string(std::to_string(_callback.port));
			pop::system::append_to_args(arg_arr + 6, popsys.get_args());
			arg_arr[s - 1] = nullptr;
			execvp(arg_arr[0], arg_arr);
			perror("Error in execution of object file");

			throw std::runtime_error("Error while running " + ss.str());
		} else { /* pid!=0; parent process */
			// waitpid(pid,0,0); /* do not wait for child to exit */
		}
	}
private:
	std::string url_;
};
}

#endif
#endif
