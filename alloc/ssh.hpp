#ifndef POP_ALLOC_SSH_H
#define POP_ALLOC_SSH_H


#include "alloc/alloc.hpp"

#define MAX_STR 512

namespace pop{

	class ssh_allocator : public allocator
	{
		public:
			ssh_allocator(const std::string& _url): url_(_url){}
			void allocate(const std::string& _obj_name, const boost::asio::ip::tcp::endpoint& _endpoint) const
			{
				std::stringstream ss;
				ss << "ssh" << " " << url_ << " ./" << _obj_name << " " << _endpoint.address() << " " << _endpoint.port();
				LOG(debug) << "Run object with :" << ss.str();

				/*Spawn a child to run the program.*/
				pid_t pid=fork();
				if (pid==0) { /* child process */
					char buf0[MAX_STR];
					std::stringstream ss1;
					std::stringstream ss2;
					snprintf(buf0, sizeof(buf0), "./%s", _obj_name.c_str());

					ss1 << _endpoint.address();
					ss2 << _endpoint.port();

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
