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
	
	class Item:
		def __init__(self, label, x, y, w, h):
			self.label = label
			self.x = int(x)
			self.y = int(y)
			self.w = int(w)
			self.h = int(h)
			
			image = QImage("../bin/data/images/items.png")
			self.sprite = image.copy(self.x, self.y, self.w, self.h)
			
	instance = None
	
	def __new__(cls):
		if cls.instance is None:
			cls.instance = object.__new__(cls)
			cls.instance.units = {}
			cls.instance.decors = {}
			cls.instance.items = {}
			
		return cls.instance
	
	def load_units(self, filename):
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "couldn't load units defintion:", what
			return False
		
		print "loading units ..."
		self.units = {}
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
			print "couldn't load decors definition:", what
			return False
		
		print "loading decors ..."
		self.decors = {}
		decor_nodes = doc.getElementsByTagName("decor")
		for node in decor_nodes:
			id = int(node.getAttribute("id"))
			name = node.getAttribute("name")
			x = node.getAttribute("x")
			y = node.getAttribute("y")
			width = node.getAttribute("w")
			height = node.getAttribute("h")
			block = node.getAttribute("block")
			movable = node.getAttribute("movable")
			
			self.decors[id] = EntityFactory.Decor(name, x, y, width, height, block, movable)
		return True
	
	def load_items(self, filename):
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "couldn't load items definition:", what
			return False
		
		print "loading items ..."
		self.items = {}
		item_nodes = doc.getElementsByTagName("item")
		for node in item_nodes:
			name = node.getAttribute("name")
			label = node.getAttribute("label")
			x = node.getAttribute("x")
			y = node.getAttribute("y")
			w = node.getAttribute("w")
			h = node.getAttribute("h")
			self.items[str(name)] = EntityFactory.Item(label, x, y, w, h)
		
		return True
		
	def get_unit_by_id(self, id):
		return self.units[id]
	
	def get_decor_by_id(self, id):
		return self.decors[id]
	
	def get_item_by_name(self, name):
		return self.items[name]

	def get_units(self):
		return self.units
	
	def get_decors(self):
		return self.decors
	
	def get_items(self):
		return self.items
	
