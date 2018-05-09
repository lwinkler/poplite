#!/usr/bin/env python

##
## test.py
## ~~~~~~~
##
## Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
## 
## Distributed under the Boost Software License, Version 1.0. (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
##


import subprocess

dirs = [
	'examples/get_and_set', 
	'examples/ping_pong', 
	'examples/inheritance',
	#'popc_tests/bigdata',
	'popc_tests/callback',
	'popc_tests/exceptions',
	'popc_tests/heritage',
	'popc_tests/stresstest',
	'popc_tests/stdlib',
	'popc_tests/tree'
]

for dir1 in dirs:
	print '======> run test %s' % dir1
	rname = dir1.split('/')[-1]
	process = subprocess.Popen('cd %s && make run_%s' % (dir1, rname), shell=True, stdout=subprocess.PIPE)
	process.wait()
	if process.returncode != 0:
		print '======> Fail with exit code %d' % process.returncode
		print process.stdout.read()


print 'Remaining running processes'
process = subprocess.Popen('ps', shell=True, stdout=subprocess.PIPE)
process.wait()
print process.stdout.read()
