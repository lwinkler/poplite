#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import parser

#--------------------------------------------------------------------------------

def write_head(fout, filename, filename_in):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_BROKER_H
#define _POP_%s_BROKER_H

#include "class/broker.hpp"
#include "%s.hpp"

namespace pop
{
namespace remote
{
""" % (filename.upper(), filename.upper(), filename_in))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("}\n}\n#endif\n")

#--------------------------------------------------------------------------------

def write_constr(fout, m):
	
	fout.write("std::bind(&remote::broker<%s>::call_constr<std::string>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),\n" % m.spelling)

def write_meth(fout, m, classname):
	
	fout.write("std::bind(&remote::broker<%s>::conc<%s>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &%s::%s),\n"
		% (classname, parser.list_args1(m), classname, m.spelling))

#--------------------------------------------------------------------------------

def write_broker(fout, class_node):
	
	fout.write("template<> const std::vector<remote::parallel_method<%s>> broker<%s>::methods_\n{"
		% (class_node.spelling, class_node.spelling))

	for c in parser.find_constructors(class_node):
		write_constr(fout, c)

	for m in parser.find_methods(class_node):
		write_meth(fout, m, class_node.spelling)

	fout.write("nullptr\n};\n")

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
