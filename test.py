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

dirs = ['get_and_set', 'ping_pong', 'inheritance']

for dir1 in dirs:
	print '======> run test %s' % dir1
	process = subprocess.Popen('cd examples/%s && make run_%s' % (dir1, dir1), shell=True, stdout=subprocess.PIPE)
	process.wait()
	if process.returncode != 0:
		print '======> Fail with exit code %d' % process.returncode
		print process.stdout.read()


print 'Remaining running processes'
process = subprocess.Popen('ps', shell=True, stdout=subprocess.PIPE)
process.wait()
print process.stdout.read()
