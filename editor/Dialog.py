# -*- coding: utf-8 -*-

import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from EntityFactory import EntityFactory
from SelectFile import SelectFile


class UnitListWindow(QDialog):
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Liste des unités")
		
		self.selected_id = -1;
		
		vbox = QVBoxLayout()
		
		self.label = QLabel()
		self.label.setText("")
		hbox = QHBoxLayout()
		hbox.addWidget(QLabel(u"Sélection : "))
		hbox.addWidget(self.label)
		
		self.treeview = QTreeView()
		
		
		but_cancel = QPushButton("Annuler")
		self.connect(but_cancel, SIGNAL("clicked()"), self.discard)
		
		but_confirm = QPushButton("Valider")
		self.connect(but_confirm, SIGNAL("clicked()"), SLOT("close()"))
		
		bbox = QHBoxLayout()
		bbox.addWidget(but_cancel)
		bbox.addWidget(but_confirm)
		
		vbox.addLayout(hbox)
		vbox.addWidget(self.treeview)
		vbox.addLayout(bbox)
		self.setLayout(vbox)
	
	def discard(self, event=None):
		self.selected_id = -1
		self.close()
		
	def fill(self, factory):
		self.model = QStandardItemModel(0, 4, self)
		self.model.setHeaderData(0, Qt.Horizontal, QVariant("ID"))
		self.model.setHeaderData(1, Qt.Horizontal, QVariant("Nom"))
		self.model.setHeaderData(2, Qt.Horizontal, QVariant("HP"))
		self.model.setHeaderData(3, Qt.Horizontal, QVariant("Vitesse"))
		
		for id, unit in factory.get_units().iteritems():
			self.model.insertRow(0)

			self.model.setData(self.model.index(0, 0), QVariant(id))
			self.model.setData(self.model.index(0, 1), QVariant(unit.name))
			self.model.setData(self.model.index(0, 2), QVariant(unit.hp))
			self.model.setData(self.model.index(0, 3), QVariant(unit.speed))
		
		self.treeview.setModel(self.model)
		self.treeview.setSortingEnabled(True)
		self.connect(self.treeview, SIGNAL("clicked(QModelIndex)"), self.what)
		
	def what(self, event):
		id = int(self.model.item(event.row(), 0).text())
		name = self.model.item(event.row(), 1).text()
		self.selected_id = id
		self.label.setText("id %d (%s)" % (id, name))
	
	def get_selected_id(self):
		return self.selected_id


class WindowListDecor(QDialog):
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Liste des décors")
		
		self.selected_id = -1;
		
		vbox = QVBoxLayout()
		
		self.label = QLabel()
		self.label.setText("")
		
		self.preview = QLabel()
		
		hbox = QHBoxLayout()
		hbox.addWidget(QLabel(u"Sélection : "))
		hbox.addWidget(self.label)
		hbox.addWidget(self.preview)
		
		self.treeview = QTreeView()
		
		but_cancel = QPushButton("Annuler")
		self.connect(but_cancel, SIGNAL("clicked()"), self.discard)
		
		but_confirm = QPushButton("Valider")
		self.connect(but_confirm, SIGNAL("clicked()"), SLOT("close()"))
		
		bbox = QHBoxLayout()
		bbox.addWidget(but_cancel)
		bbox.addWidget(but_confirm)
		
		vbox.addLayout(hbox)
		vbox.addWidget(self.treeview)
		vbox.addLayout(bbox)
		self.setLayout(vbox)
	
	def discard(self, event=None):
		self.selected_id = -1
		self.close()
		
	def fill(self, factory):
		self.model = QStandardItemModel(0, 4, self)
		self.model.setHeaderData(0, Qt.Horizontal, QVariant("ID"))
		self.model.setHeaderData(1, Qt.Horizontal, QVariant("Nom"))
		self.model.setHeaderData(2, Qt.Horizontal, QVariant("Largeur"))
		self.model.setHeaderData(3, Qt.Horizontal, QVariant("Hauteur"))
		
		for id, decor in factory.get_decors().iteritems():
			self.model.insertRow(0)
			self.model.setData(self.model.index(0, 0), QVariant(id))
			self.model.setData(self.model.index(0, 1), QVariant(decor.name))
			self.model.setData(self.model.index(0, 2), QVariant(decor.width))
			self.model.setData(self.model.index(0, 3), QVariant(decor.height))
		
		self.treeview.setModel(self.model)
		self.treeview.setSortingEnabled(True)
		self.connect(self.treeview, SIGNAL("clicked(QModelIndex)"), self.what)
		
	def what(self, event):
		id = int(self.model.item(event.row(), 0).text())
		name = self.model.item(event.row(), 1).text()
		self.selected_id = id
		self.label.setText("id %d (%s)" % (id, name))
	
		self.preview.setPixmap(QPixmap.fromImage(EntityFactory().get_decor_by_id(id).sprite))
		
	def get_selected_id(self):
		return self.selected_id

		
class WindowListItem(QDialog):
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Liste des items")
		
		self.selected_id = -1;
		
		vbox = QVBoxLayout()
		
		self.label = QLabel()
		self.label.setText("")
		
		self.preview = QLabel()
		
		hbox = QHBoxLayout()
		hbox.addWidget(QLabel(u"Sélection : "))
		hbox.addWidget(self.label)
		hbox.addWidget(self.preview)
		
		self.treeview = QTreeView()
		
		but_cancel = QPushButton("Annuler")
		self.connect(but_cancel, SIGNAL("clicked()"), self.discard)
		
		but_confirm = QPushButton("Valider")
		self.connect(but_confirm, SIGNAL("clicked()"), SLOT("close()"))
		
		bbox = QHBoxLayout()
		bbox.addWidget(but_cancel)
		bbox.addWidget(but_confirm)
		
		vbox.addLayout(hbox)
		vbox.addWidget(self.treeview)
		vbox.addLayout(bbox)
		self.setLayout(vbox)
	
	def discard(self, event=None):
		self.selected_id = -1
		self.close()
		
	def fill(self, factory):
		self.model = QStandardItemModel(0, 2, self)
		self.model.setHeaderData(0, Qt.Horizontal, QVariant(u"Nom"))
		self.model.setHeaderData(1, Qt.Horizontal, QVariant(u"Libellé"))

		
		for name, item in factory.get_items().iteritems():
			self.model.insertRow(0)
			self.model.setData(self.model.index(0, 0), QVariant(name))
			self.model.setData(self.model.index(0, 1), QVariant(item.label))
		
		self.treeview.setModel(self.model)
		self.treeview.setSortingEnabled(True)
		self.connect(self.treeview, SIGNAL("clicked(QModelIndex)"), self.what)
		
	def what(self, event):
		name = str(self.model.item(event.row(), 0).text())
		self.selected_id = name
		self.label.setText(name)
		self.preview.setPixmap(QPixmap.fromImage(EntityFactory().get_item_by_name(name).sprite))
		
	def get_selected_id(self):
		return self.selected_id
		

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
	


