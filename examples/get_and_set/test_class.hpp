//
// test_class.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_CLASS_H
#define TEST_CLASS_H

// #include "class/interface.hpp"
#include "class/system.hpp"


struct test_struct1 : boost::noncopyable
{
	int a;
	test_struct1() : a(0){}	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & a;
	}
};


class gps_position
{
	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & degrees;
			ar & minutes;
			ar & seconds;
		}

	public:
		int degrees;
		int minutes;
		float seconds;
		gps_position(){};
		gps_position(int d, int m, float s) :
			degrees(d), minutes(m), seconds(s)
		{}
		bool operator == (const gps_position& _gps){return degrees == _gps.degrees && minutes == _gps.minutes && seconds == _gps.seconds;}
};

class pop_parallel TestClass
{
	public:
		pop_allocation(pop::local_allocator())
		TestClass(std::string _s) {std::cout << "call constr with " << _s << std::endl;}

		void pop_sync SetValues(int _i1, int _i2, double _d, std::string _s)
		{
			i1_ = _i1;
			i2_ = _i2;
			d_  = _d;
			s_  = _s;
			std::cout << "SetValues " << i1_ << " " << i2_ << " " << d_ << " " << s_ << std::endl;
		}

		int GetValue()
		{
			return 333;
		}

		void GetValues(int& _i1, int& _i2, double& _d, std::string& _s)
		{
			_i1 = i1_;
			_i2 = i2_;
			_d  = d_;
			_s  = s_;
		}

		void SetTest(test_struct1& _test1){test1_.a = _test1.a;}
		// test_struct1 GetTest(){return test1_;}

		void SetGps(gps_position& _gps) { gps_ = _gps;}
		gps_position GetGps(){return gps_;}

		std::string GetStr() {return "s_";}

	private:
		int i1_;
		int i2_;
		double d_;
		std::string s_;
		gps_position gps_;
		test_struct1 test1_;
};

#endif
