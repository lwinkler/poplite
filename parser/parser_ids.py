#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import sys
import clang.cindex as cindex

cindex.Config.set_library_path("/usr/lib/llvm-3.5/lib")

import popparser as parser

#--------------------------------------------------------------------------------

def write_head(fout, filename):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_METH_IDS_H
#define _POP_%s_METH_IDS_H

namespace pop
{
	namespace broker
	{
""" % (filename.upper(), filename.upper()))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("""
	}
}
#endif
""")

#--------------------------------------------------------------------------------

def write_constr_ids(fout, class_node):
	
	fout.write("""
struct %s_constr_ids
{
""" % (class_node.spelling))

	constructors = parser.find_constructors(class_node)
	id = 0
	for m in constructors:
		fout.write("static const int %s%d = %d;\n" % (m.spelling, id, id))
		id += 1


	fout.write("};\n")

def write_meth_ids(fout, class_node):
	
	fout.write("""
struct %s_method_ids
{
""" % (class_node.spelling))

	id = 0
	methods = parser.find_methods(class_node)
	for m in methods:
		fout.write("static const int %s%d = %d;\n" % (m.spelling, id, id))
		id += 1


	fout.write("};\n")

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
