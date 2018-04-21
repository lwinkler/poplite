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

cindex.Config.set_library_path("/usr/lib/llvm-5.0/lib")

def describe_node(node, full = False):
	""" Describe a node. For debug purposes
	"""

	print "node %s (%s) %s" % (node.spelling, node.displayname, node.kind)
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
			print "%s- %s (%s): %s %s" % (tabs, c.spelling, c.displayname, c.kind, c.data)
			if full:
				print dir(c)
				pprint(c._tu.index.obj.__dict__)
			descr(c, tabs + '\t')
	descr(node, '\t')

def init_tu(argv):
	""" Initialize a translation unit with the clang parser
	"""
	if len(sys.argv) < 2:
		print "usage: %s <header> <arguments...>" % sys.argv[0]
		exit(1)

	index = cindex.Index.create()
	src = argv[1]
	return index.parse(src, ["-D_POP_PARSER_", "-x", "c++", "-std=c++17"] + argv[2:])

def generate_file_name(header, directory, label):
	""" Generate a filename with path with an added label
	"""
	fname, ext = os.path.splitext(os.path.basename(header))
	if os.path.dirname(header):
		directory = os.path.dirname(header) + '/' + directory
	return directory + '/' + fname + '.' + label + ext

def capitalize(name):
	""" Return a capitalized version of name for use in ifndef/define
	"""
	return os.path.basename(name.upper()).replace('.', '_')


def print_ast(node, indent):
	""" Print the content of source tree
	"""
	print "\t" * indent + "node %s %s %s [line=%s, col=%s]" % (node.get_definition(), node.spelling, node.kind, node.location.line, node.location.column)
	for c in node.get_children():
		print_ast(c, indent + 1)

def find_parallel_classes(node, parent, src):
	""" Find all classes with annotation "parallel"
	"""

	found = []

	# print "node %s %s %s [line=%s, col=%s]" % (node.get_definition(), node.spelling, node.kind, node.location.line, node.location.column)
	# if node.kind == cindex.CursorKind.UNEXPOSED_ATTR:
		# print "ASDFAS"
		# print node.extent

	if node.kind == cindex.CursorKind.ANNOTATE_ATTR and node.spelling == "parallel" and parent.location.file.name == src:
		if parent.kind != cindex.CursorKind.CLASS_DECL:
			print "Warning: node %s is annoted as parallel but is not a class" % parent.spelling
		else:
			# print 'Found parallel class %s [line=%s, col=%s]' % (parent.spelling, parent.location.line, parent.location.column)
			found.append(parent)

	# Recurse for children of this node
	for c in node.get_children():
		found += find_parallel_classes(c, node, src)
	
	return found

def find_methods(class_node):
	""" Find all public methods in parallel class
	"""
	meths = []
	find_methods1(class_node, meths)
	return meths

def find_methods1(class_node, meths):
	""" Find all public methods in parallel class (mapped by signature)
	"""

	# print "class %s %s %s [line=%s, col=%s]" % (class_node.get_definition(), class_node.spelling, class_node.kind, class_node.location.line, class_node.location.column)
	par = get_direct_parents(class_node, True, True)
	if len(par) > 1:
		raise Exception('Parallel class ' + class_node.spelling + ' has more than one parallel class as parent')
	if len(par) == 1:
		find_methods1(par[0], meths)

	# Recurse for children of this node
	for c in class_node.get_children():
		if c.kind == cindex.CursorKind.CXX_METHOD and c.access_specifier == cindex.AccessSpecifier.PUBLIC:
			describe_node(c, True)
			print 'Found parallel method %s::%s [line=%s, col=%s] access=%s static=%s' % (class_node.spelling, c.displayname, c.location.line, c.location.column, c.access_specifier, c.is_static_method())
			found = False
			for meth in meths:
				if meth.displayname == c.displayname:
					# TODO: HAndle virtual
					print 'Replace method %s' % meth.displayname
					meth = c
					found = True
					break
			if not found:
				meths.append(c)
	
	# for parent in get_direct_parents(class_node, True, True):
		# find_methods1(parent, meths)

def find_constructors(class_node):
	""" Find all public methods in parallel class
	"""

	found = []

	# print "class %s %s %s [line=%s, col=%s]" % (class_node.get_definition(), class_node.spelling, class_node.kind, class_node.location.line, class_node.location.column)


	# Recurse for children of this node
	for c in class_node.get_children():
		if c.kind == cindex.CursorKind.CONSTRUCTOR and c.access_specifier == cindex.AccessSpecifier.PUBLIC:
			# print 'Found constructor %s [line=%s, col=%s]' % (c.spelling, c.location.line, c.location.column)
			found.append(c)
	
	return found

def find_arguments(meth_node):
	""" Find all public methods in parallel class
	"""

	found = []

	# print "class %s %s %s [line=%s, col=%s]" % (meth_node.get_definition(), meth_node.spelling, meth_node.kind, meth_node.location.line, meth_node.location.column)


	# Recurse for children of this node
	for c in meth_node.get_children():
		if c.kind == cindex.CursorKind.PARM_DECL:
			# print 'Found param %s %s [line=%s, col=%s]' % (c.type.spelling, c.spelling, c.location.line, c.location.column)
			found.append(c)
	
	return found

def get_invoker(meth_node):
	""" Return the method invoker (sync, async)
	"""
	# print "node %s %s %s [line=%s, col=%s]" % (meth_node.get_definition(), meth_node.spelling, meth_node.kind, meth_node.location.line, meth_node.location.column)

	for c in meth_node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR:
			return c.spelling

	return "sync" # our default

def get_allocation(constr_node):
	""" Return the allocation (sync, async)
	"""
	# print "node %s %s %s [line=%s, col=%s]" % (constr_node.get_definition(), constr_node.spelling, constr_node.kind, constr_node.location.line, constr_node.location.column)

	for c in constr_node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR:
			return c.spelling

	return "pop::local_allocator()" # our default

def is_parallel(node):
	""" Return true if a node has parallel class annotation
	"""
	# describe_node(node)
	for c in node.get_children():
		if c.kind == cindex.CursorKind.ANNOTATE_ATTR and c.spelling == "parallel":
			return True
	return False

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
			print "Found parent of %s: %s" % (node.spelling, cc.spelling)
			parents += [cc.get_definition()]
	return parents

def list_args1(parent, front_comma = False, back_comma = False):
	""" List all types of arguments as a string with commas, if specified add an extra comma in front or end """
	out = []
	for arg in find_arguments(parent):
		out.append(arg.type.spelling)
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
		out.append(arg.type.spelling + " " + arg.spelling)
	fc = ', ' if out and front_comma else ''
	bc = ', ' if out and back_comma else ''
	return fc + ', '.join(out) + bc


def align(filename):
	""" Align the file with command astyle """
	call(["astyle", "-nT", filename])

if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
