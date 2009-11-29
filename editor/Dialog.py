# -*- coding: utf-8 -*-

import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from EntityFactory import EntityFactory
from SelectFile import SelectFile

class ConfigDialog(QDialog):
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Configuration")
		
		vbox = QVBoxLayout()
		self.setLayout(vbox)
		
		groupbox = QGroupBox(u"Paramètres", self)
		
		self.file_tileset = SelectFile()
		self.file_units = SelectFile()
		self.file_decors = SelectFile()
		self.file_items = SelectFile()
		self.file_music = SelectFile()
		self.spin_tilesize = QSpinBox()
		self.spin_tilesize.setRange(1, 100)
		self.spin_tilesize.setFixedWidth(50)
		
		form = FormLayout()

		form.add_row(u"Image du tileset :", self.file_tileset)
		form.add_row(u"Définition des unités :", self.file_units)
		form.add_row(u"Définition des décors :", self.file_decors)
		form.add_row(u"Définition des items :", self.file_items)
		form.add_row(u"Liste des musiques :", self.file_music)
		form.add_row(u"Résolution tile (pixels) :", self.spin_tilesize)
		
		groupbox.setLayout(form)
		vbox.addWidget(groupbox)
		
		# buttons
		but_cancel = QPushButton("Annuler")
		but_valid = QPushButton("Appliquer")
		self.connect(but_valid, SIGNAL("clicked()"), self.apply)
		self.connect(but_cancel, SIGNAL("clicked()"), SLOT("close()"))
		
		buttonbox = QHBoxLayout()
		buttonbox.addWidget(but_cancel)
		buttonbox.addWidget(but_valid)
		vbox.addLayout(buttonbox)
		self.valid = False
	
	def apply(self):
		self.valid = True
		self.close()
	
	def valided(self):
		return self.valid
	
	def load_config(self, config):
		self.file_tileset.set_text(config["tileset"])
		self.file_units.set_text(config["units"])
		self.file_decors.set_text(config["decors"])
		self.file_items.set_text(config["items"])
		self.file_music.set_text(config["musics"])
		self.spin_tilesize.setValue(int(config["tilesize"]))

	def write_config(self, config):
		config["tileset"] = self.file_tileset.get_text()
		config["units"] = self.file_units.get_text()
		config["decors"] = self.file_decors.get_text()
		config["items"] = self.file_items.get_text()
		config["musics"] = self.file_music.get_text()
		config["tilesize"] = self.spin_tilesize.value()
		print "configuration updated"
		self.close()
		

class FormLayout(QGridLayout):
	
	def __init__(self):
		QGridLayout.__init__(self)
		self.current_row = 0
		self.setSpacing(0)
		
		
	def add_row(self, text, widget):
		self.addWidget(QLabel(text), self.current_row, 0)
		self.addWidget(widget, self.current_row, 1, Qt.AlignRight)
		self.current_row += 1


class AskMapSize(QDialog):
	"Demander les dimensions d'une nouvelle carte"
	
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Nouvelle carte")
		
		vbox = QVBoxLayout()
		self.setLayout(vbox)
		
		# widgets
		self.txt_width = QSpinBox()
		self.txt_width.setRange(1, 100)
		
		self.txt_height = QSpinBox()
		self.txt_height.setRange(1, 100)
		
		self.txt_zonewidth = QSpinBox()
		self.txt_zonewidth.setRange(1, 20)
		
		self.txt_zoneheight = QSpinBox()
		self.txt_zoneheight.setRange(1, 20)
		
		but_cancel = QPushButton("Annuler")
		self.connect(but_cancel, SIGNAL("clicked()"), SLOT("close()"))
		
		but_valid = QPushButton("Valider")
		self.connect(but_valid, SIGNAL("clicked()"), self.apply)
		
		# form
		groupbox = QGroupBox(u"Dimensions de la carte", self)
		
		form = FormLayout()
		form.setSpacing(10)
		form.add_row(u"Nombre de zones en largeur :", self.txt_width)
		form.add_row(u"Nombre de zones en hauteur :", self.txt_height)
		form.add_row(u"Nombre de tiles par zone en hauteur :", self.txt_zonewidth)
		form.add_row(u"Nombre de tiles par zone en largeur :", self.txt_zoneheight)
		
		groupbox.setLayout(form)
		vbox.addWidget(groupbox)
		
		# buttons
		buttonbox = QHBoxLayout()
		buttonbox.addWidget(but_cancel)
		buttonbox.addWidget(but_valid)
		vbox.addLayout(buttonbox)
		
		self.valid = False
		
	def valided(self):
		return self.valid
	
	def apply(self, event=None):
		self.valid = True
		self.close()
	
	def get_width(self):
		return self.txt_width.value()
	
	def get_height(self):
		return self.txt_height.value()
	
	def get_zone_width(self):
		return self.txt_zonewidth.value()
		
	def get_zone_height(self):
		return self.txt_zoneheight.value()



