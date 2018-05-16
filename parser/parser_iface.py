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

	definitions = []
	write_meth_ids(fout, class_node, definitions)

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
		if parser.get_full_name(m.lexical_parent) in real_parents:
			continue
		if parser.is_template_method(m):
			id = write_template_meth(fout, m, id)
		else:
			write_meth(fout, m, id)
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

	# TODO: Maybe we can avoid this
	if definitions:
		fout.write('#define POP_SPECIFICATIONS_%s \\\n%s\n' % (classname, '\\\n'.join(definitions)))

#--------------------------------------------------------------------------------

def write_constr(fout, c, id, parent_ifaces):
	# note: virtual inheritence is not handled
	parent_constr = ', '.join([iface + '(_executable, _allocator, false)' for iface in parent_ifaces])
	objfile = parser.get_full_name(c.lexical_parent).replace('::', '.')
	fout.write('%s_iface(%sconst std::string& _executable = "%s.obj", const pop::allocator& _allocator = %s) : %s {sync<void%s>(method_ids::%s%d%s);}\n' 
		% (c.spelling, parser.list_args(c, False, True), objfile, parser.get_allocation(c), parent_constr, parser.list_args1(c, True), c.spelling, id, parser.list_args2(c, True)))
#--------------------------------------------------------------------------------

def write_meth(fout, m, id):
	virtual  = 'virtual ' if m.is_virtual_method() else ''
	fout.write('inline %s%s %s(%s) {' %(virtual, m.result_type.spelling, m.spelling, parser.list_args(m)) 
		+ 'return %s<%s%s>(method_ids::%s%d%s);}\n' % (parser.get_invoker(m), m.result_type.spelling, parser.list_args1(m, True), m.spelling, id, parser.list_args2(m, True)))

def write_template_meth(fout, m, id):
	ttypes = parser.get_template_types(m)
	virtual = 'virtual ' if m.is_virtual_method() else ''
	ttparams = parser.get_template_type_parameters(m)
	ttparams1 = '<%s>' % (', '.join(ttparams))
	ttparams2 = '<%s>' % (', '.join(('typename ' + t) for t in ttparams))
	invoker = parser.get_template_invoker(m)
	print ttparams1
	print ttparams2
	fout.write('template%s inline %s%s %s(%s) {' %(ttparams2, virtual, m.result_type.spelling, m.spelling, parser.list_args(m))
			+ 'return %s<%s%s>(method_ids::%s%d%s::value%s);}\n' % (invoker, m.result_type.spelling, parser.list_args1(m, True), m.spelling, id, ttparams1, parser.list_args2(m, True)))
	return id + len(ttypes)

#--------------------------------------------------------------------------------
def write_meth_ids(fout, class_node, definitions):
	fout.write("""
struct method_ids
{
""")

	id = 0
	methods = parser.find_methods(class_node)[0]
	for m in methods:
		if parser.is_template_method(m):
			id0 = id
			fout.write('template<class T> struct %s%d{static const int value;};\n' % (m.spelling, id0))
			for t in parser.get_template_types(m):
				definitions.append('template<> const int %s_iface::method_ids::%s%d%s::value = %d;' % (parser.get_full_name(class_node), m.spelling, id0, t, id))
				id += 1
		else:
			fout.write('static const int %s%d = %d;\n' % (m.spelling, id, id))
			id += 1

	constructors = parser.find_constructors(class_node)
	for m in constructors:
		fout.write('static const int %s%d = %d;\n' % (m.spelling, id, id))
		id += 1

	fout.write('};\n')

#--------------------------------------------------------------------------------

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"

