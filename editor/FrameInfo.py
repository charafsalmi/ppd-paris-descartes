# -*- coding: utf-8 -*-

import os

from PyQt4.QtGui import *
from PyQt4.QtCore import *


class FrameInfo(QFrame):
	def __init__(self):
		QFrame.__init__(self)
		
		# widgets
		lab_map = QLabel("Carte :")
		self.txt_map = QLineEdit()
		self.txt_map.setReadOnly(True)
		
		lab_width = QLabel("Zones en largeur :")
		self.txt_width = QLineEdit()
		self.txt_width.setReadOnly(True)
		
		lab_height = QLabel("Zones en hauteur :")
		self.txt_height = QLineEdit()
		self.txt_height.setReadOnly(True)
		
		lab_zone = QLabel("Zone courante :")
		self.lab_zone_val = QLabel()
		
		lab_current_tile = QLabel("Tile courante :")
		self.pix_current_tile = QLabel()
		lab_id_tile = QLabel("ID :")
		self.txt_id_tile = QLineEdit()
		self.txt_id_tile.setReadOnly(True)
		
		# layout en grille
		grid = QGridLayout()
		grid.addWidget(lab_map, 0, 0)
		grid.addWidget(self.txt_map, 0, 1)
		grid.addWidget(lab_width, 1, 0)
		grid.addWidget(self.txt_width, 1, 1)
		grid.addWidget(lab_height, 2, 0)
		grid.addWidget(self.txt_height, 2, 1)
		grid.addWidget(lab_zone, 3, 0)
		grid.addWidget(self.lab_zone_val, 3, 1)
		
		grid.addWidget(lab_current_tile, 0, 2)
		grid.addWidget(self.pix_current_tile, 0, 3)
		grid.addWidget(lab_id_tile, 1, 2)
		grid.addWidget(self.txt_id_tile, 1, 3)
		
		self.setLayout(grid)
	
	def set_tile_images(self, images):
		self.tiles = images
		
	def set_map_name(self, map_name):
		self.txt_map.setText(os.path.basename(map_name))
	
	def set_current_tile(self, id):
		self.txt_id_tile.setText(str(id))
		self.pix_current_tile.setPixmap(QPixmap.fromImage(self.tiles[id]))
	
	def set_current_zone(self, i, j):
		self.lab_zone_val.setText("y: %d ; x: %d" % (i, j))
	
