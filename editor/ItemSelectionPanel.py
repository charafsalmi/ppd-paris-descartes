# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from AbstractSelectionPanel import *
from EntityFactory import *

class ItemSelectionPanel(AbstractSelectionPanel):
	def __init__(self, parent):
		AbstractSelectionPanel.__init__(self, parent)

		self.set_tree_headers("Nom ID", u"Libellé")
		# populate tree from factory
		for item_id, item in EntityFactory().get_items().iteritems():
			self.populate_tree(item_id, item.label)
		
	def on_click(self, current, previous):
		name = str(current.text(0))
		image = EntityFactory().get_item_by_name(name).sprite
		self.set_preview(name, image)
	
	def on_double_click(self, item, column):
		self.parent.add_item(str(item.text(0)))
		
