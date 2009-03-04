# -*- coding: utf-8 -*-

import xml.dom.minidom as dom

class Teleporter:
	"téléporteur d'une zone"
	
	def __init__(self, xml_tp):

		self.container = int(xml_tp.getAttribute("container"))
		self.x = int(xml_tp.getAttribute("x"))
		self.y = int(xml_tp.getAttribute("y"))
		self.zone_x = int(xml_tp.getAttribute("zone_x"))
		self.zone_y = int(xml_tp.getAttribute("zone_y"))
		self.tile_x = int(xml_tp.getAttribute("tile_x"))
		self.tile_y = int(xml_tp.getAttribute("tile_y"))
	
	def to_xml(self):
		return '<tp x="%d" y="%d" container="%d" zone_x="%d" zone_y="%d" tile_x="%d" tile_y="%d" />' % \
			(self.x, self.y, self.container, self.zone_x, self.zone_y,
			self.tile_x, self.tile_y)
