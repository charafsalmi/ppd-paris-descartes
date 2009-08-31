# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *


class TiledCanvas(QGraphicsView):
	TILESIZE = 32
	
	def __init__(self):
		self.scene = QGraphicsScene()
		self.scene.setItemIndexMethod(QGraphicsScene.NoIndex)
		QGraphicsView.__init__(self, self.scene)
		
		self.cursor = None
		self.make_cursor()
		
		self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
		self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
		
		self.max_line = -1
		self.max_row = -1
		self.current_tile_id = -1	
	
		self.setResizeAnchor(QGraphicsView.NoAnchor)
	
	def make_cursor(self):
		if self.cursor != None:
			self.scene.removeItem(self.cursor)
		
		rect = QRectF(0, 0, self.TILESIZE, self.TILESIZE)
		pen = QPen(Qt.red, 1, Qt.SolidLine)
		brush = QBrush(QColor.fromRgb(255, 0, 0, 128))
		
		self.cursor = self.scene.addRect(rect, pen, brush)
		self.cursor.setVisible(True)
		self.cursor.setZValue(9000)
		
	def set_cursor_color(self, r, g, b, alpha=128):
		brush = QBrush(QColor.fromRgb(r, g, b, alpha))
		self.cursor.setBrush(brush)
	
	def set_cursor_visible(self, visible):
		self.cursor.setVisible(visible)
		
	def set_max_row(self, row):
		self.max_row = row
		self.setFixedWidth(self.TILESIZE * row + 5)
		
	def set_max_line(self, line):
		self.max_line = line
		self.setFixedHeight(self.TILESIZE * line + 5)
		
	def mouseMoveEvent(self, event):
		
		x = int(event.pos().x()) / self.TILESIZE
		y = int(event.pos().y()) / self.TILESIZE
		if 0 <= x < self.max_row and 0 <= y < self.max_line:
			self.cursor.setPos(self.mapToScene(x * self.TILESIZE, y * self.TILESIZE))
	
	def mousePressEvent(self, event):
		x = event.pos().x() / self.TILESIZE
		y = event.pos().y() / self.TILESIZE
		
		self.current_tile_id = y * self.max_row + x
		
	def get_tile_id(self):
		return self.current_tile_id


