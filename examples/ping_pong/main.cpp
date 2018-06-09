//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "class/broker.hpp"
#include "com/broker_combox.hpp"

#include "ping_pong.hpp"
#include "mutex.hpp"

using namespace std;

/// Two remote objects are calling each other until counter reaches zero

namespace pop {
template<typename T> class local_object final {
	public:
		local_object() : combox_(brok_) {
			brok_.ptr_obj().reset(new T());

			contact_.host_name = pop::system::instance().host_name();
			contact_.port = combox_.contact().port();
		}

		inline void run() {
			combox_.run();
		}

		inline void stop() {
			combox_.stop();
		}

		inline T& object() {
			return *(brok_.ptr_obj());
		}
		
		inline const pop::accesspoint& contact() const {
			return contact_;
		}
	private:
		pop::accesspoint contact_;
		pop::remote::broker<T> brok_;
		pop::broker_combox<T> combox_;
};
} // namespace pop

int main(int argc, char* argv[]) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	if(argc < 4) {
		cerr << "usage: " << argv[0] << " <sync or async> <nb pings> <second host>" << endl;
		return 1;
	}

	try {
		// create 2 objects
		ping_pong_iface p1;
		ping_pong_iface p2(argv[3]);
		p1.set_next_one(p2.contact());
		p2.set_next_one(p1.contact());

		if(string(argv[1]) == "async") {
			// pop::mutex_iface l;
			pop::local_object<pop::mutex> l;
			pop::accesspoint contact = l.contact();
			std::thread th([&l] {
					l.run();
			});

			// objects ping each other asynchrounously
			p2.async_ping(atoi(argv[2]), contact);

			// note: we need to add a sleep here to allow all methods to complete
			l.object().wait();
			l.stop();
			th.join();
		} else if(string(argv[1]) == "async") {
			// objects ping each other synchrounously
			p2.sync_ping(atoi(argv[2]));
		} else {
			cerr << "Unknown invoker " << argv[1] << endl;
		}

	} catch (std::exception& e) {
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
