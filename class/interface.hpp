#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"

namespace pop{

using namespace std;

class interface
{
	public:
		interface(bufin& is, bufout& os) : ia(is), oa(os) {}

		template<typename ...Args> void call_sync(int x_method_id, std::tuple<Args...>& tup)
		{
			try
			{
				oa << x_method_id;
				oa << tup;
				sleep(1); // TODO

				ia >> tup;
			}
			catch(exception& e)
			{
				cout << "exception in call_sync: " << e.what() << endl;
			}
		}
	private:
		bufin&  ia;
		bufout& oa;
};



}

#endif
