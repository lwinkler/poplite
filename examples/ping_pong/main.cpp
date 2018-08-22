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
// TODO: See if we can construct the broker with an object and destroy the thread in destructor
// TODO: move to its own file
namespace pop {
template<typename T> class local_object final {
	public:
		local_object() : combox_(brok_) {
			brok_.ptr_obj().reset(new T());
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
			return combox_.contact();
		}
	private:
		pop::remote::broker<T> brok_;
		pop::broker_combox<T> combox_;
};
} // namespace pop


// Call our interface
void play_ping_pong(ping_pong_iface& _p, const string& _invoker, unsigned int _nb_calls) {
	if(_invoker == "async") {
		pop::mutex_iface l;
		// objects ping each other asynchrounously
		_p.async_ping(_nb_calls, l.contact());
		// note: we need to add a wait here to allow all methods to complete
		l.wait();
	} else if(_invoker == "async2") {
		pop::local_object<pop::mutex> l;
		pop::accesspoint contact = l.contact();
		std::thread th([&l] {
			l.run();
		});

		// objects ping each other asynchrounously
		_p.async_ping(_nb_calls, contact);

		// note: we need to add a wait here to allow all methods to complete
		l.object().wait();
		l.stop();
		th.join();
	} else if(_invoker == "sync") {
		// objects ping each other synchrounously
		_p.sync_ping(_nb_calls);
	} else {
		cerr << "Unknown invoker " << _invoker << endl;
	}
}

int main(int argc, char* argv[]) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);


	try {
		if(argc == 3) {
			// create 2 objects
			ping_pong_iface p1;
			ping_pong_iface p2;
			p1.set_next_one(p2.contact());
			p2.set_next_one(p1.contact());

			play_ping_pong(p1, argv[1], boost::lexical_cast<unsigned int>(argv[2]));
		} else if(argc == 4) {
			// create 2 objects
			ping_pong_iface p1;
			ping_pong_iface p2(argv[3]);
			p1.set_next_one(p2.contact());
			p2.set_next_one(p1.contact());

			play_ping_pong(p1, argv[1], boost::lexical_cast<unsigned int>(argv[2]));
		} else {
			cerr << "usage: " << argv[0] << " <sync or async> <nb pings> <second host>" << endl;
			return 1;
		}

	} catch (std::exception& e) {
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
