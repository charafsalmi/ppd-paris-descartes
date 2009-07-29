# -*- coding: utf-8 -*-


import sys
import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from TiledCanvas import TiledCanvas
from Zone import Zone

class Map(TiledCanvas):	
	
	class Tile(QGraphicsPixmapItem):
		def __init__(self, qimage, id):
			QGraphicsPixmapItem.__init__(self)
			self.setPixmap(QPixmap.fromImage(qimage))
			self.setZValue(10)
			self.id = id
			
		def get_id(self):
			return self.id
		
		def change_image(self, qimage, id):
			self.setPixmap(QPixmap.fromImage(qimage))
			self.id = id
	
	
	class Unit(QGraphicsPixmapItem):
		def __init__(self, zone_unit):
			QGraphicsPixmapItem.__init__(self)
			self.setPixmap(QPixmap("images/entity.png"))
			self.setZValue(20)
			self.zone_unit = zone_unit
		
		def center_pos(self, x, y):
			pixmap = self.pixmap()
			x -= pixmap.width() / 2
			y -= pixmap.height() / 2
			return x, y
		
		def get_zone_unit(self):
			return self.zone_unit
			
			
	def __init__(self, tileset, tileset_path):
		TiledCanvas.__init__(self)
		self.tileset = tileset
		
		# découpage de la feuille en une liste de tiles
		self.img_tiles = []
		img_tileset = QImage(tileset_path)
		for i in xrange(tileset.WIDTH * tileset.HEIGHT):
			left = (i % tileset.WIDTH) * TiledCanvas.TILESIZE
			top = (i / tileset.WIDTH) * TiledCanvas.TILESIZE
			
			tile = img_tileset.copy(left, top, TiledCanvas.TILESIZE, TiledCanvas.TILESIZE)
			self.img_tiles.append(tile)
		
		self.setFocusPolicy(Qt.NoFocus)
		
		self.width = 0 # nombre de zones en largeur
		self.height = 0 # nombre de zones en hauteur
		self.zones = []
		self.history = []
		
		# widget is disabled untill a map is loaded
		self.setEnabled(False)
		self.set_cursor_visible(False)
		
		self.filename = None
		
		# on click callback method
		self.on_click = self.put_current_tile
		self.mouse_button_down = False
		
		self.current_unit_id = -1
		self.units = []
	
	
	def open(self, filename):
		"Charger une carte depuis un fichier"
		
		print "opening map", filename
		doc = None
		try:
			doc = xml.parse(filename)
		except Exception, what:
			print "error while opening map:", what
			return False
		
		# clean zone data
		del self.zones[:]
		
		node_map = doc.getElementsByTagName("map")[0]
		self.width = int(node_map.getAttribute("width"))
		self.height = int(node_map.getAttribute("height"))
			
		nodes_zone = doc.getElementsByTagName("zone")
		if len(nodes_zone) != self.width * self.height:
			print "error: zones manquantes"	
			return
		
		# build tile-items at first load
		if not self.isEnabled():
			self.build_tiles()
			self.setEnabled(True)
			self.set_cursor_visible(True)
		
		for count, node in enumerate(nodes_zone):
			i = count / self.width
			j = count % self.width
			zone = Zone(self)
			zone.load_from_xml(node)
			self.zones.append(zone)
			
		self.filename = filename
		
		# on affiche la zone courante
		self.set_current_zone(0)
		return True
	
	
	def create(self, width, height):
		"Créer une carte vierge"
		
		assert width >= 0
		assert height >= 0
		
		# build tile-items at first load
		if not self.isEnabled():
			self.build_tiles()
			self.setEnabled(True)
			self.set_cursor_visible(True)
		
		self.width = width
		self.height = height
		del self.zones[:]
		for i in xrange(width * height):
			zone = Zone(self)
			zone.fill_with_tile(0)
			self.zones.append(zone)
		
		self.filename = None
		self.set_current_zone(0)
		
	
	def get_filename(self):
		return self.filename
	
	
	def set_current_zone(self, index):
		"Changer la zone affichée à l'écran"
		
		del self.history[:]
		self.clear_units()
		self.current_zone_pos = index
		self.zones[index].draw()
		self.emit(SIGNAL("music_changed(PyQt_PyObject)"), self.zones[index].get_music())
	
	def get_current_zone(self):
		return self.zones[self.current_zone_pos]
	
	
	def get_current_zone_pos(self):
		i = self.current_zone_pos / self.width
		j = self.current_zone_pos % self.width
		return i, j


	def change_zone(self, dx, dy):
		y = (self.current_zone_pos / self.width) + dy
		x = (self.current_zone_pos % self.width) + dx
		if 0 <= y < self.height and 0 <= x < self.width:		
			self.set_current_zone(y * self.width + x)
			return True
			
		return False
		
	
	def build_tiles(self):
		# création des objets tiles dans la scène
		self.tiles = []
		for i in xrange(Zone.WIDTH * Zone.HEIGHT):
			tile = Map.Tile(self.img_tiles[0], 0) # 1st tile (id 0)
			self.scene.addItem(tile)
			x, y = self.pos_to_coords(i)
			tile.setPos(self.mapToScene(x, y))
			self.tiles.append(tile)
		
		# force reposition (wtf bugfix ? :/)
		for index, tile in enumerate(self.tiles):
			x, y = self.pos_to_coords(index)
			tile.setPos(self.mapToScene(x, y))
		
	
	def get_tile_images(self):
		"Liste des images des tiles découpées"
		
		return self.img_tiles
	
	
	def mousePressEvent(self, event):
		TiledCanvas.mousePressEvent(self, event)
		if event.button() == Qt.LeftButton:
			self.mouse_button_down = True
			self.on_click(event)
			
	
	def mouseMoveEvent(self, event):
		TiledCanvas.mouseMoveEvent(self, event)
		if self.mouse_button_down:
			self.on_click(event)
	
	
	def mouseReleaseEvent(self, event):
		if event.button() == Qt.LeftButton:
			assert self.mouse_button_down
			self.mouse_button_down = False
	
	
	def put_current_tile(self, event):
		tile_id = self.tileset.get_tile_id()
		if tile_id != -1:
			x = event.pos().x() / TiledCanvas.TILESIZE
			y = event.pos().y() / TiledCanvas.TILESIZE
			
			if 0 <= x < Zone.WIDTH and 0 <= y < Zone.HEIGHT:
				index = self.get_current_zone().put_tile(x, y, tile_id)
							
				# placement de la nouvelle tile et sauvegarde de l'ancienne dans
				# l'historique (si différente de la nouvelle)
				old_tile_id = self.tiles[index].get_id()
				if self.draw_tile(index, tile_id):
					self.history.append((index, old_tile_id))
	
	
	def draw_tile(self, index, id):
		if id != self.tiles[index].get_id():
			self.tiles[index].change_image(self.img_tiles[id], id)
			return True
		return False

	
	def undo_put_tile(self):
		if self.history:
			index, id = self.history.pop()
			self.draw_tile(index, id)
			self.get_current_zone().put_tile_index(index, id)
			return True
		return False
	
	
	def put_current_unit(self, event):
		assert self.current_unit_id != -1
		x = event.pos().x()
		y = event.pos().y()
		zone_unit = self.get_current_zone().add_unit(self.current_unit_id, x, y)
		self.add_unit(zone_unit)

		# back to "put tile" mode on click
		self.on_click = self.put_current_tile
		self.set_cursor_visible(True)
	
	
	def place_unit(self, id):
		"Placer une unité au prochain clic"
		
		self.current_unit_id = id
		# "put unit" mode on click
		self.on_click = self.put_current_unit
		self.set_cursor_visible(False)
	
	
	def remove_unit(self, event=None):
		"Supprimer une unité au clic"
		
		if self.on_click != self.remove_unit:
			# first call
			self.on_click = self.remove_unit
			self.set_cursor_visible(False)
		else:
			unit = self.scene.itemAt(self.mapToScene(event.pos().x(), event.pos().y()))
			if isinstance(unit, Map.Unit):
				# remove from the zone (model)
				self.get_current_zone().remove_unit(unit.get_zone_unit())
				# remove from the scene (view)
				self.scene.removeItem(unit)
				self.units.remove(unit)
			
			# back to "put tile" mode on click
			self.on_click = self.put_current_tile
			self.set_cursor_visible(True)
	
	
	def add_unit(self, zone_unit):
		"Ajouter une unité dans la scène"
		
		unit = Map.Unit(zone_unit)
		self.scene.addItem(unit)
		x, y = unit.center_pos(zone_unit.x, zone_unit.y)
		unit.setPos(self.mapToScene(x, y))
		self.units.append(unit)
	
	
	def clear_units(self):
		"Supprimer les unités de la scène"
		
		for unit in self.units:
			self.scene.removeItem(unit)
		del self.units[:]
	
	
	def fill_with_tile(self, tile_id):
		"Remplir la scène avec un type de tile"
		
		for tile in self.tiles:
			if tile.get_id() != tile_id:
				tile.change_image(self.img_tiles[tile_id], tile_id)
		self.get_current_zone().fill_with_tile(tile_id)
		
		
	def pos_to_coords(self, pos):
		x = (pos % Zone.WIDTH) * TiledCanvas.TILESIZE
		y = (pos / Zone.WIDTH) * TiledCanvas.TILESIZE
		return x, y

		
	def coords_to_pos(self, x, y):
		return y * Zone.WIDTH + x
		
	
	def save(self):
		self.save_as(self.filename)

		
	def save_as(self, filename):
		# create a xml document
		doc = xml.Document()

		root = doc.createElement("map")
		doc.appendChild(root)
		
		root.setAttribute("width", str(self.width))
		root.setAttribute("height", str(self.height))
		
		for zone in self.zones:
			node = xml.Element("zone")
			zone.to_xml(node)
			root.appendChild(node)
		
		print >> open(filename, "w"), doc.toprettyxml(indent='\t', newl='\n', encoding="utf-8")
		print "save map to", filename
		self.filename = filename

		
	def get_width(self):
		return self.width

	
	def get_height(self):
		return self.height
	
	
	def add_line(self, where, tile_id):
	
		assert where >= 0 and where <= self.height
		for i in xrange(self.width):
			zone = Zone(self)
			zone.fill_with_tile(tile_id)
			self.zones.insert(self.width * where, zone)
		self.height += 1
		self.set_current_zone(0)
	
	
	def add_column(self, where, tile_id):
		assert where >= 0 and where <= self.width
		
		for i in xrange(where, len(self.zones) + self.height, self.width + 1):
			zone = Zone(self)
			zone.fill_with_tile(tile_id)
			self.zones.insert(i, zone)
		self.width += 1
		self.set_current_zone(0)
	
	

