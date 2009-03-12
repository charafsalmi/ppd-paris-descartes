# -*- coding: utf-8 -*-

import xml.dom.minidom as dom

from Entity import Entity
from Teleporter import Teleporter
from Item import Item
from Decor import Decor


ZONE_WIDTH = 20
ZONE_HEIGHT = 16


class Zone:
	def __init__(self, xml_elem):
		# données d'une zone
		self.entities = []
		self.items = []
		self.id_tiles = []
		self.teleporters = []
		self.decors = []
		self.music_name = ""
		
		# parsing music
		try:
			self.music_name = xml_elem.getElementsByTagName("music")[0].firstChild.data
		except IndexError:
			pass # pas de musique
		else:
			print "parsing music :", self.music_name,
		
		# parsing tiles
		all_tiles = xml_elem.getElementsByTagName("tiles")[0].firstChild.data.split()
		for i in xrange(ZONE_WIDTH * ZONE_HEIGHT):
			self.id_tiles.append(int(all_tiles[i]))
		
		# parsing entities
		try:
			all_entities = xml_elem.getElementsByTagName("entities")[0].getElementsByTagName("entity")
		except IndexError:
			pass # pas d'entités
		else:
			print "parsing", len(all_entities), "entities",
			for node in all_entities:
				id_ = int(node.getAttribute("id"))
				x = int(node.getAttribute("x"))
				y = int(node.getAttribute("y"))
				self.entities.append(Entity(id_, x, y))
		
		# parsing decors
		try:
			xml_decors = xml_elem.getElementsByTagName("decors")[0].getElementsByTagName("decor")
		except IndexError:
			pass # pas de décors
		else:
			print "parsing", len(xml_decors), "decors",
			for node in xml_decors:
				_id = int(node.getAttribute("id"))
				x = int(node.getAttribute("x"))
				y = int(node.getAttribute("y"))
				self.decors.append(Decor(_id, x, y))
				
		# parsing items
		try:
			xml_items = xml_elem.getElementsByTagName("items")[0].getElementsByTagName("item")
		except IndexError:
			pass # pas d'items
		else:
			print "parsing", len(xml_items), "items",
			for node in xml_items:
				_id = int(node.getAttribute("id"))
				x = int(node.getAttribute("x"))
				y = int(node.getAttribute("y"))
				self.items.append(Item(_id, x, y))
		
		# parsing teleporters
		try:
			xml_teleporters = xml_elem.getElementsByTagName("teleporters")[0].getElementsByTagName("tp")
		except IndexError:
			pass # pas de tp
		else:
			print "parsing", len(xml_teleporters), "teleporters",
			for node in xml_teleporters:
				container = int(node.getAttribute("container"))
				x = int(node.getAttribute("x"))
				y = int(node.getAttribute("y"))
				zone_x = int(node.getAttribute("zone_x"))
				zone_y = int(node.getAttribute("zone_y"))
				tile_x = int(node.getAttribute("tile_x"))
				tile_y = int(node.getAttribute("tile_y"))
				self.teleporters.append(Teleporter(container, x, y, zone_x, zone_y, tile_x, tile_y))
		
		print " ... done."
		
	def get_entities(self):
		"liste des entités de la zone"
		
		return self.entities
	
	def get_teleporters(self):
		"liste des téléporteurs de la zone"
		
		return self.teleporters
		
	def draw(self, app):
		"""
		dessiner toute la zone
		app: instance de l'application
		"""
		
		# draw tiles
		for position, id_tile in enumerate(self.id_tiles):
			app.draw_tile(position, id_tile)
		
	def put_tile_at(self, position, tile_id):
		self.id_tiles[position] = tile_id
		
	def get_tile_at(self, position):
		return self.id_tiles[position]
		
	def fill_tiles(self, tile_id):
		self.id_tiles = [tile_id] * len(self.id_tiles)
		
	def save(self, f):
		"""
		écriture de la zone en XML
		f: fichier ouvert en écriture
		"""
		
		f.write("<zone>\n")
		
		# écriture des tiles
		f.write("\t<tiles>\n")
		for i, tile in enumerate(self.id_tiles):
			f.write("%3d " % tile)
			if (i + 1) % ZONE_WIDTH == 0:
				f.write("\n")
		f.write("\t</tiles>\n")
		
		# écriture de la musique
		f.write("\t<music>%s</music>\n" % self.music_name)
		
		# écriture des entités
		if len(self.entities) > 0:
			f.write("\t<entities>\n")
			for entity in self.entities:
				f.write("\t\t%s\n" % entity.to_xml())
			f.write("\t</entities>\n")
		
		# écriture des décors
		if len(self.decors) > 0:
			f.write("\t<decors>\n")
			for decor in self.decors:
				f.write("\t\t%s\n" % decor.to_xml())
			f.write("\t</decors>\n")
		
		# écriture des items
		if len(self.items) > 0:
			f.write("\t<items>\n")
			for it in self.items:
				f.write("\t\t%s\n" % it.to_xml())
			f.write("\t</items>\n")
		
		# écriture des téléporteurs
		if len(self.teleporters) > 0:
			f.write("\t<teleporters>\n")
			for tp in self.teleporters:
				f.write("\t\t%s\n" % tp.to_xml())
			f.write("\t</teleporters>\n")
		
		f.write("</zone>\n")

