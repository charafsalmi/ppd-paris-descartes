# -*- coding: utf-8 -*-


class Entity:
	"entité d'une zone"
	
	def __init__(self, id_, x, y):
		self.id = id_
		self.x = x
		self.y = y
	
	def __lt__(self, other):
		return self.id < other.id
	
	def __str__(self):
		return "entité id %d (x: %d, y: %d)" % (self.id, self.x, self.y)
	
	def to_xml(self):
		return '<entity id="%d" x="%d" y="%d" />' % (self.id, self.x, self.y)
		
