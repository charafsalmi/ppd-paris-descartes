# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from EntityFactory import *

class UnitTreeWidget(QTreeWidget):
	def __init__(self, parent):
		QTreeWidget.__init__(self)
		self.parent = parent
		self.setHeaderLabels(["ID", "Nom", "HP", "Vitesse"])
		
		# populate tree from factory
		for unit_id, unit in EntityFactory().get_units().iteritems():
			item = QTreeWidgetItem(None)
			item.setText(0, str(unit_id))
			item.setText(1, unit.name)
			item.setText(2, str(unit.hp))
			item.setText(3, str(unit.speed))
			
			self.addTopLevelItem(item)
		
		self.setSortingEnabled(True)
		#self.connect(self, SIGNAL("clicked(QModelIndex)"), self.callback)
		self.connect(self, SIGNAL("itemClicked(QTreeWidgetItem*, int)"), self.callback)
		
	def callback(self, item, column):
		self.parent.add_unit_from_tree(int(item.text(0)))
		
