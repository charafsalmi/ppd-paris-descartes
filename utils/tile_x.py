#!/usr/bin/env python
# -*- coding: utf-8 -*-

# applique le filtre hq*x sur un tileset
# usage: ./tile_x.py tileset_input.png tileset_output.png ratio(2, 3, ou 4) tilesize_x, tilesize_y

import sys
import os
import PIL
from PIL import Image

# taille des tiles du tileset en input
TILE_SIZE_X = 16
TILE_SIZE_Y = 16
filename_in = sys.argv[1]
filename_out = sys.argv[2]
if len(sys.argv) > 3:
	ratio = sys.argv[3]
	ratio = int(ratio)
else:
	ratio = 2
if len(sys.argv) > 4:
	TILE_SIZE_X = sys.argv[4]
	TILE_SIZE_X = int(TILE_SIZE_X)
if len(sys.argv) > 5:
	TILE_SIZE_Y = sys.argv[5]
	TILE_SIZE_Y = int(TILE_SIZE_Y)

if os.path.isfile(filename_in):
	img = Image.open(filename_in)
	img = img.convert("RGB")
	width, height = img.size
	# le tileset à créer
	tileset2x = Image.new("RGB", (width * ratio, height * ratio))
	
	input_name = "input.bmp"
	output_name = "output.bmp"
	for y in xrange(height / TILE_SIZE_Y):
		for x in xrange(width / TILE_SIZE_X):
			up = x * TILE_SIZE_X
			left = y * TILE_SIZE_Y
			tile = img.crop((left, up, left + TILE_SIZE_X, up + TILE_SIZE_Y))		
			tile.save(input_name)
			if ratio == 2:
				os.system("hq2x %s %s" % (input_name, output_name))
			elif ratio == 3:
				os.system("hq3x %s %s" % (input_name, output_name))
			elif ratio == 4:
				os.system("hq4x %s %s" % (input_name, output_name))
			# on récupère la tile
			tile2x = Image.open(output_name)
			tileset2x.paste(tile2x, (up * ratio, left * ratio))
	
	# clean temp files
	os.remove(input_name)
	os.remove(output_name)
	
	tileset2x.save(filename_out)
	print "done."

