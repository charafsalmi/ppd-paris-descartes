# -*- coding: utf-8 -*-

import xml.dom.minidom as xml

from PyQt4.QtGui import *
from PyQt4.QtCore import *

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
		self.model.setHeaderData(0, Qt.Horizontal, QVariant("id"))
		self.model.setHeaderData(1, Qt.Horizontal, QVariant("name"))
		self.model.setHeaderData(2, Qt.Horizontal, QVariant("hp"))
		self.model.setHeaderData(3, Qt.Horizontal, QVariant("speed"))
		
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


class AskMapSize(QDialog):
	"Demander les dimensions d'une nouvelle carte"
	
	def __init__(self, parent=None):
		QDialog.__init__(self, parent)
		self.setWindowTitle(u"Nouvelle carte")
	
		# widgets
		
		lab_width = QLabel("Nombre de zones en largeur :")
		txt_width = QSpinBox()
		txt_width.setMinimum(1)
		txt_width.setMaximum(100)
		
		lab_height = QLabel("Nombre de zones en hauteur :")
		txt_height = QSpinBox()
		txt_height.setMinimum(1)
		txt_height.setMaximum(100)
		
		but_cancel = QPushButton("Annuler")
		self.connect(but_cancel, SIGNAL("clicked()"), SLOT("close()"))
		
		but_valid = QPushButton("Valider")
		self.connect(but_valid, SIGNAL("clicked()"), self.apply)
		
		grid = QGridLayout()
		grid.setSpacing(10)
		
		grid.addWidget(lab_width, 0, 0)
		grid.addWidget(txt_width, 0, 1)
		grid.addWidget(lab_height, 1, 0)
		grid.addWidget(txt_height, 1, 1)
		grid.addWidget(but_cancel, 2, 0)
		grid.addWidget(but_valid, 2, 1)
		
		grid.setColumnMinimumWidth(0, 100)
		grid.setColumnMinimumWidth(1, 100)
		self.setLayout(grid)
		
		self.valid = False

		self.txt_width = txt_width
		self.txt_height = txt_height
				
	def valided(self):
		return self.valid
	
	def apply(self, event=None):
		self.valid = True
		self.close()
	
	def get_width(self):
		return self.txt_width.value()
	
	def get_height(self):
		return self.txt_height.value()
	
