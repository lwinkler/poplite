#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import sys
import os
import parser
import parser_ids
import parser_brok
import parser_iface
from subprocess import call


#--------------------------------------------------------------------------------

def main():
	# TODO: There seems to be a problem to parse headers with .h extensions, add -x c++ -std=c++17 and test
	filename_in = sys.argv[1]
	gendir = "gen"
	mid_out = parser.generate_file_name(filename_in, gendir, "ids")
	iface_out = parser.generate_file_name(filename_in, gendir, "iface")
	brok_out = parser.generate_file_name(filename_in, gendir, "brok")

	# if os.path.exists(mid_out): os.remove(mid_out)
	# if os.path.exists(iface_out):os.remove(iface_out)
	# if os.path.exists(brok_out): os.remove(brok_out)

	tu = parser.init_tu(sys.argv)
	parclasses = parser.find_parallel_classes(tu.cursor, None, filename_in)

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s at %s" % (c.spelling, c.location)

	if not os.path.exists(gendir):
		os.makedirs(gendir)
	
	# Generate the file containing methods and constructors ids
	print "Generate %s containing methods and constructors ids" % mid_out
	with open(mid_out, "w") as fout:

		parser_ids.write_head(fout, filename_in)

		for c in parclasses:
			parser_ids.write_meth_ids(fout, c)

		parser_ids.write_foot(fout)
	
	parser.align(mid_out)

	# Generate the file containing the interface
	print "Generate %s containing the interface" % iface_out
	with open(iface_out, "w") as fout:

		parser_iface.write_head(fout, filename_in, os.path.basename(mid_out))

		for c in parclasses:
			parser_iface.write_interface(fout, c)

		parser_iface.write_foot(fout)
	
	parser.align(iface_out)

	# Generate the file containing the broker
	print "Generate %s containing the remote broker" % brok_out
	with open(brok_out, "w") as fout:

		parser_brok.write_head(fout, filename_in, os.path.basename(filename_in))

		for c in parclasses:
			parser_brok.write_broker(fout, c)

		parser_brok.write_foot(fout)
	
	parser.align(brok_out)

	# Generate the file containing the remote main used to launch the object
	for c in parclasses:
		obj_out = os.path.dirname(filename_in) + gendir + "/main.%s.cpp" % c.spelling
		with open(obj_out, "w") as fout:
			fout.write('#include "%s"\n' % os.path.basename(brok_out))

		with open(obj_out, "a") as fout:
			call(["sed", os.path.dirname(sys.argv[0]) + "/object_main.cpp", "-e", 's/_parclass_/%s/g' % c.spelling], stdout=fout)
		



if __name__ == "__main__":
	main()
