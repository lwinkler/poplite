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

// this include should always be right before the parallel class declaration
#include "test_class.iface.hpp"

POP_CLASS TestClass
{
	public:
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		POP_ALLOCATION(pop::local_allocator())
		TestClass(std::string _s) {std::cout << "call constr with " << _s << std::endl;}

		void POP_SYNC SetValues(int _i1, int _i2, double _d, const std::string& _s)
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

		void SetTest(const test_struct1& _test1){test1_.a = _test1.a;}
		void GetTest(test_struct1& _test1){_test1.a = test1_.a;}
		// test_struct1 GetTest(){return test1_;}

		void SetGps(gps_position& _gps) { gps_ = _gps;}
		gps_position GetGps(){return gps_;}

		std::string GetStr() {return s_;}

		std::vector<int> GetVector1(){return vi_;}
		std::vector<gps_position> GetVector2(){return vgps_;}
		std::map<int, std::string> GetMap1(){return mstr_;}

	private:
		int i1_;
		int i2_;
		double d_;
		std::string s_;
		gps_position gps_;
		test_struct1 test1_;
		std::vector<int> vi_;
		std::vector<gps_position> vgps_;
		std::map<int, std::string> mstr_;
};

#endif
