#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import sys
import os
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
	gendir = "generated"

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s at %s" % (c.spelling, c.location)

	if not os.path.exists(gendir):
		os.makedirs(gendir)
	
	# Generate the file containing methods and constructors ids
	mid_out = parser.generate_file_name(filename_in, gendir, "ids")
	print "Generate %s containing methods and constructors ids" % mid_out
	with open(mid_out, "w") as fout:

		parser_ids.write_head(fout, filename_in)

		for c in parclasses:
			parser_ids.write_meth_ids(fout, c)

		parser_ids.write_foot(fout)
	
	parser.align(mid_out)

	# Generate the file containing the interface
	iface_out = parser.generate_file_name(filename_in, gendir, "iface")
	print "Generate %s containing the interface" % iface_out
	with open(iface_out, "w") as fout:

		parser_iface.write_head(fout, filename_in, mid_out)

		for c in parclasses:
			parser_iface.write_interface(fout, c)

		parser_iface.write_foot(fout)
	
	parser.align(iface_out)

	# Generate the file containing the broker
	brok_out = parser.generate_file_name(filename_in, gendir, "brok")
	print "Generate %s containing the remote broker" % brok_out
	with open(brok_out, "w") as fout:

		parser_brok.write_head(fout, filename_in, iface_out)

		for c in parclasses:
			parser_brok.write_broker(fout, c)

		parser_brok.write_foot(fout)
	
	parser.align(brok_out)




if __name__ == "__main__":
	main()
