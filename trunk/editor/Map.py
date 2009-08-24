# -*- coding: utf-8 -*-


import sys
import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from TiledCanvas import TiledCanvas
from Zone import Zone
from EntityFactory import EntityFactory


class Map(TiledCanvas):	
	
	Z_TILE = 1000
	Z_DECOR = 2000
	Z_UNIT = 3000
	Z_ITEM = 4000
	
	class Tile(QGraphicsPixmapItem):
		def __init__(self, qimage, id):
			QGraphicsPixmapItem.__init__(self)
			self.setPixmap(QPixmap.fromImage(qimage))
			self.setZValue(Map.Z_TILE)
			self.id = id
			
		def get_id(self):
			return self.id
		
		def change_image(self, qimage, id):
			self.setPixmap(QPixmap.fromImage(qimage))
			self.id = id
	
	
	class Entity(QGraphicsPixmapItem):
		def __init__(self, zone_element):
			QGraphicsPixmapItem.__init__(self)
			self.zone_element = zone_element
		
		def get_zone_element(self):
			return self.zone_element
		
		def update_zone_pos(self, x, y):
			self.zone_element.x = x
			self.zone_element.y = y
	
	
	class Unit(Entity):
		def __init__(self, zone_unit):
			Map.Entity.__init__(self, zone_unit)
			self.setPixmap(QPixmap("images/entity.png"))
			self.setZValue(Map.Z_UNIT)
		
		def place(self, x, y):
			pixmap = self.pixmap()
			x -= pixmap.width() / 2
			y -= pixmap.height() / 2
			self.setPos(x, y)
		
	
	class Decor(Entity):
		def __init__(self, zone_decor):
			Map.Entity.__init__(self, zone_decor)
			self.setPixmap(QPixmap.fromImage(EntityFactory().get_decor_by_id(zone_decor.id).sprite))
			self.setZValue(Map.Z_DECOR + zone_decor.y)
			self.zone_decor = zone_decor
		
		def place(self, x, y):
			y -= self.pixmap().height()
			# force position on tile corner
			x = x / TiledCanvas.TILESIZE * TiledCanvas.TILESIZE
			y = y / TiledCanvas.TILESIZE * TiledCanvas.TILESIZE
			self.setPos(x, y)
			self.setZValue(Map.Z_DECOR + self.get_zone_element().y)
	
		def update_zone_pos(self, x, y):
			self.zone_element.x = x / TiledCanvas.TILESIZE
			self.zone_element.y = y / TiledCanvas.TILESIZE - 1
	
	
	class Item(Entity):
		def __init__(self, zone_item):
			Map.Entity.__init__(self, zone_item)
			self.setPixmap(QPixmap.fromImage(EntityFactory().get_item_by_name(zone_item.name).sprite))
			self.setZValue(Map.Z_ITEM)
			self.zone_item = zone_item
		
		def place(self, x, y):
			pixmap = self.pixmap()
			y -= pixmap.height()
			self.setPos(x, y)
		
		
	def __init__(self, tileset, tileset_path):
		TiledCanvas.__init__(self)
		self.tileset = tileset
		
		# découpage de la feuille en une liste de tiles
		self.img_tiles = []
		img_tileset = QImage(tileset_path)
		for i in xrange(tileset.WIDTH * tileset.HEIGHT):
			left = (i % tileset.WIDTH) * self.TILESIZE
			top = (i / tileset.WIDTH) * self.TILESIZE
			
			tile = img_tileset.copy(left, top, self.TILESIZE, self.TILESIZE)
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
		
		self.selected_entity = None # pour les déplacements et les suppressions
		self.entities = []
		self.scene.setSceneRect(0, 0, Zone.WIDTH * self.TILESIZE, Zone.HEIGHT * self.TILESIZE)
		
		# hover cursor
		rect = QRectF(0, 0, self.TILESIZE, self.TILESIZE)
		pen = QPen(Qt.blue, 1, Qt.SolidLine)
		brush = QBrush(QColor.fromRgb(0, 0, 255, 64))
		
		self.hover_cursor = self.scene.addRect(rect, pen, brush)
		self.hover_cursor.setVisible(False)
		self.hover_cursor.setZValue(Map.Z_UNIT + 1)
		
	
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
			tile.setPos(x, y)
			self.tiles.append(tile)

	
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
		if self.on_click == self.put_current_tile:
			if self.mouse_button_down:
				self.on_click(event)
		else:
			if self.on_click == self.place_entity:
				if self.selected_entity:
					self.selected_entity.place(event.pos().x(), event.pos().y())
					return
			
			entities = self.scene.items(self.mapToScene(event.pos().x(), event.pos().y()))
			for entity in entities:
				if self.selected_entity == entity:
					return
				
				if isinstance(entity, Map.Entity):
					self.selected_entity = entity
					rect = entity.boundingRect()
					rect.moveTo(entity.pos())
					self.hover_cursor.setRect(rect)
					self.hover_cursor.setVisible(True)
					return
			
			self.hover_cursor.setVisible(False)
			self.selected_entity = None
			
	
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
	
	
	def place_unit(self, id):
		"Place unit on next click"
		zone_unit = self.get_current_zone().add_unit(id, 0, 0)
		self.selected_entity = self.add_unit(zone_unit)
		self.mode_place_entity()
	
	
	def place_decor(self, id):
		"Place decor on next click"
		zone_decor = self.get_current_zone().add_decor(id, 0, 0)
		self.selected_entity = self.add_decor(zone_decor)
		self.mode_place_entity()
	
	
	def place_item(self, name):
		zone_item = self.get_current_zone().add_item(name, 0, 0)
		self.selected_entity = self.add_item(zone_item)
		self.mode_place_entity()
	
	
	def mode_place_tile(self):
		"Utilisation en mode placement de tile"
		self.on_click = self.put_current_tile
		self.setCursor(QCursor(Qt.ArrowCursor))
		self.set_cursor_visible(True)
		self.hover_cursor.setVisible(False)
		self.selected_entity = None
	
	
	def mode_place_entity(self):
		"Utilisation en mode placement d'entité"
		self.set_cursor_visible(False)
		self.hover_cursor.setVisible(False)
		self.setCursor(QCursor(Qt.ClosedHandCursor))
		self.on_click = self.place_entity
	
	
	def place_entity(self, event=None):
		"Place the current selected entity (add or move)"
		
		assert self.selected_entity
		x, y = event.pos().x(), event.pos().y()
		self.selected_entity.place(x, y)
		self.selected_entity.update_zone_pos(x, y)
		self.mode_place_tile()
	
	
	def move_entity(self, event=None):
		"Move an entity on the scene"
		
		# first click = select entity, 2nd click = place entity
		if self.on_click != self.move_entity:
			self.on_click = self.move_entity
			self.set_cursor_visible(False)
			self.setCursor(QCursor(Qt.OpenHandCursor))
		elif self.selected_entity:
			self.mode_place_entity()
	
	
	def delete_entity(self, event=None):
		"Supprimer une unité au clic"
		
		if self.on_click != self.delete_entity:
			# first call
			self.on_click = self.delete_entity
			self.set_cursor_visible(False)
			self.setCursor(QCursor(Qt.PointingHandCursor))
		else:
			if self.selected_entity:
				entity = self.selected_entity
				# remove from the zone (model)
				zone = self.get_current_zone()
				if isinstance(entity, Map.Unit):
					zone.remove_unit(entity.get_zone_element())
					print "unit deleted"
				elif isinstance(entity, Map.Decor):
					zone.remove_decor(entity.get_zone_element())
					print "decor deleted"
				elif isinstance(entity, Map.Item):
					zone.remove_item(entity.get_zone_element())
					print "item deleted"
				else:
					raise Exception("Can't remove unknow entity")
					
				# remove from the scene (view)
				self.scene.removeItem(entity)
				self.entities.remove(entity)
			
			# back to "put tile" mode on click
			self.mode_place_tile()
	
	
	def add_unit(self, zone_unit):
		"Ajouter une unité dans la scène"
		
		unit = Map.Unit(zone_unit)
		self.scene.addItem(unit)
		unit.place(zone_unit.x, zone_unit.y)
		self.entities.append(unit)
		return unit
	
	
	def add_decor(self, zone_decor):
		"Ajouter un décor dans la scène"
		
		decor = Map.Decor(zone_decor)
		self.scene.addItem(decor)
		decor.place(zone_decor.x * TiledCanvas.TILESIZE,
			(zone_decor.y + 1) * TiledCanvas.TILESIZE)
		self.entities.append(decor)
		return decor
	
	def add_item(self, zone_item):
		
		item = Map.Item(zone_item)
		self.scene.addItem(item)
		item.place(zone_item.x, zone_item.y)
		self.entities.append(item)
		return item
	
	def clear_units(self):
		"Supprimer toutes les entités de la scène"
		
		for entity in self.entities:
			self.scene.removeItem(entity)
		
		del self.entities[:]
	
	
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
	
	
