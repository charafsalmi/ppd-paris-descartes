#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys


if len(sys.argv) < 2:
	print "Error: You specified too few arguments!\n"
else:
	infile = open(sys.argv[1])
	outfile = open(os.path.splitext(sys.argv[2])[0] + "_old_.xml", "w")
	diff = open(sys.argv[2])
	maps = sorted([map(int, item.split(" >> ")) for item in diff.readlines()])
	for pair in maps:
		print  pair

#TODO:
#	Ouvrir IN
#		Pour chaque ligne dans IN, la recopier dans OUT
#		en remplaçant l'id s'il est dans maps[0], par celui de maps[1]
#		correspondant. [puis supprimer la paire utilisée de maps, sauf si
#		on autorise plusieurs types par tile]
#

