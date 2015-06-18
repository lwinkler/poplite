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

def main():

	[filename_in, tu] = parser.init_tu(sys.argv)

	parclasses = parser.find_parallel_classes(tu.cursor, None, filename_in)

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s in %s" % (c.spelling, c.location)
		constructors = parser.find_constructors(c)
		print "found %d constructors" % len(constructors)
		for m in constructors:
			print "\tconstr: %s allocation:%s" % (m.spelling, parser.get_allocation(m))
			# print_ast(m, 1)
			arguments = parser.find_arguments(m)
			for a in arguments:
				print "\t\targ %s %s" % (a.type.spelling, a.spelling)
		methods = parser.find_methods(c)
		print "found %d parallel methods" % len(methods)
		for m in methods:
			print "\tmethod: %s %s %s" % (parser.get_invoker(m), m.result_type.spelling, m.spelling)
			# print_ast(m, 1)
			arguments = parser.find_arguments(m)
			for a in arguments:
				print "\t\targ %s %s" % (a.type.spelling, a.spelling)
	
	# Generate the file containing methods and constructors ids

	filename_out = parser.generate_file_name(filename_in, "generated", "ids")
	with open(filename_out, "w") as fout:

		write_head(fout, filename_in)

		for c in parclasses:
			write_constr_ids(fout, c)
			write_meth_ids(fout, c)

		write_foot(fout)
	
	parser.align(filename_out)




if __name__ == "__main__":
	main()
