


#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>
#include <fstream>

#include "class/broker.hpp"
// #include "class/interface.hpp"

using namespace std;
using namespace pop;

class gps_position
{
	private:
		friend class boost::serialization::access;
		// When the class Archive corresponds to an output archive, the
		// & operator is defined similar to <<.  Likewise, when the class Archive
		// is a type of input archive the & operator is defined similar to >>.
		template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
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
		TestClass(std::tuple<int>& _i) {}
		static TestClass* __constr(std::tuple<int>& _i){return new TestClass(_i);}
		void ChangeValues(std::tuple<int, int, double, string>& args)
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

		void ParMethod5(std::tuple<vector<int>>& args)
		{
			bufout oo(cout);
			oo<<"Invocation of the real method5 gps:"<<get<0>(args);
		}
		void SetValues(std::tuple<int, int, double, string>& args)
		{
			cout<<"Invocation of the real method"<<get<0>(args)<<get<1>(args)<<get<2>(args)<<get<3>(args)<<endl;
			i1  = get<0>(args);
			i2  = get<1>(args);
			d   = get<2>(args);
			str = get<3>(args);
		}
		void GetValues(std::tuple<int, int, double, string>& args)
		{
			cout<<"Invocation of the real method"<<get<0>(args)<<get<1>(args)<<get<2>(args)<<get<3>(args)<<endl;
			get<0>(args) = i1;
			get<1>(args) = i2;
			get<2>(args) = d;
			get<3>(args) = str;
		}


		// typedef void (broker<TestClass>::*pt_method_)(pop::bufin& ia, pop::bufout& oa);
		static const vector<remote::parallel_method<TestClass>>& parallel_methods()
		{
			static const vector<remote::parallel_method<TestClass>>meths
			{
				std::bind(&remote::broker<TestClass>::call_constr<int>,                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::__constr),
				std::bind(&remote::broker<TestClass>::call_simple<int,int,double,string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ChangeValues),
				std::bind(&remote::broker<TestClass>::call_simple<int,int>,               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod2),
				std::bind(&remote::broker<TestClass>::call_simple<>,                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod3),
				std::bind(&remote::broker<TestClass>::call_simple<gps_position>,          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod4),
				std::bind(&remote::broker<TestClass>::call_simple<vector<int>>,           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::ParMethod5),
				std::bind(&remote::broker<TestClass>::call_simple<int,int,double,string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::SetValues),
				std::bind(&remote::broker<TestClass>::call_simple<int,int,double,string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &TestClass::GetValues)
			};
			return meths;
		}

	private:
		int i1;
		int i2;
		double d;
		std::string str;

};

#endif
