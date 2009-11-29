# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from AbstractSelectionPanel import *
from EntityFactory import *

class DecorSelectionPanel(AbstractSelectionPanel):
	def __init__(self, parent):
		AbstractSelectionPanel.__init__(self, parent)
		
		self.set_tree_headers("ID", "Nom", "Tiles en largeur", "Tiles en hauteur")
		# populate tree from factory
		for decor_id, decor in EntityFactory().get_decors().iteritems():
			self.populate_tree(decor_id, decor.name, decor.width, decor.height)
	
	def on_click(self, current, previous):
		id = int(current.text(0))
		image = EntityFactory().get_decor_by_id(id).sprite
		self.set_preview(current.text(1), image)
	
	def on_double_click(self, item, column):
		self.parent.add_decor(int(item.text(0)))
