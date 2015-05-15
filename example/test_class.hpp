


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

using namespace pop;

class gps_position
{
	private:
		friend class boost::serialization::access;
		// When the class Archive corresponds to an output archive, the
		// & operator is defined similar to <<.  Likewise, when the class Archive
		// is a type of input archive the & operator is defined similar to >>.
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
		TestClass(int _i) {std::cout << "call constr with " << _i << std::endl;}
		//##POP_CONSTR(call_sync, call_simple, pop::local_allocator(), int)
		static TestClass* __constr(int _i){return new TestClass(_i);}
		/*
		void ChangeValues(std::tuple<int, int, double, std::string>& args)
		{
			cout<<"Invocation of the real method"<<endl;
			get<0>(args) = 6354;
			get<1>(args) = 909;
			get<2>(args) = 6.66;
			get<3>(args) = "something new";
		}
		void ParMethod2(std::tuple<int, int>& args)
		{
			cout<<"Invocation of the real method1"<<get<0>(args)<<get<1>(args)<<endl;
		}

		void ParMethod3(std::tuple<>& args)
		{
			cout<<"Invocation of the real method3"<<endl;
		}

		void ParMethod4(std::tuple<gps_position>& args)
		{
			bufout oo(cout);
			oo<<"Invocation of the real method4 gps:"<<get<0>(args);
		}

		void ParMethod5(std::tuple<std::vector<int>>& args)
		{
			bufout oo(cout);
			oo<<"Invocation of the real method5 gps:"<<get<0>(args);
		}
		*/
		//##POP_METH(call_sync, call_simple, void, SetValues, int, int, double, std::string)
		void SetValues(int _i1, int _i2, double _d, std::string _s)
		{
			i1_ = _i1;
			i2_ = _i2;
			d_  = _d;
			s_  = _s;
			std::cout << "SetValues " << i1_ << " " << i2_ << " " << d_ << " " << s_ << std::endl;
		}
		//##POP_METH(call_sync, call_simple, void, GetValues, int&, int&, double&, std::string&)
		void GetValues(int& _i1, int& _i2, double& _d, std::string& _s)
		{
			_i1 = i1_;
			_i2 = i2_;
			_d  = d_;
			_s  = s_;
		}

		//##POP_METH(call_sync, call_simple, std::string, GetStr)
		std::string GetStr() {return s_;}


		// typedef void (broker<TestClass>::*pt_method_)(pop::bufin& ia, pop::bufout& oa);
		/*
		static const std::vector<remote::parallel_method<TestClass>>& parallel_methods()
		{
			static const std::vector<remote::parallel_method<TestClass>>meths
			{
				std::bind(&remote::broker<TestClass>::call_constr<int>,                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::__constr),
				// std::bind(&remote::broker<TestClass>::call_simple<int,int,double,std::string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ChangeValues),
				// std::bind(&remote::broker<TestClass>::call_simple<int,int>,               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod2),
				// std::bind(&remote::broker<TestClass>::call_simple<>,                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod3),
				// std::bind(&remote::broker<TestClass>::call_simple<gps_position>,          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod4),
				// std::bind(&remote::broker<TestClass>::call_simple<std::vector<int>>,           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod5),
				std::bind(&remote::broker<TestClass>::call_simple<void, int,int,double,std::string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::SetValues),
				std::bind(&remote::broker<TestClass>::call_simple<void, int&,int&,double&,std::string&>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::GetValues),
				std::bind(&remote::broker<TestClass>::call_simple<std::string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::GetStr)
			};
			return meths;
		}
		*/

	private:
		int i1_;
		int i2_;
		double d_;
		std::string s_;

};


/*
class TestClassInterface : public pop::interface
{
	public:
	TestClassInterface(int i1):pop::interface(pop::local_allocator()){call_sync<int>(0, i1);}
	inline void SetValues(int _i1, int _i2, double _d, std::string _s)    {call_sync<void, int , int , double, std::string>(1, _i1, _i2, _d, _s);} // TODO : handle const
	inline void GetValues(int& _i1, int& _i2, double& _d, std::string& _s){call_sync<void, int&, int&, double, std::string>(2, _i1, _i2, _d, _s);}
	inline std::string GetStr(){return call_sync<std::string>(3);}
};
*/

#endif
