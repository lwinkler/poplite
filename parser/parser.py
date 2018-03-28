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

cindex.Config.set_library_path("/usr/lib/llvm-4.0/lib")


def init_tu(argv):
	""" Initialize a translation unit with the clang parser
	"""
	if len(sys.argv) < 2:
		print "usage: %s <header> <arguments...>" % sys.argv[0]
		exit(1)

	index = cindex.Index.create()
	src = argv[1]
	return [src, index.parse(src, ["-D_POP_PARSER_"] + argv[2:])]

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

	found = []

	# print "class %s %s %s [line=%s, col=%s]" % (class_node.get_definition(), class_node.spelling, class_node.kind, class_node.location.line, class_node.location.column)


	# Recurse for children of this node
	for c in class_node.get_children():
		if c.kind == cindex.CursorKind.CXX_METHOD and c.access_specifier == cindex.AccessSpecifier.PUBLIC:
			# print 'Found parallel method %s [line=%s, col=%s]' % (c.spelling, c.location.line, c.location.column)
			found.append(c)
	
	return found

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

def list_args1(parent, comma = False):
	""" List all types of arguments as a string with commas, if specified add an extra comma in front """
	out = []
	for arg in find_arguments(parent):
		out.append(arg.type.spelling)
	if comma and out:
		return ',' + ','.join(out)
	else:
		return ','.join(out)

def list_args2(parent, comma = False):
	""" List all arguments without types as a string with commas, if specified add an extra comma in front """
	out = []
	for arg in find_arguments(parent):
		out.append(arg.spelling)
	if comma and out:
		return ',' + ','.join(out)
	else:
		return ','.join(out)

def list_args(parent, comma = False):
	""" List all arguments with types as a string with commas, if specified add an extra comma in front """
	out = []
	for arg in find_arguments(parent):
		out.append(arg.type.spelling + " " + arg.spelling)
	if comma and out:
		return ',' + ','.join(out)
	else:
		return ','.join(out)


def align(filename):
	""" Align the file with command astyle """
	call(["astyle", "-nToO", "--style=allman", filename])

def main():
	[src, tu] = parser.init_tu(sys.argv)
	tu = index.parse(src, ["-I."])

	parclasses = find_parallel_classes(tu.cursor, None, src)

	print "found %d parallel classes:" % len(parclasses)
	for c in parclasses:
		print "parallel class %s in %s" % (c.spelling, c.location)
		constructors = find_constructors(c)
		print "found %d constructors" % len(constructors)
		for m in constructors:
			print "\tconstr: %s allocation:%s" % (m.spelling, get_allocation(m))
			# print_ast(m, 1)
			arguments = find_arguments(m)
			for a in arguments:
				print "\t\targ %s %s" % (a.type.spelling, a.spelling)
		methods = find_methods(c)
		print "found %d parallel methods" % len(methods)
		for m in methods:
			print "\tmethod: %s %s %s" % (get_invoker(m), m.result_type.spelling, m.spelling)
			# print_ast(m, 1)
			arguments = find_arguments(m)
			for a in arguments:
				print "\t\targ %s %s" % (a.type.spelling, a.spelling)




if __name__ == "__main__":
	print "this file is a Python library: try using popgen instead"
