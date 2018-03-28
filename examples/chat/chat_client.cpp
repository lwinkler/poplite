
using namespace std;

#include "chat_client.hpp"

void chat_client::send_all(const string& _text)
{
	LOG(debug) << "send to " << connected_clients_.size() << " clients";
	for(auto& contact : connected_clients_)
	{
		contact.print(_text);
	}
}

void chat_client::print(const string& _text)
{
	cout << ">> " << _text << endl;
}

void chat_client::add_contact(const pop::accesspoint& _ap)
{
	try {
		connected_clients_.emplace_back(_ap);
	} catch(exception& e) {
		cerr << "Exception while adding contact " << e.what() << endl;
	}
}
