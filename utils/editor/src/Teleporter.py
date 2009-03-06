# -*- coding: utf-8 -*-


class Teleporter:
	"téléporteur d'une zone"
	
	def __init__(self, container, x, y, zone_x, zone_y, tile_x, tile_y):

		self.container = container
		self.x = x
		self.y = y
		self.zone_x = zone_x
		self.zone_y = zone_x
		self.tile_x = tile_x
		self.tile_y = tile_y
	
	def __str__(self):
		return "tp en [%d][%d], à destination de la map %d, zone [%d][%d], tile [%d][%d]" % (self.y, self.x, self.container, self.zone_y, self.zone_x, self.tile_y, self.tile_x)
	
	def to_xml(self):
		return '<tp x="%d" y="%d" container="%d" zone_x="%d" zone_y="%d" tile_x="%d" tile_y="%d" />' % \
			(self.x, self.y, self.container, self.zone_x, self.zone_y,
			self.tile_x, self.tile_y)

