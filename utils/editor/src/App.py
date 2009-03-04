# -*- coding: utf-8 -*-

import Tkinter as Tk
import tkFileDialog
import tkMessageBox
from PIL import Image, ImageTk

from PopUp import *
from Entity import *
from Teleporter import *

import xml.dom.minidom as dom
import sys

SHEET_WIDTH = 16
SHEET_HEIGHT = 20

ZONE_WIDTH = 20
ZONE_HEIGHT = 16

TILE_SIZE = 32
TILESET = "../../bin/data/images/tileset.png"

DEFAULT_PATH = "../../bin/data/map/"


class EntityDialog(PopUp):
	"Interface de la gestion des entités"
	
	def __init__(self, entities):
		PopUp.__init__(self, "Gestion des entités")
		self.save = False
		self.entities = []
		self.current = -1
		for entity in entities:
			self.add_entity(entity)
		
	def body(self, master):
		
		frame_left = Tk.LabelFrame(master, text="Entités", padx=5, pady=5)
		frame_left.pack(side=Tk.LEFT)
		
		# listbox des entités existantes
		self.lbx_entity = Tk.Listbox(frame_left, width=50, height=10, bg="white")
		self.lbx_entity.pack(side=Tk.LEFT)
		
		# barre de défilement verticale à droite du listbox
		scb_entity = Tk.Scrollbar(frame_left, command=self.lbx_entity.yview, orient=Tk.VERTICAL)
		scb_entity.pack(expand=1, fill=Tk.BOTH)
		self.lbx_entity.configure(yscrollcommand=scb_entity.set)
		
		frame_right = Tk.Frame(master, padx=5, pady=5)
		frame_right.pack(side=Tk.RIGHT)
		
		self.btn_add = Tk.Button(frame_right, text="Ajouter")#, command=self.ask_entity)
		self.btn_add.pack(fill=Tk.X)
		self.btn_edit = Tk.Button(frame_right, text="Modifier")
		self.btn_edit.pack(fill=Tk.X)
		self.btn_delete = Tk.Button(frame_right, text="Supprimer")#, command=self.delete_entity)
		self.btn_delete.pack(fill=Tk.X)
		self.btn_del_all = Tk.Button(frame_right, text="Supprimer tous")#, command=self.delete_all)
		self.btn_del_all.pack(fill=Tk.X)
		
	def ask_entity(self):
		"saisir une entité pour l'ajouter"
		
		entity = AskEntityDialog(self, "add").get_entity()
		if entity:
			self.add_entity(entity)	
	
	def add_entity(self, entity):
		"ajouter d'une entité dans la liste et la listbox"
		
		self.lbx_entity.insert(self.current, str(entity))
		self.lbx_entity.select_set(self.current)
		self.lbx_entity.select_anchor(self.current)
		
		self.entities.insert(self.current, entity)
		self.current += 1
	
	def delete_entity(self):
		"supprimer l'entité sélectionnée"
		
		self.lbx_entity.delete(self.current)
		self.lbx_entity.select_set(self.current)
		self.lbx_entity.select_anchor(self.current)

		self.current -= 1
		print "current:", self.current
		
	def delete_all(self):
		"supprimer toutes les entités de la liste"
		
		if self.current != -1:
			self.lbx_entity.delete(0, Tk.END)
			self.current = -1

	def get_current(self):
		"retourne l'entité sélectionnée si elle existe"
		
		return self.entities[self.current] if self.current != -1 else None

	def get_result(self):
		"retourne toutes les entités si validation"
		
		return self.entities if self.save else None

