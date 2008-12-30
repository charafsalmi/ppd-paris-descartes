#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Sur debian/ubuntu :
#$ sudo apt-get install python-tk python-imaging-tk
#$ cd ppd-paris-descartes/editor/
#$ python editor.py

import Tkinter as Tk
import tkFileDialog
from PIL import Image, ImageTk

SHEET_WIDTH = 8
SHEET_HEIGHT = 8

ZONE_WIDTH = 20
ZONE_HEIGHT = 16

TILE_SIZE = 32
TILESET = "../data/images/tileset.png"


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
		
		# création du menu
		menubar = Tk.Menu(self)
		m_file = Tk.Menu(menubar, tearoff=False)
		m_file.add_command(label="Ouvrir une carte", command=self.open_map)
		m_file.add_command(label="Enregistrer la carte", command=self.save_map)
		m_file.add_separator()
		m_file.add_command(label="Quitter", command=self.destroy)
		menubar.add_cascade(label="Fichier", menu=m_file)
		
		m_edit = Tk.Menu(menubar, tearoff=False)
		m_edit.add_command(label="Annuler (U)", command=self.undo)
		self.bind("<u>", self.undo)
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
		self.lab_tile.pack()
		
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
		for i in xrange(ZONE_WIDTH * ZONE_HEIGHT):
			self.map.append(0)
			self.can.create_image((i % ZONE_WIDTH) * TILE_SIZE,
					(i / ZONE_WIDTH) * TILE_SIZE,
					anchor=Tk.NW, image=self.tiles[self.map[-1]])
		self.current = 0 # indice de la tile courante
		self.lab_tile["image"] = self.tiles[self.current]
		
	def open_map(self):
		"charger une carte dans l'éditeur"
		
		filename = tkFileDialog.askopenfilename(initialdir="../data/map")
		if filename:
			self.history = []
			self.map = []
			content = open(filename).read().split()
			
			for i in xrange(ZONE_WIDTH * ZONE_HEIGHT):
				self.map.append(int(content[i]))
				
				self.can.create_image((i % ZONE_WIDTH) * TILE_SIZE,
					(i / ZONE_WIDTH) * TILE_SIZE,
					anchor=Tk.NW, image=self.tiles[self.map[-1]])
	
	def save_map(self):
		"sauvegarder la carte de l'éditeur dans un fichier"
		
		f = tkFileDialog.asksaveasfile()
		if f:
			for i, tile in enumerate(self.map):
				f.write("%2d " % tile)
				if (i + 1) % ZONE_WIDTH == 0:
					f.write("\n")
			f.close()
	
	def set_current(self, event):
		"définir la tile courante"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		tile_id = y * SHEET_WIDTH + x
		self.current = tile_id
		self.lab_tile["image"] = self.tiles[tile_id]
		
	def put_tile(self, event):
		"placer une tile sur la carte"
		
		x = event.x / TILE_SIZE
		y = event.y / TILE_SIZE
		# si la nouvelle tile est différente de l'ancienne
		indice = y * ZONE_WIDTH + x
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
			print "l'historique est vide !"
	
app = App()
app.mainloop()


