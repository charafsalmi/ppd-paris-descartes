# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from AbstractSelectionPanel import *
from EntityFactory import *

class UnitSelectionPanel(AbstractSelectionPanel):
	def __init__(self, parent):
		AbstractSelectionPanel.__init__(self, parent)

		self.set_tree_headers("ID", "Nom", "HP", "Vitesse")
		# populate tree from factory
		for unit_id, unit in EntityFactory().get_units().iteritems():
			self.populate_tree(unit_id, unit.name, unit.hp, unit.speed)
		
	def on_click(self, current, previous):
		self.set_preview(current.text(1), None)
		
	def on_double_click(self, item, column):
		self.parent.add_unit(int(item.text(0)))
		
