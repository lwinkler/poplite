#!/usr/bin/env python
""" Usage: call with <filename> <classname(s)>
"""

import sys
import os
import parser
import parser_brok
import parser_iface
from subprocess import call


#--------------------------------------------------------------------------------

def main():
	filename_in   = sys.argv[1]
	classnames_in = sys.argv[2].split(',')
	gendir = os.path.dirname(filename_in) + '/gen' if os.path.dirname(filename_in) else 'gen' 

	tu = parser.init_tu(sys.argv)
	parclasses = parser.find_parallel_classes(tu.cursor, None, filename_in)

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s at %s" % (c.spelling, c.location.file)

	for classname_in in classnames_in:
		if classname_in not in [cl.spelling for cl in parclasses]:
			raise Exception('Did not find parallel class %s in %s' % (classname_in, filename_in))

	if not os.path.exists(gendir):
		os.makedirs(gendir)
	
	# Generate the file containing the interface
	for c in parclasses:
		if c.spelling not in classnames_in:
			continue
		iface_out = gendir + "/%s.iface.hpp" % c.spelling
		print "Generate %s containing the interface" % iface_out

		with open(iface_out, "w") as fout:
			parser_iface.write_head(fout, c.spelling)
			parser_iface.write_interface(fout, c)
			parser_iface.write_foot(fout)
	
		parser.align(iface_out)

	# Generate the file containing the broker
	for c in parclasses:
		if c.spelling not in classnames_in:
			continue
		brok_out = gendir + "/%s.brok.hpp" % c.spelling
		print "Generate %s containing the remote broker" % brok_out

		with open(brok_out, "w") as fout:
			parser_brok.write_head(fout, c.spelling, c.location.file)
			parser_brok.write_broker(fout, c)
			parser_brok.write_foot(fout)
	
		parser.align(brok_out)

	# Generate the file containing the remote main used to launch the object
	for c in parclasses:
		if c.spelling not in classnames_in:
			continue
		obj_out  = gendir + "/main.%s.cpp" % c.spelling
		with open(obj_out, "w") as fout:
			fout.write('#include "%s.brok.hpp"\n' % c.spelling)

		with open(obj_out, "a") as fout:
			call(["sed", os.path.dirname(sys.argv[0]) + "/object_main.cpp", "-e", 's/_parclass_/%s/g' % c.spelling], stdout=fout)

		parser.align(obj_out)
		



if __name__ == "__main__":
	main()
