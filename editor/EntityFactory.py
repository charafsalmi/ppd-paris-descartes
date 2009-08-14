# -*- coding: utf-8 -*-


import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from TiledCanvas import TiledCanvas


class EntityFactory(object):
	
	class Unit:
		def __init__(self, name, hp, speed):
			self.name = name
			self.hp = hp
			self.speed = speed
	
	class Decor:
		def __init__(self, name, x, y, width, height, block, movable):
			self.name = name
			self.x = int(x)
			self.y = int(y)
			self.width = int(width)
			self.height = int(height)
			self.block = int(block)
			self.movable = bool(movable)
	
			image = QImage("../bin/data/images/decors.png")
			x = self.x * TiledCanvas.TILESIZE
			y = self.y * TiledCanvas.TILESIZE
			w = self.width * TiledCanvas.TILESIZE
			h = self.height * TiledCanvas.TILESIZE
			self.sprite = image.copy(x, y, w, h)
	
	instance = None
	
	def __new__(cls):
		if cls.instance is None:
			cls.instance = object.__new__(cls)
			cls.instance.units = {}
			cls.instance.decors = {}
			
		return cls.instance
	
	def load_units(self, filename):
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "couldn't load units defintion:", what
			return False
		
		print "loading units ..."
		unit_nodes = doc.getElementsByTagName("mob")
		for node in unit_nodes:
			id = int(node.getAttribute("id"))
			name = node.getAttribute("name")
			hp = int(node.getAttribute("hp"))
			speed = int(node.getAttribute("speed"))
			
			self.units[id] = EntityFactory.Unit(name, hp, speed)
		return True
	
	def load_decors(self, filename):
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "couldn't load decors definition: ", what
			return False
		
		print "loading decors ..."
		decor_nodes = doc.getElementsByTagName("decor")
		for node in decor_nodes:
			id = int(node.getAttribute("id"))
			name = node.getAttribute("name")
			x  = node.getAttribute("x")
			y  = node.getAttribute("y")
			width = node.getAttribute("w")
			height = node.getAttribute("h")
			block = node.getAttribute("block")
			movable = node.getAttribute("movable")
			
			self.decors[id] = EntityFactory.Decor(name, x, y, width, height, block, movable)
		return True

	def get_unit_by_id(self, id):
		if id in self.units:
			return self.units[id]
		return None
	
	def get_decor_by_id(self, id):
		if id in self.decors:
			return self.decors[id]
		return None
	
	def get_units(self):
		return self.units
	
	def get_decors(self):
		return self.decors
		
