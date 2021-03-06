#!/usr/bin/env python3

##
## parser_brok.py
## ~~~~~~~~~~~~~~
##
## Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
## 
## Distributed under the Boost Software License, Version 1.0. (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
##

import parser

#--------------------------------------------------------------------------------

def write_head(fout, classname, filename_in):

	fout.write("""/* This file was generated automatically by the poplite parser */
#include "class/broker.hpp"
#include "%s"

""" % (filename_in))

#--------------------------------------------------------------------------------

def write_constr(m, constr_style):
	
	return 'broker_constructor_%s::create_binded_constructor<%s>()' % (constr_style, parser.list_args_types(m))

#--------------------------------------------------------------------------------

def write_meth(m, full_name, template_str):
	
	conc = 'conc'
	create = 'create_binded_method'
	if m.is_static_method():
		conc = 'static_conc'
		create = 'static_create_binded_method'
	elif m.is_const_method():
		conc = 'const_conc'
		create = 'const_create_binded_method'
	
	if parser.is_template_method(m):
		meths = []
		for t in parser.get_template_types(m):
			# TODO: Difficult case: if the template inherits from a different template or a non-template class. Not handled yet.
			meths.append('%s(&%s, &%s::%s%s)' % (create, conc, parser.get_full_name(m.lexical_parent) + template_str, m.spelling, t))
		return ',\n'.join(meths)
	else:
		# fout.write('std::bind(&remote::broker<%s>::%s<%s%s>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &%s::%s),\n'
				# % (full_name, conc, parser.get_full_name(m.result_type), parser.list_args_types(m, True), parser.get_full_name(m.lexical_parent), m.spelling))
		return '%s(&%s, &%s::%s)' % (create, conc, parser.get_full_name(m.lexical_parent) + template_str, m.spelling)

#--------------------------------------------------------------------------------

def write_broker(fout, class_node):

	fout.write("""
namespace pop
{
namespace remote
{
""")
	
	# implementation of static array of methods
	constr_style = parser.get_class_construction_style(class_node)
	templates_str = parser.get_template_types(class_node)
	for template_str in templates_str:
		full_name = parser.get_full_name(class_node) + template_str
		fout.write('template<> const std::vector<parallel_method<%s>> broker<%s, broker_constructor_%s<%s>>::methods_{\n'
			% (full_name, full_name, constr_style, full_name))

		meths = []
		for m in parser.find_methods(class_node)[0]:
			meths.append(write_meth(m, full_name, template_str))
		fout.write(',\n'.join(meths))

		fout.write('\n};\n')
		fout.write('template<> const std::vector<parallel_constructor_%s<%s>> broker_constructor_%s<%s>::constr_methods_{\n'
			% (constr_style, full_name, constr_style, full_name))
		meths = []
		for c in parser.find_constructors(class_node):
			meths.append(write_constr(c, constr_style))
		fout.write(',\n'.join(meths))

		fout.write('\n};\n')
	fout.write('}\n}\n')

def write_main(fout, class_node):
	
	# implementation of static array of methods
	templates_str = parser.get_template_types(class_node)
	constr_style = parser.get_class_construction_style(class_node)

	fout.write("""
#include "com/broker_combox.hpp"

template<typename T> inline void run_broker(const boost::asio::ip::tcp::resolver::query& _query) {
	pop::remote::broker<T, pop::remote::broker_constructor_%s<T>> brok;
	pop::broker_combox combox(brok, _query);
	combox.run();
}

int main(int _argc, char* _argv[])
{
	pop::system::instance(&_argc, _argv);

	try
	{
		// Check command line arguments.
		if (_argc < 4)
		{
			LOG(error) << "Usage: " << _argv[0] << " <object_type> <hostname of interface> <port of interface>";
			return -1;
		}
		boost::asio::ip::tcp::resolver::query query(_argv[2], _argv[3]);
""" % (constr_style))

	for t in templates_str:
		full_name = parser.get_full_name(class_node) + t
		fout.write("""
		if(strcmp(_argv[1], "%s") == 0) {
			run_broker<%s>(query);
			return 0;
		}
""" % (full_name, full_name))

	fout.write("""
		LOG(error) << "Object type " << _argv[1] << " not found in : " << _argv[0];
		return 1;
	}
	catch (std::exception& e)
	{
		LOG(error) << "Exception in object main: " << e.what();
		return 1;
	}
	catch (...)
	{
		LOG(error) << "Unknown exception in object main";
		return 1;
	}

	return 0;
}
""")


#--------------------------------------------------------------------------------

if __name__ == '__main__':
	print('this file is a Python library: try using popgen instead')
