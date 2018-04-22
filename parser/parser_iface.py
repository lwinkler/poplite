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

def write_head(fout, classname, mid_file):

	fout.write("""/* This file was generated automatically by the poplite parser */
#ifndef _POP_%s_IFACE_H
#define _POP_%s_IFACE_H
#include "class/interface.hpp"
#include "%s"

#include "alloc/alloc.hpp"
#include "alloc/local.hpp"
#include "alloc/manual.hpp"
#include "alloc/ssh.hpp"

""" % (parser.capitalize(classname), parser.capitalize(classname), mid_file))

#--------------------------------------------------------------------------------

def write_foot(fout):

	fout.write("""
#endif
""")

#--------------------------------------------------------------------------------

def write_interface(fout, class_node):
	parent_nodes = parser.get_direct_parents(class_node, True, True)
	if len(parent_nodes) > 1:
		raise Exception('Parallel class ' + class_node.spelling + ' cannot have more than one direct parent')
	parent_ifaces = [node.spelling + "_iface" for node in  parent_nodes] if parent_nodes else ['pop::interface']

	# note: we must have virtual inheritence to have multiple inheritance
	fout.write("""
class %s_iface : %s
{
public:
""" % (class_node.spelling, ', '.join(['public ' + iface for iface in parent_ifaces])))

	# Needed for virtual inheritence
	# unused for the moment: each parclass can have only one parclass direct parent
	# if 'pop::interface' not in parent_ifaces:
		# parent_ifaces += ['pop::interface']

	# Add a constructor from accesspoint for references to parallel objects
	fout.write("%s_iface(pop::accesspoint _ap) : %s {}\n" % (class_node.spelling, ', '.join([iface + '(_ap)' for iface in parent_ifaces])))

	id = 0
	[methods, real_parents] = parser.find_methods(class_node)
	for m in methods:
		write_meth(fout, m, id, class_node.spelling, real_parents)
		id += 1

	# Constructors need to be inserted after methods to have matching ids in parent and child
	for c in parser.find_constructors(class_node):
		write_constr(fout, c, id, parent_ifaces)
		id += 1

	fout.write("""
protected:
	// for inheritance
	%s_iface(const std::string& _executable, const pop::allocator& _allocator, bool _ignore) : %s{}
""" % (class_node.spelling, ', '.join([iface + '(_executable, _allocator, _ignore)' for iface in parent_ifaces])))

	fout.write("};\n")

#--------------------------------------------------------------------------------

def write_constr(fout, c, id, parent_ifaces):
	# note: virtual inheritence is not handled
	parent_constr = ', '.join([iface + '(_executable, _allocator, false)' for iface in parent_ifaces])
	fout.write('%s_iface(%sconst std::string& _executable = "%s.obj", const pop::allocator& _allocator = %s) : %s {sync<void%s>(%s_method_ids::%s%d%s);}\n' 
		% (c.spelling, parser.list_args(c, False, True), c.spelling, parser.get_allocation(c), parent_constr, parser.list_args1(c, True), c.spelling, c.spelling, id, parser.list_args2(c, True)))
#--------------------------------------------------------------------------------

def write_meth(fout, m, id, classname, real_parents):
	if m.lexical_parent.spelling not in real_parents:
		fout.write('inline %s%s %s(%s) {' %('virtual ' if m.is_virtual_method() else '', m.result_type.spelling, m.spelling, parser.list_args(m)) 
			+ 'return %s<%s%s>(%s_method_ids::%s%d%s);}\n' % (parser.get_invoker(m), m.result_type.spelling, parser.list_args1(m, True), classname, m.spelling, id, parser.list_args2(m, True)))

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"

