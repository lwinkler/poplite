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

def write_head(fout, classname):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_IFACE_H
#define _POP_%s_IFACE_H
#include "class/interface.hpp"

""" % (parser.capitalize(classname), parser.capitalize(classname)))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("""
#endif
""")

#--------------------------------------------------------------------------------

def write_interface(fout, class_node):
	parent_nodes = parser.get_direct_parents(class_node, True, True)
	classname = class_node.spelling
	if len(parent_nodes) > 1:
		raise Exception('Parallel class ' + classname + ' cannot have more than one direct parent')
	parent_ifaces = [parser.get_full_name(node) + '_iface' for node in  parent_nodes] if parent_nodes else ['pop::interface']

	# note: we must have virtual inheritence to have multiple inheritance
	fout.write("""
class %s_iface : %s
{
private:""" % (classname, ', '.join(['public ' + iface for iface in parent_ifaces])))

	write_meth_ids(fout, class_node) 

	fout.write('public:\n')

	# Needed for virtual inheritence
	# unused for the moment: each parclass can have only one parclass direct parent
	# if 'pop::interface' not in parent_ifaces:
		# parent_ifaces += ['pop::interface']

	# Add a constructor from accesspoint for references to parallel objects
	fout.write("%s_iface(pop::accesspoint _ap) : %s {}\n" % (classname, ', '.join([iface + '(_ap)' for iface in parent_ifaces])))

	id = 0
	[methods, real_parents] = parser.find_methods(class_node)
	for m in methods:
		write_meth(fout, m, id, real_parents)
		id += 1

	# Constructors need to be inserted after methods to have matching ids in parent and child
	for c in parser.find_constructors(class_node):
		write_constr(fout, c, id, parent_ifaces)
		id += 1

	fout.write("""
protected:
	// for inheritance
	%s_iface(const std::string& _executable, const pop::allocator& _allocator, bool _ignore) : %s{}
""" % (classname, ', '.join([iface + '(_executable, _allocator, _ignore)' for iface in parent_ifaces])))

	fout.write("};\n")

#--------------------------------------------------------------------------------

def write_constr(fout, c, id, parent_ifaces):
	# note: virtual inheritence is not handled
	parent_constr = ', '.join([iface + '(_executable, _allocator, false)' for iface in parent_ifaces])
	fout.write('%s_iface(%sconst std::string& _executable = "%s.obj", const pop::allocator& _allocator = %s) : %s {sync<void%s>(method_ids::%s%d%s);}\n' 
		% (c.spelling, parser.list_args(c, False, True), c.spelling, parser.get_allocation(c), parent_constr, parser.list_args1(c, True), c.spelling, id, parser.list_args2(c, True)))
#--------------------------------------------------------------------------------

def write_meth(fout, m, id, real_parents):
	if parser.get_full_name(m.lexical_parent) not in real_parents:
		fout.write('inline %s%s %s(%s) {' %('virtual ' if m.is_virtual_method() else '', m.result_type.spelling, m.spelling, parser.list_args(m)) 
			+ 'return %s<%s%s>(method_ids::%s%d%s);}\n' % (parser.get_invoker(m), m.result_type.spelling, parser.list_args1(m, True), m.spelling, id, parser.list_args2(m, True)))

#--------------------------------------------------------------------------------
def write_meth_ids(fout, class_node):
	fout.write("""
struct method_ids
{
""")

	id = 0
	methods = parser.find_methods(class_node)[0]
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

