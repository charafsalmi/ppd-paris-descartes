# -*- coding: utf-8 -*-


class Teleporter:
	"téléporteur d'une zone"
	
	def __init__(self, mapname, x, y, zone_x, zone_y, tile_x, tile_y):

		self.mapname = mapname
		self.x = x
		self.y = y
		self.zone_x = zone_x
		self.zone_y = zone_y
		self.tile_x = tile_x
		self.tile_y = tile_y
	
	def __str__(self):
		return "tp en [%d][%d], à destination de la map %s, zone [%d][%d], tile [%d][%d]" % (self.y, self.x, self.mapname, self.zone_y, self.zone_x, self.tile_y, self.tile_x)
	
	def to_xml(self):
		return '<tp x="%d" y="%d" map="%s" zone_x="%d" zone_y="%d" tile_x="%d" tile_y="%d" />' % \
			(self.x, self.y, self.mapname, self.zone_x, self.zone_y,
			self.tile_x, self.tile_y)

