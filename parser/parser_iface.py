#!/usr/bin/env python

##
## parser_iface.py
## ~~~~~~~~~~~~~~~
##
## Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
## 
## Distributed under the Boost Software License, Version 1.0. (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
##

import parser

#--------------------------------------------------------------------------------

def write_head(fout, filename, mid_file):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_IFACE_H
#define _POP_%s_IFACE_H
#include "class/interface.hpp"
#include "%s"
// #include "%s"

#include "alloc/alloc.hpp"
#include "alloc/local.hpp"
#include "alloc/manual.hpp"
#include "alloc/ssh.hpp"

""" % (parser.capitalize(filename), parser.capitalize(filename), mid_file, filename))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("""
#endif
""")

#--------------------------------------------------------------------------------

def write_interface(fout, class_node):
	parent_nodes = parser.get_parents(class_node, True, True)
	if len(parent_nodes) > 1:
		raise Exception('Class ' + class_node.spelling + ' cannot have more than one direct parent')
	parent_iface = parent_nodes[0].spelling + '_iface' if parent_nodes else "pop::interface"

	fout.write("""
class %s_iface : public %s
{
public:
""" % (class_node.spelling, parent_iface))

	# Add a constructor from accesspoint for references to parallel objects
	fout.write("%s_iface(pop::accesspoint _ap) : %s(_ap) {}\n" % (class_node.spelling, parent_iface))

	id = 0
	for c in parser.find_constructors(class_node):
		write_constr(fout, c, id, parent_iface)
		id += 1

	for m in parser.find_methods(class_node):
		write_meth(fout, m, id, class_node.spelling)
		id += 1

	fout.write("""
protected:
	// for inheritance
	%s_iface(const std::string& _executable, const pop::allocator& _allocator) : %s(_executable, _allocator){}
""" % (class_node.spelling, parent_iface))

	fout.write("};\n")

#--------------------------------------------------------------------------------

def write_constr(fout, c, id, parent_iface):
	# note: virtual inheritence is not handled
	parent_constr = parent_iface + '(_executable, _allocator)'
	fout.write('%s_iface(%sconst std::string& _executable = "%s.obj", const pop::allocator& _allocator = %s) : %s {sync<void%s>(%s_method_ids::%s%d%s);}\n' 
		% (c.spelling, parser.list_args(c, False, True), c.spelling, parser.get_allocation(c), parent_constr, parser.list_args1(c, True), c.spelling, c.spelling, id, parser.list_args2(c, True)))
#--------------------------------------------------------------------------------

def write_meth(fout, m, id, classname):
	fout.write('inline %s %s(%s) {' %(m.result_type.spelling, m.spelling, parser.list_args(m)) 
		+ 'return %s<%s%s>(%s_method_ids::%s%d%s);}\n' % (parser.get_invoker(m), m.result_type.spelling, parser.list_args1(m, True), classname, m.spelling, id, parser.list_args2(m, True)))

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"

