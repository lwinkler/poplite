#ifndef _MOTHER_PH
#define _MOTHER_PH

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "Mother.iface.hpp"
class Mother_iface;

POP_CLASS Mother {

public:

	Mother();

	virtual void Method1();
	virtual void callMethod(Mother_iface& c, int v);
	void SetVal(int v);
	int GetVal();

protected:
	int val;
};
#if 0 TODO clean

namespace boost {
namespace serialization {
	template<class Archive> inline void save_construct_data(Archive &ar, const Mother_iface *t, const unsigned int file_version) {
		std::cout << __LINE__ << std::endl;
		const pop::accesspoint& ap(t->contact());
		ar & ap;
	}
	template<class Archive> inline void load_construct_data(Archive &ar, Mother_iface *t, const unsigned int file_version) {
		std::cout << __LINE__ << std::endl;
		// retrieve data from archive required to construct new instance
		pop::accesspoint ap;
		ar & ap;
		// invoke inplace constructor to initialize instance of Mother_iface
		::new(t)Mother_iface(ap);
	}
	/*
	template<class Archive>void save(Archive& archive, const Mother_iface& label, const unsigned int version)
	{
		std::cout << __LINE__ << std::endl;
		;
	}

	template<class Archive>void load(Archive& archive, Mother_iface& f, const unsigned int version)
	{
		std::cout << __LINE__ << std::endl;
		;
	}
	*/
	template<class Archive> inline void serialize(Archive & ar, Mother_iface & t, const unsigned int file_version) {
		// split_free(ar, t, file_version); 
		std::cout << __LINE__ << std::endl;
	}
}
}

template<class archive>
inline void serialize(
		archive & ar, 
		mother_iface & t, 
		const unsigned long int file_version
		){
	assert(false);
}

namespace boost { namespace serialization {
	/*template<class Archive>
		inline void save_construct_data(Archive & ar, const Mother_iface * t, const unsigned long int file_version) {
			// save data required to construct instance
			ar << t->contact();
		}

	template<class Archive>
		inline void load_construct_data(Archive & ar, Mother_iface * t, const unsigned long int file_version) {
			// retrieve data from archive required to construct new instance
			pop::accesspoint ap;
			ar >> ap;
			std::cout << "AAAAA Received ap " << ap.host_name << std::endl;
			// invoke inplace constructor to initialize instance of Mother_iface
			::new(t)Mother_iface(ap);
		}
		*/
	template<class Archive>
		inline void save_construct_data(
				    Archive & ar, const Mother_iface * t, const unsigned long int file_version
				){
			    // save data required to construct instance
			assert(false);
			ar << t->contact();
		}
	// load data required for construction and invoke constructor in place
	template<class Archive>
		inline void load_construct_data(
				    Archive & ar, Mother_iface* t, const unsigned int file_version
				){
			    // default just uses the default constructor to initialize
			    // previously allocated memory. 
				pop::accesspoint ap;
				ar >> ap;
				std::cout << "AAAAA Received ap " << ap.host_name << std::endl;
			    ::new(t)Mother_iface(ap);
		}
} // namespace serialization
} // namespace boost



#endif

#endif
