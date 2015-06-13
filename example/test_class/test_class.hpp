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

#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>
#include <fstream>

#include "class/broker.hpp"
#include "class/interface.hpp"
#include "alloc/local.hpp"
#include "alloc/manual.hpp"
#include "alloc/ssh.hpp"

using namespace pop;

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
};

class TestClass
{
	public:
		//##POP_CONSTR(pop::local_allocator(), std::string)
		TestClass(std::string _s) {std::cout << "call constr with " << _s << std::endl;}

		//##POP_METH(sync, conc, void, SetValues, int, int, double, std::string)
		void SetValues(int _i1, int _i2, double _d, std::string _s)
		{
			i1_ = _i1;
			i2_ = _i2;
			d_  = _d;
			s_  = _s;
			std::cout << "SetValues " << i1_ << " " << i2_ << " " << d_ << " " << s_ << std::endl;
		}

		//##POP_METH(sync, conc, void, GetValues, int&, int&, double&, std::string&)
		void GetValues(int& _i1, int& _i2, double& _d, std::string& _s)
		{
			_i1 = i1_;
			_i2 = i2_;
			_d  = d_;
			_s  = s_;
		}

		//##POP_METH(sync, conc, std::string, GetStr)
		std::string GetStr() {return s_;}

	private:
		int i1_;
		int i2_;
		double d_;
		std::string s_;

};

#endif
