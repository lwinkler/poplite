#!/usr/bin/env python3
""" Usage: call with <filename> <classname(s)> (<template instances>)
"""

import sys
import parser


#--------------------------------------------------------------------------------

def main():
	argv1 = sys.argv
	argv2 = []
	# cut arg list at --
	if '--' in sys.argv:
		ddash = sys.argv.index('--')
		argv1 = sys.argv[:ddash]
		argv2 = sys.argv[ddash + 1:]
	if len(argv1) != 2:
		print('usage: %s <header> -- <compilation arguments...>' % sys.argv[0])
		print(' e.g.: %s my_class.hpp -- -I/usr/include -I...' % sys.argv[0])
		exit(1)

	header_hpp_fname = argv1[1]

	# print 'parse ' + header_hpp_fname
	tu = parser.init_tu(header_hpp_fname, argv2)
	#parser.print_ast(tu.cursor)
	parclasses = parser.find_parallel_classes(tu.cursor, None)

	# print 'found %d parallel classe(s):' % len(parclasses)
	classnames = []
	for parclass in parclasses:
		if parser.is_parallel(parclass) and str(parclass.location.file) == header_hpp_fname:
				classnames.append(parser.get_full_name(parclass))

	sys.stdout.write(';'.join(classnames))

if __name__ == '__main__':
	main()
