#!/usr/bin/env python

##
## parser_ids.py
## ~~~~~~~~~~~~~
##
## Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
## 
## Distributed under the Boost Software License, Version 1.0. (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
##

""" Usage: call with <filename> <typename>
"""

import clang.cindex as cindex
import parser

#--------------------------------------------------------------------------------

def write_head(fout, filename):
	""" Write the header or the file """

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_METH_IDS_H
#define _POP_%s_METH_IDS_H

""" % (parser.capitalize(filename), parser.capitalize(filename)))

#--------------------------------------------------------------------------------

def write_foot(fout):
	""" Write the foot or the file """

	fout.write("""
#endif
""")

#--------------------------------------------------------------------------------

def write_meth_ids(fout, class_node):
	# TODO: use enum class direction: unsigned short{ LEFT, RIGHT }; // scoped
	
	fout.write("""
struct %s_method_ids
{
""" % (class_node.spelling))

	id = 0
	methods = parser.find_methods(class_node)
	for m in methods:
		fout.write("static const int %s%d = %d;\n" % (m.spelling, id, id))
		id += 1

	constructors = parser.find_constructors(class_node)
	for m in constructors:
		fout.write("static const int %s%d = %d;\n" % (m.spelling, id, id))
		id += 1

	fout.write("};\n")

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
