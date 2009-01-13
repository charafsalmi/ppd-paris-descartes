
#!/usr/bin/env python

import os, sys

if len(sys.argv) < 2:
	print "Error  : You specified too few arguments!\n"
else:
	in_ = file(sys.argv[1])
	diff = file(sys.argv[2])
	strings= str(sys.argv[2]).partition(".")
	outfile = strings[0] + "_old_.xml"
	out = open (outfile, "w")

	maps = diff.readlines()
	maps = [item.partition(" >> ") for item in maps]
	# supprime le \n du 3me lm et supprime le 2me lm (">>") pour chaque bloc de 3 dans old:
	maps = [[obj[0]] + [obj[-1][:-1]] for obj in maps]
	
	maps.sort(cmp=lambda a, b: len(a[0]) < len(b[0])) 

	print maps

#TODO:
#	Ouvrir IN
#		Pour chaque ligne dans IN, la recopier dans OUT
#		en remplaçant l'id s'il est dans maps[0], par celui de maps[1]
#		correspondant. [puis supprimer la paire utilisée de maps, sauf si
#		on autorise plusieurs types par tile]
#