class AskEntityDialog(PopUp):
	
	def __init__(self, master, mode):
		title = None
		if mode == "add":
			pass
		elif mode == "edit":
			pass
		else:
			raise Exception("bad mode")
		self.entity = {}
		PopUp.__init__(self, master)
		
		
	def body(self, master):
		"construction des widgets"
		
		Tk.Label(master, text="Identifiant : ").grid(row=0)
		Tk.Label(master, text="Position X :").grid(row=1)
		Tk.Label(master, text="Position Y :").grid(row=2)
		
		self.ent_id = Tk.Entry(master)
		self.ent_id.grid(row=0, column=1)
		self.ent_posx = Tk.Entry(master)
		self.ent_posx.grid(row=1, column=1)
		self.ent_posy = Tk.Entry(master)
		self.ent_posy.grid(row=2, column=1)
		

	def validate(self):
		"contrôle des valeurs entrées"
		
		self.entity.clear()
		success = False
		try:
			id_value = self.ent_id.get()
			if len(id_value) == 0:
				raise ValueError
			self.entity["id"] = id_value
			self.entity["x"] = int(self.ent_posx.get())
			self.entity["y"] = int(self.ent_posx.get())
			success = True
		except ValueError:
			tkMessageBox.showwarning("Erreur", "Données invalides")
			self.entity.clear()
			
		return success
	
	def get_entity(self):
		return self.entity


class TiledCanvas(Tk.Canvas):
	def __init__(self, master, **kwargs):
		Tk.Canvas.__init__(self, master, **kwargs)
		
		#curseur
		self.cursor = self.create_rectangle(0, 0, TILE_SIZE, TILE_SIZE,
			outline="red", width=1)
		self.bind("<Motion>", self.place_cursor)
		
	def place_cursor(self, event):
		"positionne le curseur sur la tile sous la souris"
		
		x = (event.x / TILE_SIZE) * TILE_SIZE
		y = (event.y / TILE_SIZE) * TILE_SIZE
		self.coords(self.cursor, x - 1, y - 1, x + TILE_SIZE, y + TILE_SIZE)
		self.lift(self.cursor)


class Zone:
	def __init__(self, xml_elem):
		# données d'une zone
		self.entities = []
		self.items = []
		self.id_tiles = []
		self.teleporters = []
		
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
			for element in all_entities:
				name = element.getAttribute("name")
				x = int(element.getAttribute("x"))
				y = int(element.getAttribute("y"))
				self.entities.append(Entity(name, x, y))
	
		# parsing teleporters
		try:
			xml_teleporters = xml_elem.getElementsByTagName("teleporters")[0].getElementsByTagName("tp")
		except IndexError:
			pass # pas de tp
		else:
			for xml_tp in xml_teleporters:
				self.teleporters.append(Teleporter(xml_tp))
		
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
		
		# écriture des entités
		if len(self.entities) > 0:
			f.write("\t<entities>\n")
			for entity in self.entities:
				f.write("\t\t%s\n" % entity.to_xml())
			f.write("\t</entities>\n")
		
		# écriture des items
		# TODO
		
		# écriture des téléporteurs
		if len(self.teleporters) > 0:
			f.write("\t<teleporters>\n")
			for tp in self.teleporters:
				f.write("\t\t%s\n" % tp.to_xml())
			f.write("\t</teleporters>\n")
		
		f.write("</zone>\n")


