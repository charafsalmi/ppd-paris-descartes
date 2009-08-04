#!/usr/bin/env python
# -*- coding: utf-8 -*-

# applique le filtre hq*x sur un tileset

import sys
import os
import PIL
from PIL import Image

def main(filename_in, filename_out, ratio=2, tile_width=16, tile_height=16):
	tmp_in = "tmp_in.bmp"
	tmp_out = "tmp_out.bmp"
	command = {
		2: "hq2x %s %s",
		3: "hq3x %s %s",
		4: "hq4x %s %s"
	}[ratio] % (tmp_in, tmp_out)
	
	img = Image.open(filename_in)
	img = img.convert("RGB")
	width, height = img.size
	# le tileset à créer
	img_output = Image.new("RGB", (width * ratio, height * ratio))
	for y in xrange(height / tile_height):
		for x in xrange(width / tile_width):
			up = y * tile_height
			left = x * tile_width
			tile = img.crop((left, up, left + tile_width, up + tile_height))
			tile.save(tmp_in)
			
			os.system(command)
			
			# on récupère et on colle la nouvelle tile
			img_output.paste(Image.open(tmp_out), (left * ratio, up * ratio))
	
	# remove temp files
	os.remove(tmp_in)
	os.remove(tmp_out)
	
	img_output.save(filename_out)
	
def show_usage():
	print "usage: ./TileX.py input output [ratio=2] [tilewidth=16] [tileheight=16]"
	print " - input : image du tileset d'origine"
	print " - output : nom de l'image du tileset à créer"
	print " - ratio : facteur appliqué au tileset d'origine"
	print " - tilewidth : largeur d'une taille en px sur le tileset d'origine"
	print " - tileheight : hauteur d'une taille en px sur le tileset d'origine"
	print " /!\\ hq<ratio>x doit être dans le path"

if __name__ == "__main__":
	if len(sys.argv) < 3:
		show_usage()
	elif not os.path.isfile(sys.argv[1]):
		print sys.argv[1], "is not a valid filename"
	else:
		main(*(sys.argv[1:3] + map(int, sys.argv[3:6])))
		print "done."

