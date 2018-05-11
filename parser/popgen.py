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

	# Generate fake interface files before parsing
	for cl in classnames_in:
		iface_out = gendir + "/%s.iface.hpp" % cl.replace('::', '/')
		idir = os.path.dirname(iface_out)
		if not os.path.exists(idir):
			os.makedirs(idir)
		with open(iface_out, "w") as fout:
			fout.write('class %s_iface;\n' % cl)

	tu = parser.init_tu(sys.argv)
	parclasses = parser.find_parallel_classes(tu.cursor, None, filename_in)

	print "found %d parallel classe(s):" % len(parclasses)
	for c in parclasses:
		print "parallel class %s at %s" % (parser.get_full_name(c), c.location.file)

	for classname_in in classnames_in:
		if classname_in not in [parser.get_full_name(cl) for cl in parclasses]:
			raise Exception('Did not find parallel class %s in %s' % (classname_in, filename_in))
	
	# Generate the file containing the interface
	for c in parclasses:
		if parser.get_full_name(c) not in classnames_in:
			continue
		iface_out = gendir + "/%s.iface.hpp" % parser.get_full_name(c).replace('::', '/')

		print "Generate %s containing the interface" % iface_out
		with open(iface_out, "w") as fout:
			parser_iface.write_head(fout, parser.get_full_name(c))
			parser_iface.write_interface(fout, c)
			parser_iface.write_foot(fout)
	
		parser.align(iface_out)

	# Generate the file containing the broker
	for c in parclasses:
		full_name = parser.get_full_name(c)
		if full_name not in classnames_in:
			continue
		brok_out = gendir + "/%s.brok.hpp" % full_name.replace('::', '/')
		print "Generate %s containing the remote broker" % brok_out

		with open(brok_out, "w") as fout:
			parser_brok.write_head(fout, full_name, c.location.file)
			parser_brok.write_broker(fout, c)
			parser_brok.write_foot(fout)
	
		parser.align(brok_out)

	# Generate the file containing the remote main used to launch the object
	for c in parclasses:
		full_name = parser.get_full_name(c)
		if full_name not in classnames_in:
			continue
		obj_out  = gendir + "/main.%s.cpp" % full_name.replace('::', '.')
		with open(obj_out, "w") as fout:
			fout.write('#include "%s.brok.hpp"\n' % full_name.replace('::', '/'))

		with open(obj_out, "a") as fout:
			call(["sed", os.path.dirname(sys.argv[0]) + "/object_main.cpp", "-e", 's/_parclass_/%s/g' % full_name], stdout=fout)

		parser.align(obj_out)
		



if __name__ == "__main__":
	main()
