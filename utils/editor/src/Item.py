# -*- coding: utf-8 -*-


class Item:
	"objet contenu dans une zone"
	
	def __init__(self, code, x, y):
		self.code = code
		self.x = x
		self.y = y
	
	def __lt__(self, other):
		return self.code < other.code
	
	def __str__(self):
		return "Item code %s (x: %d, y: %d)" % (self.code, self.x, self.y)
	
	def to_xml(self):
		return '<item code="%s" x="%d" y="%d" />' % (self.code, self.x, self.y)
		
