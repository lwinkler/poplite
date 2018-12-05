#!/usr/bin/env python

##
## parser.py
## ~~~~~~~~~
##
## Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
## 
## Distributed under the Boost Software License, Version 1.0. (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
##

""" Usage: call with <filename> <typename>
"""

import sys
import os
import clang.cindex as cindex
from subprocess import call
from pprint import pprint

cindex.Config.set_library_path('/usr/lib/llvm-6.0/lib')

def describe_node(node, full = False):
	""" Describe a node. For debug purposes
	"""
	try:
		print 'node %s (%s) %s' % (node.spelling, node.displayname, node.kind)
	except:
		print 'node'
	print node

	if full:
		pprint(node.__dict__)
		pprint(node._tu.__dict__)
		pprint(node._tu.index.__dict__)
		pprint(node._tu.index.obj.__dict__)
		print dir(node)

	# Recurse for children of this node
	def descr(node, tabs):
		for c in node.get_children():
			print '%s- %s (%s): %s %s' % (tabs, c.spelling, c.displayname, c.kind, c.data)
			if full:
				print dir(c)
				pprint(c._tu.index.obj.__dict__)
			descr(c, tabs + '\t')
	descr(node, '\t')

def init_tu(src, argv):
	""" Initialize a translation unit with the clang parser
	"""
	index = cindex.Index.create()
	return index.parse(src, ['-D_POP_PARSER_', '-x', 'c++', '-std=c++17'] + argv)

def capitalize(name):
	""" Return a capitalized version of name for use in ifndef/define
	"""
	return name.replace(':', '_') # .upper()

def convert_to_classname(name):
	return name.replace('::', '/')

def convert_to_objname(name):
	return name.replace('::', '.')

def print_ast(node, indent = 0):
	""" Print the content of source tree
	"""
	print '\t' * indent + 'node %s %s %s [line=%s, col=%s]' % (node.get_definition(), node.spelling, node.kind, node.location.line, node.location.column)
	for c in node.get_children():
		print_ast(c, indent + 1)

def get_full_name(node):
	""" Return the full name of the node with namespaces
	"""
	ns = ''
	parent = node
	while hasattr(parent, 'lexical_parent'):
		parent = parent.lexical_parent;
		if parent is None:
			break
		if parent.kind != cindex.CursorKind.TRANSLATION_UNIT and parent.spelling:
			# print parent.kind
			ns = parent.spelling + '::' + ns

	return ns + node.spelling

def find_parallel_classes(node, classnames):
	""" Find all classes with annotation "pop_parallel"
	"""

	found = []

	# print 'node %s %s %s [line=%s, col=%s]' % (node.get_definition(), node.spelling, node.kind, node.location.line, node.location.column)
	if node.kind in (cindex.CursorKind.CLASS_DECL, cindex.CursorKind.CLASS_TEMPLATE) and (classnames is None or get_full_name(node) in classnames):
		# print 'Found parallel class %s [line=%s, col=%s]' % (node.spelling, node.location.line, node.location.column)
		found.append(node)

	# Recurse for children of this node
	for c in node.get_children():
		found += find_parallel_classes(c, classnames)
	
	return found

def find_methods(class_node):
	""" Find all public methods in parallel class
	"""
	meths = []
	real_parents = []
	find_methods1(class_node, meths, real_parents)
	return [meths, real_parents]

def find_methods1(class_node, meths, real_parents):
	""" Find all public methods in parallel class (mapped by signature)
	"""

	# print 'class %s %s %s [line=%s, col=%s]' % (class_node.get_definition(), class_node.spelling, class_node.kind, class_node.location.line, class_node.location.column)
	real_parent = ''
	if real_parents is not None:
		par_parents = get_direct_parents(class_node, True, True)
		if len(par_parents) > 1:
			print 'Warning: parallel class %s has more than one parallel class as parent. The interface will only be castable into the first parent' % class_node.spelling
		if len(par_parents) >= 1:
			find_methods1(par_parents[0], meths, real_parents)
			real_parent = get_full_name(par_parents[0].get_definition())
			real_parents.append(real_parent)
		
	for parent in get_direct_parents(class_node, False, True):
		if get_full_name(parent) != real_parent:
			find_methods1(parent, meths, None)

	# Recurse for children of this node
	for c in class_node.get_children():
		# describe_node(c, True)
		if (c.kind == cindex.CursorKind.CXX_METHOD or c.kind == cindex.CursorKind.FUNCTION_TEMPLATE) and c.access_specifier == cindex.AccessSpecifier.PUBLIC:
			# print 'Found parallel method %s::%s [line=%s, col=%s] access=%s static=%s virtual=%s' % (class_node.spelling, c.displayname, c.location.line, c.location.column, c.access_specifier, c.is_static_method(), c.is_virtual_method())
			found = False
			for meth in meths:
				if meth.displayname == c.displayname and meth.is_virtual_method():
					# print 'Replace method %s' % meth.displayname
					# meth = c
					found = True
					break
			if not found:
				meths.append(c)

def find_constructors(class_node):
	""" Find all public methods in parallel class
	"""

	found = []

	# print 'class %s %s %s [line=%s, col=%s]' % (class_node.get_definition(), class_node.spelling, class_node.kind, class_node.location.line, class_node.location.column)


	# Recurse for children of this node
	for c in class_node.get_children():
		if c.kind == cindex.CursorKind.CONSTRUCTOR and c.access_specifier == cindex.AccessSpecifier.PUBLIC:
			# print 'Found constructor %s [line=%s, col=%s]' % (c.spelling, c.location.line, c.location.column)
			found.append(c)
	
	return found

