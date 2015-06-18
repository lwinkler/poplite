#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import sys
import clang.cindex as cindex

cindex.Config.set_library_path("/usr/lib/llvm-3.5/lib")

import parser
import parser_ids
import parser_brok
import parser_iface


#--------------------------------------------------------------------------------

def main():

	[filename_in, tu] = parser.init_tu(sys.argv)
	parclasses = parser.find_parallel_classes(tu.cursor, None, filename_in)

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s at %s" % (c.spelling, c.location)
	
	# Generate the file containing methods and constructors ids
	mid_out = parser.generate_file_name(filename_in, "generated", "ids")
	print "Generate %s containing methods and constructors ids" % mid_out
	with open(mid_out, "w") as fout:

		parser_ids.write_head(fout, filename_in)

		for c in parclasses:
			parser_ids.write_constr_ids(fout, c)
			parser_ids.write_meth_ids(fout, c)

		parser_ids.write_foot(fout)
	
	parser.align(mid_out)

	# Generate the file containing interface
	iface_out = parser.generate_file_name(filename_in, "generated", "iface")
	print "Generate %s containing methods and constructors ids" % iface_out
	with open(iface_out, "w") as fout:

		parser_iface.write_head(fout, filename_in, mid_out)

		for c in parclasses:
			parser_iface.write_interface(fout, c)

		parser_iface.write_foot(fout)
	
	parser.align(mid_out)




if __name__ == "__main__":
	main()
