# -*- coding: utf-8 -*-

from PyQt4.QtGui import *
from PyQt4.QtCore import *

class AbstractSelectionPanel(QFrame):
	def __init__(self, parent):
		QFrame.__init__(self)
		self.parent = parent
		
		layout = QVBoxLayout()
		
		self.tree = QTreeWidget()
		layout.addWidget(self.tree)
		
		box_preview = QHBoxLayout()
		box_preview.addWidget(QLabel(u"Sélection : "))
		self.preview_text = QLabel("Aucune")
		box_preview.addWidget(self.preview_text)
		self.preview_pixmap = QLabel()
		box_preview.addWidget(self.preview_pixmap)
		
		layout.addLayout(box_preview)
		
		self.setLayout(layout)
		
		self.tree.setSortingEnabled(True)
		self.tree.header().setResizeMode(QHeaderView.Stretch)
		self.connect(self.tree, SIGNAL("currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)"), self.on_click)
		self.connect(self.tree, SIGNAL("itemDoubleClicked (QTreeWidgetItem*, int)"), self.on_double_click)
		
	def set_tree_headers(self, *headers):
		self.tree.setHeaderLabels(list(headers))
		
	def populate_tree(self, *values):
		"Ajouter un nouvel élément dans l'arbre de sélection"
		
		tree_item = QTreeWidgetItem(None)
		for index, text in enumerate(values):
			tree_item.setText(index, "%s" % text)
		
		self.tree.addTopLevelItem(tree_item)
	
	def on_click(self, current, previous):
		raise NotImplementedError("AbstractSelectionPanel.on_click must be overidden")
		
	def on_double_click(self, item, column):
		pass
	
	def set_preview(self, text, image):
		self.preview_text.setText("%s" % text)
		if image:
			self.preview_pixmap.setPixmap(QPixmap.fromImage(image))