class App(Tk.Tk):
	def __init__(self):
		Tk.Tk.__init__(self)
		self.title("Editor")
		
		# création de la barre de menu
		menubar = Tk.Menu(self)
		
		# menu fichier
		m_file = Tk.Menu(menubar, tearoff=False)
		m_file.add_command(label="Ouvrir une carte", command=self.open_map)
		m_file.add_command(label="Enregistrer", command=self.save_map)
		m_file.add_command(label="Enregistrer sous ...", command=self.save_map_as)
		m_file.add_separator()
		m_file.add_command(label="Quitter", command=self.destroy)
		menubar.add_cascade(label="Fichier", menu=m_file)
		
		# menu édition
		m_edit = Tk.Menu(menubar, tearoff=False)
		m_edit.add_command(label="Annuler (U)", command=self.undo)
		self.bind("<u>", self.undo)
		m_edit.add_command(label="Peindre avec la tile courante", command=self.paint_all)
		m_edit.add_command(label="Gestion des entités", command=self.set_entities)
		menubar.add_cascade(label="Édition", menu=m_edit)
		
		#menu navigation
		m_navig = Tk.Menu(menubar, tearoff=False)
		m_navig.add_command(label="Nord (Flèche Haut)", command=lambda: self.change_zone(0, -1))
		m_navig.add_command(label="Sud (Flèche Bas)", command=lambda: self.change_zone(0, 1))
		m_navig.add_command(label="Ouest (Flèche Gauche)", command=lambda: self.change_zone(-1, 0))
		m_navig.add_command(label="Est (Flèche Droite)", command=lambda: self.change_zone(1, 0))
		self.bind("<Up>", lambda event: self.change_zone(0, -1))
		self.bind("<Down>", lambda event: self.change_zone(0, 1))
		self.bind("<Left>", lambda event: self.change_zone(-1, 0))
		self.bind("<Right>", lambda event: self.change_zone(1, 0))
		menubar.add_cascade(label="Naviation", menu=m_navig)
		
		self.config(menu=menubar)
		
		tilesheet = Image.open(TILESET)
		# découpage de la feuille en une liste de tiles
		self.img_tiles = []
		for i in xrange(SHEET_WIDTH * SHEET_HEIGHT):
			left = (i % SHEET_WIDTH) * TILE_SIZE
			top = (i / SHEET_WIDTH) * TILE_SIZE
			
			tile = tilesheet.crop((left, top, left + TILE_SIZE, top + TILE_SIZE))
			# conversion en PhotoImage
			self.img_tiles.append(ImageTk.PhotoImage(tile))
		
		# canevas de la zone de sélection
		frame_left = Tk.Frame(self)
		frame_left.pack(side=Tk.LEFT)
		self.can_select = TiledCanvas(frame_left, width=SHEET_WIDTH * TILE_SIZE,
			height=SHEET_HEIGHT * TILE_SIZE)
		self.can_select.pack()
		self.can_select.bind("<ButtonPress-1>", self.set_current)
		
		self.img = ImageTk.PhotoImage(tilesheet)
		self.can_select.create_image(0, 0, image=self.img, anchor=Tk.NW)
		
		# labels d'information
		Tk.Label(frame_left, text="tile courante : ").pack(side=Tk.LEFT)
		self.lab_tile = Tk.Label(frame_left)
		self.lab_tile.pack(side=Tk.LEFT)
		self.ent_tile_id = Tk.Entry(frame_left, state=Tk.DISABLED)
		self.ent_tile_id.pack(side=Tk.LEFT, expand=1, anchor=Tk.W)
		
		self.lab_zone = Tk.Label(frame_left, text="zone courante : ")
		self.lab_zone.pack()
		
		# canevas de la zone d'édition
		frame_right = Tk.Frame(self)
		frame_right.pack()
		self.can = TiledCanvas(frame_right, width=ZONE_WIDTH * TILE_SIZE,
			height=ZONE_HEIGHT * TILE_SIZE)
		self.can.pack()
		self.can.bind("<ButtonPress-1>", self.put_tile)
		self.can.bind("<B1-Motion>", self.put_tile)
		
		
		
		# création d'une carte par défaut
		self.history = [] # pile de l'historique
		self.zones = []
		self.zones.append([])
		self.width = 0
		self.height = 0
		self.current_tile = 0 # indice de la tile courante
		self.current_zone = 0
		
		self.lab_tile["image"] = self.img_tiles[self.current_tile]
		
		# filename de la carte
		self.filename = None
		
	def open_map(self):
		"charger une carte dans l'éditeur"
		
		filename = tkFileDialog.askopenfilename(initialdir=DEFAULT_PATH,
			filetypes=[("Documents XML", ".xml")])
		if filename:
			try:
				doc = dom.parse(filename)
			except Exception, what:
				tkMessageBox.showwarning("Erreur", what)
				return
			
			# clean zone data
			del self.history[:]
			del self.zones[:]
			
			map_element = doc.getElementsByTagName("map")[0]
			self.width = int(map_element.getAttribute("width"))
			self.height = int(map_element.getAttribute("height"))
			
			zone_elements = doc.getElementsByTagName("zone")
			if len(zone_elements) != self.width * self.height:
				tkMessageBox.showwarning("Erreur", "zone(s) manquante(s)")
				sys.exit()
			
			count = 0
			for elem in zone_elements:
				i = count / self.width
				j = count % self.width
				self.zones.append(Zone(elem))
				print "current zone: [%d][%d]" % (i, j)
				
				count += 1
			
			self.title("Editor - " + filename)
			self.filename = filename
			
			# on affiche la zone courante
			self.set_current_zone(0)
			
			print "> open", filename
			
	def save_map(self):
		"sauvegarder la carte de l'éditeur"
		
		if not self.filename:
			self.save_map_as()
		else:
			f = open(self.filename, "w")
			
			f.write('<?xml version="1.0" encoding="utf-8" ?>\n')
			f.write('<map width="%d" height="%d">\n' % (self.width, self.height))
			
			for zone in self.zones:
				zone.save(f)
			f.write('</map>')
			f.close()
			
			print "> save", self.filename
			
	def save_map_as(self):
		"demander où sauvegarder la carte"
		
		f = tkFileDialog.asksaveasfilename()
		if f:
			self.filename = f
			self.save_map()
		else:
			self.filename = None # car f est un tuple vide
	
	def draw_tile(self, position, tile_id):
		"""
		dessiner une tile sur le canevas
		position: ordre de la tile
		tile_id: identifiant
		"""
		
		self.can.create_image(
			(position % ZONE_WIDTH) * TILE_SIZE,
			(position / ZONE_WIDTH) * TILE_SIZE,
			anchor=Tk.NW, image=self.img_tiles[tile_id]
		)
		
	def set_current(self, event):
		"définir la tile courante"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		tile_id = y * SHEET_WIDTH + x
		if tile_id != self.current_tile:
			self.current_tile = tile_id
			self.lab_tile["image"] = self.img_tiles[tile_id]
			self.ent_tile_id["state"] = Tk.NORMAL
			self.ent_tile_id.delete(0, Tk.END)
			self.ent_tile_id.insert(0, str(tile_id))
			self.ent_tile_id["state"] = "readonly"
	
	def put_tile(self, event):
		"placer une tile sur la carte"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		position = y * ZONE_WIDTH + x
		
		# si la nouvelle tile est différente de l'ancienne
		old_tile = self.zones[self.current_zone].get_tile_at(position)
		if x < ZONE_WIDTH and y < ZONE_HEIGHT and old_tile != self.current_tile:
			self.can.create_image(x * TILE_SIZE,
				y * TILE_SIZE, image=self.img_tiles[self.current_tile], anchor=Tk.NW)
			
			# ajout du placement dans la pile de l'historique
			self.history.append((old_tile, position))
			self.zones[self.current_zone].put_tile_at(position, self.current_tile)
		self.can.place_cursor(event)
	
	def undo(self, event=None):
		"annuler la dernière action"
		
		if len(self.history) > 0:
			tile, indice = self.history.pop()
			self.can.create_image((indice % ZONE_WIDTH) * TILE_SIZE,
				(indice / ZONE_WIDTH) * TILE_SIZE,
				anchor=Tk.NW, image=self.img_tiles[tile])
			self.zones[self.current_zone].put_tile_at(indice, tile)
		else:
			print "> l'historique est vide !"
	
	def paint_all(self):
		"peindre tout la carte avec la tile courante"
		
		self.can.delete(Tk.ALL)
		self.zones[self.current_zone].fill_tiles(self.current_tile)
		self.zones[self.current_zone].draw(self)
	
	def set_current_zone(self, zone_position):
		
		self.current_zone = zone_position # indice de la zone courante
		self.zones[zone_position].draw(self)
		i = zone_position / self.width
		j = zone_position % self.width
		self.lab_zone["text"] = "zone courante : [%d][%d]" % (i, j)
	
	def change_zone(self, dx, dy):
		y = (self.current_zone / self.width) + dy
		x = (self.current_zone % self.width) + dx
		if 0 <= y < self.height and 0 <= x < self.width:
			self.set_current_zone(y * self.width + x)
		else:
			tkMessageBox.showwarning("Stop", "Il n'y a plus rien de coté là !")
		
		
	def set_entities(self):
		"interface pour ajouter/modifier/supprimer des entités"
		
		EntityDialog(self.entities)
		# to be continued
		
