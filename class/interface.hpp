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

// #include <type_traits>

#include "com/serialize.hpp"
#include "com/exception.hpp"
#include "com/interface_combox.hpp"
#include "alloc/alloc.hpp"

namespace pop {


/// A template to allow the return of a variable, including null
template<typename R> class return_class
{
public:
	return_class(bufin& _ia){_ia >> r_;}
	R return_value(){return r_;}

private:
	R r_;
};

/// Specification for void
template<> class return_class<void>
{
public:
	return_class(bufin& /*_ia*/){}
	void return_value(){return;}
};

/// Interface is the local part used to communicate with a broker (remote) that contains the instantiation of the parallel object
class interface
{
	public:
		interface(const std::string& _executable, const pop::allocator& _allocator) :
			combox_(),
			link_life_(true)
		{
			_allocator.allocate(_executable, combox_.callback());
			// Handle connection
			combox_.run();
		}
		// This method only exists to facilitate inheritence (parser)
		inline interface(const std::string& _executable, const pop::allocator& _allocator, bool _ignore) : interface(_executable, _allocator) {}

		interface(const pop::accesspoint& _contact) :
			combox_(),
			link_life_(false)
		{
			// Send our endpoint
			combox_.send_my_contact(_contact);
			
			// Wait for the broker to call us back
			combox_.run();
		}

		interface(interface&&) = default; // TODO: Move is not working due to parent classes. See if boost fixes this
		interface& operator=(interface&&) = default;

		// you shall not copy
		interface(const interface&) = delete;
		interface& operator=(const interface&) = delete;

		
		virtual ~interface()
		{
			try {
				LOG(debug) << "Destroy interface";
				// Close our current socket
				sync<void>(link_life_ ? method_id::DESTROY : method_id::DISCONNECT);

				if(link_life_)
				{
					// Close the service of the remote object
					combox_.close_service();
				}
			} catch(std::exception& exc) {
				LOG(info) << "Cannot " << (link_life_ ? "close" : "disconnect") << " broker service remotely: " << exc.what();
			}
		}

		template<typename R, typename ...Args> R sync(int _method_id, Args& ...args)
		{
			pop::exception exc;
			try
			{
				LOG(debug) << "call sync "<< _method_id;
				std::tuple<Args...> tup(std::forward_as_tuple(args...));
				std::stringstream oss;
				static const bool is_async = false;
				bufout oa(oss);
				oa << _method_id;
				oa << is_async;
				oa << tup;
				combox_.connec().sync_write_ss(oss);

				LOG(debug) << "sent to broker";

				if(_method_id == method_id::DISCONNECT || _method_id == method_id::DESTROY)
				{
					combox_.connec().socket().close();
					return R();
				}

				combox_.connec().sync_read();

				bufin ia(combox_.connec().input_stream());
				return_class<R> ret(ia);
				serialize_out<bufin, Args... >(ia, tup);

				LOG(debug) << "received answer from broker";

				// exception is our ack signal
				ia >> exc;
				if(exc.empty()) {
					return ret.return_value();
				}
			}
			catch(std::exception& exc)
			{
				LOG(error) << "Interface: Exception in sync method: " << exc.what();
				throw exc;
			}
			catch(...)
			{
				LOG(error) << "Interface: Unknown exception in sync method";
				throw;
			}
			throw exc;
			// return R();
		}


		template<typename R, typename ...Args> void async(int _method_id, Args& ...args)
		{
			// note:unused 
			// static_assert(std::is_void<R>::value, "Return type of async methods must be void");
			  
			try
			{
				LOG(debug) << "call async "<< _method_id;
				std::tuple<Args...> tup(std::forward_as_tuple(args...));
				std::stringstream oss;
				bool is_async = true;
				bufout oa(oss);
				oa << _method_id;
				oa << is_async;
				oa << tup;
				combox_.connec().sync_write_ss(oss);

				LOG(debug) << "sent to broker";

				if(_method_id == method_id::DISCONNECT || _method_id == method_id::DESTROY)
				{
					combox_.connec().socket().close();
					return;
				}

				combox_.connec().sync_read();

				bufin ia(combox_.connec().input_stream());
				LOG(debug) << "received answer from broker";

				pop::exception exc;
				ia >> exc;
				assert(exc.empty());

				if(_method_id == method_id::DISCONNECT || _method_id == method_id::DESTROY)
					combox_.connec().socket().close();

				return;
			}
			catch(std::exception& e)
			{
				LOG(error) << "Exception in async: " << e.what();
			}
			catch(...)
			{
				LOG(error) << "Unknown exception in async";
			}
		}

		// inline const boost::asio::ip::tcp::endpoint endpoint() const {return combox_.endpoint();}
		inline const pop::accesspoint& contact() const {return combox_.contact();}

	private:
		pop::interface_combox combox_;
		bool link_life_;
};
}

/*
namespace boost {
namespace serialization {
	template<class Archive> inline void save_construct_data(Archive &ar, const pop::interface *t, const unsigned int file_version) {
		ar << t->contact();
	}
	template<class Archive> inline void load_construct_data(Archive &ar, pop::interface *t, const unsigned int file_version) {
		// retrieve data from archive required to construct new instance
		pop::accesspoint ap;
		ar >> ap;
		// invoke inplace constructor to initialize instance of pop::interface
		::new(t)pop::interface(ap);
	}
	template<class Archive>void save(Archive& archive, const pop::interface& label, const unsigned int version)
	{
		;
	}

	template<class Archive>void load(Archive& archive, pop::interface& f, const unsigned int version)
	{
		;
	}
	template<class Archive> inline void serialize(Archive & ar, pop::interface & t, const unsigned int file_version) {
		split_free(ar, t, file_version); 
	}
}
}
*/

/* TODO clean
namespace boost { namespace serialization {
	template<class Archive>
		inline void save_construct_data(
				Archive & ar, const pop::interface * t, const unsigned long int file_version
				){
			// save data required to construct instance
			ar << t->contact();
		}

	template<class Archive>
		inline void load_construct_data(
				Archive & ar, pop::interface * t, const unsigned long int file_version
				){
			// retrieve data from archive required to construct new instance
			pop::accesspoint ap;
			ar >> ap;
			// invoke inplace constructor to initialize instance of pop::interface
			::new(t)pop::interface(ap);
		}
} // namespace serialization
} // namespace boost
*/

#endif
