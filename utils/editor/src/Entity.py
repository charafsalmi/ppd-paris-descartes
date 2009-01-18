# -*- coding: utf-8 -*-

class Entity:
	"entité d'une zone"
	
	def __init__(self, name, x, y):
		self.name = name
		self.x = x
		self.y = y
	
	def __lt__(self, other):
		return self.name < other.name
	
	def __str__(self):
		return "%s (x: %d, y: %d)" % (self.name, self.x, self.y)
	
	def to_xml(self):
		return '<entity name="%s" x="%d" y="%d" />' % (self.name, self.x, self.y)
		
