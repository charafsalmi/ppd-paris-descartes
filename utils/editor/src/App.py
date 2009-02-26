# -*- coding: utf-8 -*-

import Tkinter as Tk
import tkFileDialog
import tkMessageBox
from PIL import Image, ImageTk

from PopUp import *
from Entity import *

import xml.dom.minidom as dom

SHEET_WIDTH = 16
SHEET_HEIGHT = 20

ZONE_WIDTH = 20
ZONE_HEIGHT = 16

TILE_SIZE = 32
TILESET = "../../data/images/tileset.png"

DEFAULT_PATH = "../../data/map/"

		
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
		
		self.config(menu=menubar)
		
		tilesheet = Image.open(TILESET)
		# découpage de la feuille en une liste de tiles
		self.tiles = []
		for i in xrange(SHEET_WIDTH * SHEET_HEIGHT):
			left = (i % SHEET_WIDTH) * TILE_SIZE
			top = (i / SHEET_WIDTH) * TILE_SIZE
			
			tile = tilesheet.crop((left, top, left + TILE_SIZE, top + TILE_SIZE))
			# conversion en PhotoImage
			self.tiles.append(ImageTk.PhotoImage(tile))
		
		# canevas de la zone de sélection
		frame_left = Tk.Frame(self)
		frame_left.pack(side=Tk.LEFT)
		self.can_select = TiledCanvas(frame_left, width=SHEET_WIDTH * TILE_SIZE,
			height=SHEET_HEIGHT * TILE_SIZE)
		self.can_select.pack()
		self.can_select.bind("<ButtonPress-1>", self.set_current)
		
		self.img = ImageTk.PhotoImage(tilesheet)
		self.can_select.create_image(0, 0, image=self.img, anchor=Tk.NW)
		Tk.Label(frame_left, text="tile courante : ").pack(side=Tk.LEFT)
		self.lab_tile = Tk.Label(frame_left)
		self.lab_tile.pack(side=Tk.LEFT)
		self.ent_tile_id = Tk.Entry(frame_left, state=Tk.DISABLED)
		self.ent_tile_id.pack(expand=1, anchor=Tk.W)
		
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
		self.map = []
		self.current = 0 # indice de la tile courante
		self.paint_all()
		self.lab_tile["image"] = self.tiles[self.current]
		
		# filename de la carte
		self.filename = None
		
		# entités de la carte
		self.entities = []
		
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
			del self.map[:]
			del self.entities[:]
			
			# parsing tiles
			all_tiles = doc.getElementsByTagName("tiles")[0].firstChild.data.split()
			
			for i in xrange(ZONE_WIDTH * ZONE_HEIGHT):
				self.map.append(int(all_tiles[i]))
				
				self.can.create_image((i % ZONE_WIDTH) * TILE_SIZE,
					(i / ZONE_WIDTH) * TILE_SIZE,
					anchor=Tk.NW, image=self.tiles[self.map[-1]])
			
			# parsing entities
			all_entities = doc.getElementsByTagName("entities")[0].getElementsByTagName("entity")
			for element in all_entities:
				name = element.getAttribute("name")
				x = int(element.getAttribute("x"))
				y = int(element.getAttribute("y"))
				self.entities.append(Entity(name, x, y))
				
			self.title("Editor - " + filename)
			self.filename = filename
			print "> open", filename
			
	def save_map(self):
		"sauvegarder la carte de l'éditeur"
		
		if not self.filename:
			self.save_map_as()
		else:
			f = open(self.filename, "w")
			# écriture du XML à la main
			f.write('<?xml version="1.0" encoding="utf-8" ?>\n')
			f.write("<zone>\n")
			
			# écriture des tiles
			f.write("\t<tiles>\n")
			for i, tile in enumerate(self.map):
				f.write("%3d " % tile)
				if (i + 1) % ZONE_WIDTH == 0:
					f.write("\n")
			f.write("\t</tiles>\n")
			
			# écriture des entités
			f.write("\t<entities>\n")
			for entity in self.entities:
				f.write("\t\t%s\n" % entity.to_xml())
			f.write("\t</entities>\n")
			
			f.write("</zone>\n")
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
	
	def set_current(self, event):
		"définir la tile courante"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		tile_id = y * SHEET_WIDTH + x
		if tile_id != self.current:
			self.current = tile_id
			self.lab_tile["image"] = self.tiles[tile_id]
			self.ent_tile_id["state"] = Tk.NORMAL
			self.ent_tile_id.delete(0, Tk.END)
			self.ent_tile_id.insert(0, str(tile_id))
			self.ent_tile_id["state"] = "readonly"
	
	def put_tile(self, event):
		"placer une tile sur la carte"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		indice = y * ZONE_WIDTH + x
		
		# si la nouvelle tile est différente de l'ancienne
		if x < ZONE_WIDTH and y < ZONE_HEIGHT and self.map[indice] != self.current:
			self.can.create_image(x * TILE_SIZE,
				y * TILE_SIZE, image=self.tiles[self.current], anchor=Tk.NW)
			
			# ajout du placement dans la pile de l'historique
			self.history.append((self.map[indice], indice))
			self.map[indice] = self.current
		self.can.place_cursor(event)
	
	def undo(self, event=None):
		"annuler la dernière action"
		
		if len(self.history) > 0:
			tile, indice = self.history.pop()
			self.can.create_image((indice % ZONE_WIDTH) * TILE_SIZE,
				(indice / ZONE_WIDTH) * TILE_SIZE,
				anchor=Tk.NW, image=self.tiles[tile])
			self.map[indice] = tile
		else:
			print "> l'historique est vide !"
	
	def paint_all(self):
		"peindre tout la carte avec la tile courante"
		
		del self.map[:]
		self.can.delete(Tk.ALL)
		for i in xrange(ZONE_WIDTH * ZONE_HEIGHT):
			self.map.append(self.current)
			self.can.create_image((i % ZONE_WIDTH) * TILE_SIZE,
					(i / ZONE_WIDTH) * TILE_SIZE,
					anchor=Tk.NW, image=self.tiles[self.current])
	
	def set_entities(self):
		"interface pour ajouter/modifier/supprimer des entités"
		
		EntityDialog(self.entities)
		# to be continued
		