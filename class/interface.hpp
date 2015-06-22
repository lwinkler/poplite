//
// interface.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"
#include "com/interface_combox.hpp"
#include "alloc/alloc.hpp"

namespace pop{


/// A template to allow the return of a variable, including null
template<typename R> class return_class
{
public:
	return_class(bufin& _ia){_ia >> r_;}
	R return_value(){return r_;}

private:
	R r_;
};

template<> class return_class<void>
{
public:
	return_class(bufin& /*_ia*/){}
	void return_value(){return;}
};

/// Interface is the local part used to communicate with a broker (remote) that contains the instanciation of the parallel object
class interface : private boost::noncopyable
{
	public:
		interface(const std::string& _executable, const pop::allocator& _allocator, bool _linkLife = true) :
			combox_(),
			linkLife_(_linkLife)
		{

			_allocator.allocate(_executable, combox_.endpoint());
			// Handle connection
			combox_.run();
		}

		interface(const pop::accesspoint& _contact) :
			combox_(),
			linkLife_(false)
		{
			// Send our endpoint
			combox_.send_my_contact(combox_.endpoint(), _contact);
			
			// Wait for the broker to call us back
			combox_.run();
		}

		~interface()
		{
			if(linkLife_)
				sync<void>(-1);
		}

		inline void destructor()
		{
			sync<void>(-1);
		}

		template<typename R, typename ...Args> R sync(int _method_id, Args& ...args)
		{
			try
			{
				LOG(debug) << "call sync "<< _method_id;
				std::tuple<Args&...> tup(std::forward_as_tuple(args...));
				std::stringstream oss;
				bool is_async = false;
				bufout oa(oss);
				oa << _method_id;
				oa << is_async;
				oa << tup;
				combox_.connec().sync_write_ss(oss);

				LOG(debug) << "sent to broker";

				combox_.connec().sync_read(); //TODO: try sync_read(tup)

				bufin ia(combox_.connec().input_stream());
				return_class<R> ret(ia); // TODO: also for async
				if(std::tuple_size<std::tuple<Args...>>::value)
					ia >> tup;

				LOG(debug) << "received answer from broker" << &combox_.connec();

				std::string ack;
				ia >> ack;
				LOG(debug) << "received ack=" << ack;
				if(ack != "ACK")
					throw std::runtime_error("did not receive ack");
				if(_method_id == -1) // TODO: use code
					combox_.connec().socket().close();

				return ret.return_value();
			}
			catch(std::exception& e)
			{
				LOG(error) << "exception in sync: " << e.what();
			}
			return R(); // TODO
		}


		template<typename R, typename ...Args> R async(int _method_id, Args& ...args) // TODO: avoid rewrite of method
		{
			try
			{
				LOG(debug) << "call sync "<< _method_id;
				std::tuple<Args&...> tup(std::forward_as_tuple(args...));
				std::stringstream oss;
				bool is_async = true;
				bufout oa(oss);
				oa << _method_id;
				oa << is_async;
				oa << tup;
				combox_.connec().sync_write_ss(oss);

				LOG(debug) << "sent to broker";

				combox_.connec().sync_read(); //TODO: try sync_read(tup)
				bufin ia(combox_.connec().input_stream());
				if(std::tuple_size<std::tuple<Args...>>::value)
					ia >> tup;

				// TODO: serialize R
				LOG(debug) << "received answer from broker" << &combox_.connec();

				std::string ack;
				ia >> ack;
				LOG(debug) << "received ack=" << ack;
				if(ack != "ACK")
					throw std::runtime_error("did not receive ack");
				if(_method_id == -1) // TODO: use code
					combox_.connec().socket().close();

			}
			catch(std::exception& e)
			{
				LOG(error) << "exception in sync: " << e.what();
			}
			return R(); // TODO
		}


		// inline const boost::asio::ip::tcp::endpoint endpoint() const {return combox_.endpoint();}
		inline const pop::accesspoint& contact(){return combox_.contact();}


	private:
		pop::interface_combox combox_;
		bool linkLife_;
};



}

#endif
