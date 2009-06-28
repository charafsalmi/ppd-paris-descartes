# -*- coding: utf-8 -*-


import xml.dom.minidom as xml


class EntityFactory(object):
	
	class Unit:
		def __init__(self, name, hp, speed):
			self.name = name
			self.hp = hp
			self.speed = speed
	
	instance = None
	
	def __new__(cls):
		if cls.instance is None:
			cls.instance = object.__new__(cls)
			cls.instance.units = {}
			
		return cls.instance
	
	def load(self, filename):
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "could'nt load units defintion:", what
			return False
		
		unit_nodes = doc.getElementsByTagName("mob")
		for node in unit_nodes:
			id = int(node.getAttribute("id"))
			name = node.getAttribute("name")
			hp = int(node.getAttribute("hp"))
			speed = int(node.getAttribute("speed"))
			
			self.units[id] = EntityFactory.Unit(name, hp, speed)
		return True
	
	def get_unit_by_id(self, id):
		if id in self.units.iterkeys():
			return self.units[id]
		return None
	
	def get_units(self):
		return self.units
		
