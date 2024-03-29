# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *
from TiledCanvas import TiledCanvas


class Tileset(TiledCanvas):
	WIDTH = 16
	HEIGHT = 20
	
	def __init__(self, tileset_path, frameinfo):
		TiledCanvas.__init__(self)
		
		self.set_tileset_image(tileset_path)		
		self.info = frameinfo
		
	
	def set_tileset_image(self, tileset_path):
		self.tileset = self.scene.addPixmap(QPixmap(tileset_path))
		self.tileset.setZValue(0)
	
	
	def mousePressEvent(self, event):
		TiledCanvas.mousePressEvent(self, event)
		self.info.set_current_tile(self.get_tile_id())
