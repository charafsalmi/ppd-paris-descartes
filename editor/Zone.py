# -*- coding: utf-8 -*-


import xml.dom.minidom as xml
class Zone:
	"Un écran de la carte, avec ses tiles et ses entités"
	
	WIDTH = 20
	HEIGHT = 16
	
	
	class GenericEntity:
		"Entité avec en attribut un id et une position (x y)"
		
		def __init__(self, _id, x, y):
			self.id = int(_id)
			self.x = int(x)
			self.y = int(y)
		
		def __str__(self):
			return "entity id %d at (x: %d, y: %d)" % (self.id, self.x, self.y)
		
		@staticmethod
		def from_xml(node):
			_id = node.getAttribute("id")
			x = node.getAttribute("x")
			y = node.getAttribute("y")
			return Zone.GenericEntity(_id, x, y)
		
		def to_xml(self, parent, tagname):
			node = xml.Element(tagname)
			node.setAttribute("id", str(self.id))
			node.setAttribute("x", str(self.x))
			node.setAttribute("y", str(self.y))
			parent.appendChild(node)
	
	class Item:
		def __init__(self, name, x, y):
			self.name = name
			self.x = int(x)
			self.y = int(y)
		
		def __str__(self):
			return "item '%s' at (x: %d, y: %d)" % (self.name, self.x, self.y)
		
		@staticmethod
		def from_xml(node):
			name = node.getAttribute("name")
			x = node.getAttribute("x")
			y = node.getAttribute("y")
			return Zone.Item(name, x, y)
		
		def to_xml(self, parent):
			node = xml.Element("item")
			node.setAttribute("name", self.name)
			node.setAttribute("x", str(self.x))
			node.setAttribute("y", str(self.y))
			parent.appendChild(node)
	
	class Teleporter:
		def __init__(self, mapname, x, y, zone_x, zone_y, tile_x, tile_y):

			self.mapname = mapname
			self.x = int(x)
			self.y = int(y)
			self.zone_x = int(zone_x)
			self.zone_y = int(zone_y)
			self.tile_x = int(tile_x)
			self.tile_y = int(tile_y)
		
		def __str__(self):
			return "tp en tile [%d][%d], à destination de la map %s, zone [%d][%d], tile [%d][%d]" % (
				self.y, self.x,
				self.mapname,
				self.zone_y, self.zone_x,
				self.tile_y, self.tile_x)
		
		@staticmethod
		def from_xml(node):
			mapname = node.getAttribute("map")
			x = node.getAttribute("x")
			y = node.getAttribute("y")
			zone_x = node.getAttribute("zone_x")
			zone_y = node.getAttribute("zone_y")
			tile_x = node.getAttribute("tile_x")
			tile_y = node.getAttribute("tile_y")
			return Zone.Teleporter(mapname, x, y, zone_x, zone_y, tile_x, tile_y)
			
		def to_xml(self, parent):
			node = xml.Element("tp")
			node.setAttribute("x", str(self.x))
			node.setAttribute("y", str(self.y))
			node.setAttribute("map", self.mapname)
			node.setAttribute("zone_x", str(self.zone_x))
			node.setAttribute("zone_y", str(self.zone_y))
			node.setAttribute("tile_x", str(self.tile_x))
			node.setAttribute("tile_y", str(self.tile_y))
			parent.appendChild(node)
		

	def __init__(self, map_):
		
		self.map = map_
		
		self.tile_ids = None
		# note : la zone n'a pas de tiles et ne peut pas être encore utilisée.
		# soit on fournit les tiles depuis une carte XML (load_from_xml),
		# soit on remplit la zone avec un type de tile (fill_with_tile)
		
		self.music = ""
		self.units = []
		self.items = []
		self.decors = []		
		self.teleporters = []
		
	
	def load_from_xml(self, node):
		"Construction de la zone depuis un noeud XML"
		
		# parsing tiles (stockées dans une liste d'entier)
		self.tile_ids = map(
			int,
			node.getElementsByTagName("tiles")[0].firstChild.data.split())
		found = len(self.tile_ids)
		count = Zone.WIDTH * Zone.HEIGHT
		if found != count:
			print "WARNING: %s tiles found, %d expected" % (found, count)
			print "missing tiles will be set to 0"
			for i in xrange(found, count):
				self.tile_ids.append(0)
				
		# parsing music
		try:
			self.music = node.getElementsByTagName("music")[0].firstChild.data.strip()
		except IndexError:
			print "WARNING: music is missing"
		
		# parsing units
		for unit_node in node.getElementsByTagName("entity"):
			self.units.append(Zone.GenericEntity.from_xml(unit_node))
		
		# parsing decors
		for decor_node in node.getElementsByTagName("decor"):
			self.decors.append(Zone.GenericEntity.from_xml(decor_node))
		
		# parsing items
		for itm_node in node.getElementsByTagName("item"):
			self.items.append(Zone.Item.from_xml(itm_node))
		
		# parsing teleporters
		for tp_node in node.getElementsByTagName("tp"):
			self.teleporters.append(Zone.Teleporter.from_xml(tp_node))
	
		
	def draw(self):
		"Afficher le contenu de la zone dans la carte"
		
		# rendering tiles
		for index, id in enumerate(self.tile_ids):
			self.map.draw_tile(index, id)
		
		# rendering decors
		for decor in self.decors:
			self.map.add_decor(decor)
		
		# rendering units
		for unit in self.units:
			self.map.add_unit(unit)
		
		# rendering items
		for item in self.items:
			self.map.add_item(item)
		
		
	def put_tile(self, x, y, id):
		"Mettre une tile à une position dans la zone"
		
		index = self.map.coords_to_pos(x, y)
		self.tile_ids[index] = id
		return index
	
	def put_tile_index(self, index, id):
		"Mettre une tile dans la zone (version index)"
		
		self.tile_ids[index] = id
	
	
	def fill_with_tile(self, id):
		"Remplir la zone avec une tile unique"
		
		self.tile_ids = [id] * (Zone.WIDTH * Zone.HEIGHT)
	
	
	def add_unit(self, id, x, y):
		unit = Zone.GenericEntity(id, x, y)
		self.units.append(unit)
		return unit
	
	
	def add_decor(self, id, x, y):
		decor = Zone.GenericEntity(id, x, y)
		self.decors.append(decor)
		return decor
	
	
	def add_item(self, name, x, y):
		item = Zone.Item(name, x, y)
		self.items.append(item)
		return item
	
	
	def remove_unit(self, unit):
		self.units.remove(unit)
	
	
	def remove_decor(self, decor):
		self.decors.remove(decor)
		
	
	def remove_item(self, item):
		self.items.remove(item)
	
	
	def to_xml(self, parent):
		"Encoder la zone en xml"
		
		# encoding tiles
		tiles = xml.Text()
		tiles.data = ""
		for i, id in enumerate(self.tile_ids):
			tiles.data += "%3d " % id
			#if (i + 1) % Zone.WIDTH == 0:
			#	tiles.data += "\n"
		
		node = xml.Element("tiles")
		node.appendChild(tiles)
		parent.appendChild(node)
		
		# encoding music
		node = xml.Element("music")
		music = xml.Text()
		music.data = self.music
		node.appendChild(music)
		parent.appendChild(node)
		
		# encoding units
		if self.units:
			node = xml.Element("entities")
			for unit in self.units:
				unit.to_xml(node, "entity")
			parent.appendChild(node)
		
		# encoding decors
		if self.decors:
			node = xml.Element("decors")
			for decor in self.decors:
				decor.to_xml(node, "decor")
			parent.appendChild(node)
		
		# encoding items
		if self.items:
			node = xml.Element("items")
			for item in self.items:
				item.to_xml(node)
			parent.appendChild(node)
		
		# encoding teleporters
		if self.teleporters:
			node = xml.Element("teleporters")
			for tp in self.teleporters:
				tp.to_xml(node)
			parent.appendChild(node)
	

	def count_entities(self):
		return len(self.units) + len(self.decors) + len(self.items)
	
	
	def get_music(self):
		return self.music
	
	
	def set_music(self, music):
		self.music = music
		
