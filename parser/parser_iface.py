#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import os
import parser

#--------------------------------------------------------------------------------

def write_head(fout, filename, mid_file):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_IFACE_H
#define _POP_%s_IFACE_H
#include "class/interface.hpp"
#include "%s"

namespace pop
{
""" % (filename.upper(), filename.upper(), os.path.basename(mid_file)))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("""
}
#endif
""")

#--------------------------------------------------------------------------------

def write_interface(fout, class_node):

	fout.write("""
class %s : public pop::interface
{
public:
""" % class_node.spelling)

	# Add a constructor from accesspoint for references to parallel objects
	fout.write("%s(pop::accesspoint _ap) : pop::interface(_ap) {}\n" % class_node.spelling)

	id = 0
	for c in parser.find_constructors(class_node):
		write_constr(fout, c, id)
		id += 1

	for m in parser.find_methods(class_node):
		write_meth(fout, m, id, class_node.spelling)
		id += 1

	fout.write("};\n")

#--------------------------------------------------------------------------------

def write_constr(fout, c, id):
	fout.write('%s(%s):pop::interface("main.%s", %s) {constr<%s>(%s_method_ids::%s%d %s);}\n' 
		% (c.spelling, parser.list_args(c), c.spelling, parser.get_allocation(c), parser.list_args1(c), c.spelling, c.spelling, id, parser.list_args2(c, True)))
#--------------------------------------------------------------------------------

def write_meth(fout, m, id, classname):
	fout.write('inline %s %s(%s) {' %(m.result_type.spelling, m.spelling, parser.list_args(m)) 
		+ '%s<%s%s>(broker::%s_method_ids::%s%s %s);}\n' % (parser.get_invoker(m), m.result_type.spelling, parser.list_args1(m, True), classname, m.spelling, id, parser.list_args2(m, True)))

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
