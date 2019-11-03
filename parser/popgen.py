#!/usr/bin/env python3
""" Usage: call with <filename> <classname(s)> (<template instances>)
"""

import sys
import os
import parser
import parser_brok
import parser_iface
from subprocess import call
from io import StringIO
import tarfile

#--------------------------------------------------------------------------------
def write_to_tar(tar, filename, strio):
	""" Write a string io object to a tar file """
	strio.seek(0)
	info = tarfile.TarInfo(name=filename)
	info.size=len(strio.buf)
	tar.addfile(tarinfo=info, fileobj=strio)

#--------------------------------------------------------------------------------

def main():
	argv1 = sys.argv
	argv2 = []
	# cut arg list at --
	if '--' in sys.argv:
		ddash = sys.argv.index('--')
		argv1 = sys.argv[:ddash]
		argv2 = sys.argv[ddash + 1:]
	if len(argv1) != 3:
		print('usage: %s <header> <classname> -- <compilation arguments...>' % sys.argv[0])
		print(' e.g.: %s my_class.hpp ns::my_class -- -I/usr/include -I...' % sys.argv[0])
		exit(1)

	header_hpp_fname = argv1[1]
	classnames_in = argv1[2].split(',')
	gendir = os.path.dirname(header_hpp_fname) + '/gen' if os.path.dirname(header_hpp_fname) else 'gen' 

	# Generate fake interface files before parsing
	for cl in classnames_in:
		iface_out = gendir + '/%s.iface.hpp' % parser.convert_to_classname(cl)
		idir = os.path.dirname(iface_out)
		if not os.path.exists(idir):
			os.makedirs(idir)
		with open(iface_out, 'w') as fout:
			parser_iface.write_forward_declaration(fout, cl)

	tar = tarfile.TarFile(gendir + '/' + header_hpp_fname + '.tar','w')

	tu = parser.init_tu(header_hpp_fname, argv2)
	#parser.print_ast(tu.cursor)
	parclasses = parser.find_parallel_classes(tu.cursor, classnames_in)

	print('found %d parallel classe(s):' % len(parclasses))
	for c in parclasses:
		print('parallel class %s at %s' % (parser.get_full_name(c), c.location.file))

	for classname_in in classnames_in:
		if classname_in not in [parser.get_full_name(cl) for cl in parclasses]:
			raise Exception('Did not find parallel class %s in %s' % (classname_in, header_hpp_fname))
	
	# Generate the file containing the interface
	for c in parclasses:
		full_name = parser.get_full_name(c)
		if full_name not in classnames_in:
			continue
		# print 'Generate %s containing the interface' % iface_hpp_fname
		iface_hpp_fout = StringIO()
		iface_cpp_fout = StringIO()
		parser_iface.write_head(iface_hpp_fout, full_name)
		iface_cpp_fout.write('#include "%s"\n' % (header_hpp_fname))
		parser_iface.write_interface(iface_hpp_fout, c, iface_cpp_fout)
		parser_iface.write_foot(iface_hpp_fout)

		write_to_tar(tar, gendir + '/%s.iface.hpp' % parser.convert_to_classname(full_name), iface_hpp_fout)
		write_to_tar(tar, gendir + '/%s.iface.cpp' % parser.convert_to_classname(full_name), iface_cpp_fout)

	# Generate the file containing the remote main used to launch the object
	for c in parclasses:
		full_name = parser.get_full_name(c)
		if full_name not in classnames_in:
			continue

		brok_cpp_fout = StringIO()
		obj_cpp_fout = StringIO()
		parser_brok.write_head(brok_cpp_fout, full_name, c.location.file)
		parser_brok.write_broker(brok_cpp_fout, c)
		# parser_brok.write_foot(brok_cpp_fout)

		parser_brok.write_head(obj_cpp_fout, full_name, c.location.file)
		parser_brok.write_main(obj_cpp_fout, c)
		# parser_brok.write_foot(obj_cpp_fout)

		write_to_tar(tar, gendir + '/%s.brok.cpp' % parser.convert_to_classname(full_name), brok_cpp_fout)
		write_to_tar(tar, gendir + '/main.%s.cpp' % parser.convert_to_objname(full_name), obj_cpp_fout)
	
	tar.close()

if __name__ == '__main__':
	main()
