# -*- coding: utf-8 -*-

import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *


class SelectFile(QFrame):

	def __init__(self):
		QFrame.__init__(self)
		
		layout = QHBoxLayout()
		
		self.line = QLineEdit()
		self.line.setFixedWidth(200)
		layout.addWidget(self.line)
		
		but_browse = QPushButton("Parcourir")
		layout.addWidget(but_browse)
		self.connect(but_browse, SIGNAL("clicked()"), self.on_select)
		
		self.setLayout(layout)

	
	def on_select(self):
		# QString to str
		filename = str(QFileDialog.getOpenFileName(self, u"Sélectionner",
			os.path.dirname(self.get_text()),
			"Tous (*);;"))
		if filename != "":
			self.line.setText(filename)
	
	
	def set_text(self, string):
		self.line.setText(string.strip())
	
	
	def get_text(self):
		return str(self.line.text()).strip()
	
