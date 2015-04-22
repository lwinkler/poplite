#include "test_class.hpp"
#include "class/interface.hpp"


int main()
{
	stringstream ofs;// std::ifstream ifs("out");
	stringstream ifs;// std::ofstream ofs("in");
	bufout oa(ofs);
	bufin  ia(ifs);

	pop::interface iface(ia,oa);

	cout<<__LINE__<<endl;
	tuple<int,int,double,string> tup0(345,22,65.898,"blabla");
	cout<<__LINE__<<endl;
	iface.call_sync<int,int,double,string>(0, tup0);
	cout<<__LINE__<<endl;

	// tuple<int,int> tup2(345,22);
	// iface.call_sync<int,int>(1, tup2);

	// tuple<> tup3;
	// iface.call_sync<>(2, tup3);

	// tuple<gps_position> tup4(gps_position(44,44,5.5));
	// iface.call_sync<gps_position>(3, tup4);

	
	return 0;	
}
