#!/usr/bin/env python

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
#ifndef _POP_%s_BROKER_H
#define _POP_%s_BROKER_H

#include "class/broker.hpp"
#include "%s"

namespace pop
{
namespace remote
{
""" % (parser.capitalize(classname), parser.capitalize(classname), filename_in))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("}\n}\n#endif\n")

#--------------------------------------------------------------------------------

def write_constr(fout, m, classname):
	
	fout.write("std::bind(&remote::broker<%s>::call_constr<%s>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),\n" % (classname, parser.list_args1(m)))

def write_meth(fout, m, classname):
	
	conc = 'conc'
	if m.is_static_method():
		conc = 'static_conc'
	elif m.is_const_method():
		conc = 'const_conc'
	if parser.is_template_method(m):
		for t in parser.get_template_types(m):
			fout.write("create_binded_method(&remote::broker<%s>::%s, &%s::%s%s),\n"
				% (classname, conc, parser.get_full_name(m.lexical_parent), m.spelling, t))
	else:
		fout.write("std::bind(&remote::broker<%s>::%s<%s%s>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &%s::%s),\n"
			% (classname, conc, parser.get_full_name(m.result_type), parser.list_args1(m, True), parser.get_full_name(m.lexical_parent), m.spelling))

#--------------------------------------------------------------------------------

def write_broker(fout, class_node):
	
	# implementation of static array of methods
	full_name = parser.get_full_name(class_node)
	fout.write("template<> const std::vector<remote::parallel_method<%s>> broker<%s>::methods_{\n"
		% (full_name, full_name))

	for m in parser.find_methods(class_node)[0]:
		write_meth(fout, m, full_name)

	for c in parser.find_constructors(class_node):
		write_constr(fout, c, full_name)

	fout.write("nullptr\n};\n")

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