def find_arguments(meth_node):
	""" Find all arguments in method
	"""

	found = []

	# print 'class %s %s %s [line=%s, col=%s]' % (meth_node.get_definition(), meth_node.spelling, meth_node.kind, meth_node.location.line, meth_node.location.column)


	# Recurse for children of this node
	for c in meth_node.get_children():
		if c.kind == cindex.CursorKind.PARM_DECL:
			# print 'Found param %s %s [line=%s, col=%s]' % (c.type.spelling, c.spelling, c.location.line, c.location.column)
			found.append(c)
	
	return found

def get_invoker(meth_node):
	""" Return the method invoker (sync, async)
	"""
	# print 'node %s %s %s [line=%s, col=%s]' % (meth_node.get_definition(), meth_node.spelling, meth_node.kind, meth_node.location.line, meth_node.location.column)

	for c in meth_node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR:
			if c.spelling.startswith('pop_invoker:'):
				return c.spelling[len('pop_invoker:'):]

	print '%s:%d:0 : warning: No invoker (sync or async) for method %s' % (meth_node.location.file, meth_node.location.line, meth_node.spelling)
	return 'sync' # our default

def get_allocation(constr_node):
	""" Return the allocation
	"""
	# print 'node %s %s %s [line=%s, col=%s]' % (constr_node.get_definition(), constr_node.spelling, constr_node.kind, constr_node.location.line, constr_node.location.column)

	for c in constr_node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR:
			if c.spelling.startswith('pop_allocation:'):
				return c.spelling[len('pop_allocation:'):]

	return 'pop::local_allocator()' # our default

def is_parallel(node):
	""" Return true if a node has parallel class annotation
	"""
	# describe_node(node)
	for c in node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR and c.spelling == 'pop_parallel':
			return True
	return False

def is_template_method(meth):
	""" Check if a method is a template """
	return  meth.kind == cindex.CursorKind.FUNCTION_TEMPLATE

def is_template_class(node):
	""" Check if a class is a template """
	return  node.kind == cindex.CursorKind.CLASS_TEMPLATE

def get_template_types(node):
	""" Return the template types of a template method (represent all usable instances of the template method) """
	if node.kind == cindex.CursorKind.CLASS_DECL:
		return ['']

	# for c1 in node.lexical_parent.get_children():
		# Search other attribute with specific name (since annotations are not valid on templates).
		# if c1.spelling == 'template_types_of_' + node.spelling:
	for c2 in node.get_children():
		if c2.kind == cindex.CursorKind.ANNOTATE_ATTR:
			if c2.spelling.startswith('pop_template_types:'):
				return c2.spelling[len('pop_template_types:'):].split(';')
	raise Exception('Did not find template types for template method or class %s. Use POP_TEMPLATE_TYPES(<type1>;<type2>)' % (node.spelling))

# Note: only used to fix a bug in clang 5 and earlier
def get_template_invoker(meth):
	""" Return the invoker of a template method """
	for c1 in meth.lexical_parent.get_children():
		if c1.spelling == 'template_types_of_' + meth.spelling:
			for c2 in c1.get_children():
				if c2.kind == cindex.CursorKind.ANNOTATE_ATTR:
					if c2.spelling.startswith('pop_invoker:'):
						return c2.spelling[len('pop_invoker:'):]
	return None

def get_template_type_parameters(meth):
	""" Return template type parameters, e.g. <T> """
	ttp = []
	for c in meth.get_children():
		if c.kind == cindex.CursorKind.TEMPLATE_TYPE_PARAMETER:
			ttp.append(c.spelling)
	return ttp

def get_direct_parents(node, parallel = None, public_only = True):
	""" Return all parent nodes of a class
	"""
	parents = []
	for cc in node.get_children():
		# print 'Search %s' % cc.spelling
		if cc.kind == cindex.CursorKind.CXX_BASE_SPECIFIER:
			# describe_node(cc)
			if parallel is not None and not is_parallel(cc.get_definition()) == parallel:
				continue
			if public_only and not cc.access_specifier == cindex.AccessSpecifier.PUBLIC:
				continue
			# print 'Found parent of %s: %s' % (node.spelling, cc.spelling)
			parents += [cc.get_definition()]
	return parents

def tokenize(arg):
	""" Return the tokenized value """
	# print 'Search default value for parameter %s :%s' % (arg.spelling, arg.type.spelling)
	return ' '.join([tok.spelling for tok in arg.get_tokens()])

# TODO Rename these functions
def list_args1(parent, front_comma = False, back_comma = False):
	""" List all types of arguments as a string with commas, if specified add an extra comma in front or end """
	out = []
	for arg in find_arguments(parent):
		out.append(get_full_name(arg.type))
	fc = ', ' if out and front_comma else ''
	bc = ', ' if out and back_comma else ''
	return fc + ', '.join(out) + bc

def list_args2(parent, front_comma = False, back_comma = False):
	""" List all arguments without types as a string with commas, if specified add an extra comma in front or end """
	out = []
	for arg in find_arguments(parent):
		out.append(arg.spelling)
	fc = ', ' if out and front_comma else ''
	bc = ', ' if out and back_comma else ''
	return fc + ', '.join(out) + bc

def list_args(parent, front_comma = False, back_comma = False):
	""" List all arguments with types as a string with commas, if specified add an extra comma in front or end """
	out = []
	for arg in find_arguments(parent):
		out.append(tokenize(arg))
	fc = ', ' if out and front_comma else ''
	bc = ', ' if out and back_comma else ''
	return fc + ', '.join(out) + bc


def align(filename):
	""" Align the file with command astyle """
	call(['astyle', '-ntS', '--style=attach', filename])

if __name__ == '__main__':
	print 'this file is a Python library: try using popgen instead'
