#!/usr/bin/python
# -*- coding: utf-8 -*-

# author: Alexandre Bodelot <alexandre.bodelot@gmail.com>

import sys
import os

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from TiledCanvas import TiledCanvas
from Tileset import Tileset
from Map import Map
from FrameInfo import FrameInfo
from EntityFactory import EntityFactory
import Dialog
from Config import Config

from UnitSelectionPanel import *
from ItemSelectionPanel import *
from DecorSelectionPanel import *


class MainWindow(QMainWindow):

	CONFIG_FILE = "config/config.txt"
	
	def __init__(self, parent=None):
		QMainWindow.__init__(self, parent)
		
		self.setWindowTitle(u"Éditeur de cartes")
		self.setWindowIcon(QIcon("icons/application.png"))
		
		# actions
		exit = QAction(QIcon("icons/application-exit.png"), "Quitter", self)
		exit.setShortcut('Ctrl+Q')
		exit.setStatusTip("Quitter l'application")
		# sans confirmation
		self.connect(exit, SIGNAL('triggered()'), qApp, SLOT('quit()'))

		doc_new = QAction(QIcon("icons/document-new.png"), "Nouveau", self)
		doc_new.setShortcut("Ctrl+N")
		doc_new.setStatusTip(u"Créer une nouvelle carte")
		self.connect(doc_new, SIGNAL("triggered()"), self.create_map)
		
		doc_open = QAction(QIcon("icons/document-open.png"), "Ouvrir", self)
		doc_open.setShortcut("Ctrl+O")
		doc_open.setStatusTip("Ouvrir une carte existante")
		self.connect(doc_open, SIGNAL("triggered()"), self.ask_open_map)
		
		doc_save = QAction(QIcon("icons/document-save.png"), "Enregistrer", self)
		doc_save.setShortcut("Ctrl+S")
		doc_save.setStatusTip("Enregistrer la carte")
		self.connect(doc_save, SIGNAL('triggered()'), self.save)
		
		doc_save_as = QAction(QIcon("icons/document-save-as.png"), "Enregistrer sous ...", self)
		doc_save_as.setShortcut("Ctrl+Shift+S")
		doc_save_as.setStatusTip(u"Enregistrer la carte actuelle sous un nom différent")
		self.connect(doc_save_as, SIGNAL("triggered()"), self.save_as)
		
		about_qt = QAction(QIcon("icons/information.png"), u"À propos de Qt", self)
		self.connect(about_qt, SIGNAL("triggered()"), qApp, SLOT("aboutQt()"))
		
		# MENUBAR ##############################################################
		
		# MENU Fichier
		
		menubar = self.menuBar()
		file = menubar.addMenu('&Fichier')
		file.addAction(doc_new)
		file.addAction(doc_open)
		file.addAction(doc_save)
		file.addAction(doc_save_as)
		file.addSeparator()
		file.addAction(exit)
		
		# MENU Édition
		
		# paint
		paint = QAction(QIcon("icons/paint.png"), "Peindre avec la tile courante", self)
		paint.setStatusTip("Remplir toute la zone avec la tile courante")
		self.connect(paint, SIGNAL("triggered()"), self.paint_all)
		# undo tile
		act_undo = QAction(QIcon("icons/edit-undo.png"), "Annuler tile", self)
		act_undo.setShortcut("U")
		act_undo.setStatusTip(u"Annuler le dernier placement de tile")
		self.connect(act_undo, SIGNAL("triggered()"), self.undo)
		# add line
		act_add_line = QAction(QIcon("icons/edit-add-line.png"), "Ajouter une ligne", self)
		act_add_line.setStatusTip("Ajouter une ligne de zones dans la carte")
		self.connect(act_add_line, SIGNAL("triggered()"), self.add_line)
		# add column
		act_add_col = QAction(QIcon("icons/edit-add-col.png"), "Ajouter une colonne", self)
		act_add_col.setStatusTip("Ajouter une colonne de zones dans la carte")
		self.connect(act_add_col, SIGNAL("triggered()"), self.add_column)
		# remove line
		act_rem_line = QAction(QIcon("icons/edit-remove-line.png"), "Supprimer une ligne", self)
		act_rem_line.setStatusTip("Supprimer une ligne de zones de le carte")
		# remove column
		act_rem_col = QAction(QIcon("icons/edit-remove-col.png"), "Supprimer une colonne", self)
		act_rem_col.setStatusTip("Supprimer une colonne de zones de le carte")
		# delete entity
		act_del_entity = QAction(QIcon("icons/entity-remove.png"), u"Supprimer une entité", self)
		act_del_entity.setStatusTip(u"Supprimer n'importe quelle entité de la zone courante")
		act_del_entity.setShortcut("S")
		self.connect(act_del_entity, SIGNAL("triggered()"), self.delete_entity)
		# move entity
		act_move_entity = QAction(QIcon("icons/entity-move.png"), u"Déplacer une entité", self)
		act_move_entity.setStatusTip(u"Déplacer n'importe quelle entité de la zone courante")
		act_move_entity.setShortcut("D")
		self.connect(act_move_entity, SIGNAL("triggered()"), self.move_entity)
		
		edit = menubar.addMenu(u"&Édition")
		edit.addAction(act_undo)
		edit.addAction(paint)
		
		edit.addSeparator()
		edit.addAction(act_add_line)
		edit.addAction(act_add_col)
		#edit.addAction(act_rem_line)
		#edit.addAction(act_rem_col)
		
		edit.addSeparator()
		edit.addAction(act_move_entity)
		edit.addAction(act_del_entity)
		
		# MENU Affichage
		
		act_show_units = QAction(u"Afficher les unités", self)
		act_show_units.setCheckable(True)
		act_show_units.setChecked(True)
		self.show_units = act_show_units
		self.connect(act_show_units, SIGNAL("toggled(bool)"), lambda x: self.map.toggle_units(x))
		
		act_show_decors = QAction(u"Afficher les décors", self)
		act_show_decors.setCheckable(True)
		act_show_decors.setChecked(True)
		self.show_decors = act_show_decors
		self.connect(act_show_decors, SIGNAL("toggled(bool)"), lambda x: self.map.toggle_decors(x))
		
		act_show_items = QAction(u"Afficher les objets", self)
		act_show_items.setCheckable(True)
		act_show_items.setChecked(True)
		self.show_items = act_show_items
		self.connect(act_show_items, SIGNAL("toggled(bool)"), lambda x: self.map.toggle_items(x))
		
		act_show_grid = QAction(u"Afficher la grille", self)
		act_show_grid.setCheckable(True)
		act_show_grid.setShortcut("G")
		self.connect(act_show_grid, SIGNAL("toggled(bool)"), lambda x: self.map.toggle_grid(x))
		
		menu_display = menubar.addMenu("&Affichage")
		menu_display.addAction(act_show_units)
		menu_display.addAction(act_show_decors)
		menu_display.addAction(act_show_items)
		menu_display.addSeparator()
		menu_display.addAction(act_show_grid)
		
		# MENU Navigation
		
		# up
		go_up = QAction(QIcon("icons/go-up.png"), u"Nord", self)
		go_up.setShortcut(Qt.Key_Up)
		self.connect(go_up, SIGNAL("triggered()"), lambda: self.change_zone(0, -1))
		# down
		go_down = QAction(QIcon("icons/go-down.png"), u"Sud", self)
		go_down.setShortcut(Qt.Key_Down)
		self.connect(go_down, SIGNAL("triggered()"), lambda: self.change_zone(0, 1))
		# left
		go_left = QAction(QIcon("icons/go-left.png"), u"Ouest", self)
		go_left.setShortcut(Qt.Key_Left)
		self.connect(go_left, SIGNAL("triggered()"), lambda: self.change_zone(-1, 0))
		# right
		go_right = QAction(QIcon("icons/go-right.png"), u"Est", self)
		go_right.setShortcut(Qt.Key_Right)
		self.connect(go_right, SIGNAL("triggered()"), lambda: self.change_zone(1, 0))
		
		navigation = menubar.addMenu("&Navigation")
		navigation.addAction(go_up)
		navigation.addAction(go_down)
		navigation.addAction(go_left)
		navigation.addAction(go_right)
		
		# MENU Outils
		
		tools = menubar.addMenu("&Outils")
		act_config = QAction(QIcon("icons/configuration.png"), u"Configuration", self)
		self.connect(act_config, SIGNAL("triggered()"), self.show_config)
		tools.addAction(act_config)

		# MENU Aide
		
		help = menubar.addMenu("&Aide")
		about = QAction(QIcon("icons/information.png"), u"À propos", self)
		self.connect(about, SIGNAL("triggered()"), self.about)
		help.addAction(about)
		help.addAction(about_qt)
		
		# TOOLBAR
		toolbar = self.addToolBar("Toolbar")
		toolbar.addAction(doc_new)
		toolbar.addAction(doc_open)
		toolbar.addAction(doc_save)
		toolbar.addSeparator()
		toolbar.addAction(act_undo)
		
		toolbar.addSeparator()
		toolbar.addAction(act_move_entity)
		toolbar.addAction(act_del_entity)
		
		# LOADING CONFIG
		config = Config()
		config.load_from_file(self.CONFIG_FILE)
		self.default_map_path = config["map_path"]
		TiledCanvas.TILESIZE = int(config["tilesize"])
		
		self.factory = EntityFactory()
		self.factory.load_units(config["units"])
		self.factory.load_decors(config["decors"])
		self.factory.load_items(config["items"])
		
		# WIDGETS
		self.info = FrameInfo()
		self.info.set_musics(config["musics"])
		self.connect(self.info.combo_music, SIGNAL("activated(QString)"), self.music_selected)
		
		self.tileset = Tileset(config["tileset"], self.info)
		self.tileset.set_max_row(16)
		self.tileset.set_max_line(20)
		
		tabs = QTabWidget(self)
		
		index = tabs.addTab(self.tileset, QIcon("icons/tab-map.png"), "Terrain")
		tabs.setCurrentIndex(index)
		tabs.setTabToolTip(index, "Positionner les tiles")
		
		index = tabs.addTab(UnitSelectionPanel(self), QIcon("icons/tab-units.png"), u"Unités")
		tabs.setTabToolTip(index, u"Choisir et positionner une unité dans la zone courante")
		
		index = tabs.addTab(DecorSelectionPanel(self), QIcon("icons/tab-decors.png"), u"Décors")
		tabs.setTabToolTip(index, u"Choisir et positionner un décor dans la zone courante")
		
		index = tabs.addTab(ItemSelectionPanel(self), QIcon("icons/tab-items.png"), u"Objets")
		tabs.setTabToolTip(index, u"Choisir et positionner un objet dans la zone courante")
		
		self.map = Map(self.tileset, config["tileset"])
		self.info.set_tile_images(self.map.get_tile_images())
		self.connect(self.map, SIGNAL("music_changed(PyQt_PyObject)"), self.info.set_current_music)
		
		# LAYOUT
		vbox = QVBoxLayout()		
		vbox.addWidget(self.map)
		vbox.addWidget(self.info)
		
		hbox = QHBoxLayout()
		hbox.addWidget(tabs)
		hbox.addLayout(vbox)
		
		root = QWidget()
		root.setLayout(hbox)
		self.setCentralWidget(root)
		
		self.center()
		
		# STATUS BAR
		self.statusBar().showMessage(u"Prêt")
		
		self.config = config
	
	
	def music_selected(self, music):
		# QString to str, sans l'extension
		music_name = os.path.splitext(str(music))[0]
		self.map.get_current_zone().set_music(music_name)
		self.statusBar().showMessage(u"Musique sélectionnée : " + music_name)
	
	
	def undo(self):
		if self.map.undo_put_tile():
			self.statusBar().showMessage(u"Placement annulé")
		else:
			self.statusBar().showMessage(u"Annulation impossible (l'historique est vide)")
	
	
	def paint_all(self):
		tile_id = self.tileset.get_tile_id()
		if tile_id != -1:
			self.map.fill_with_tile(tile_id)
	
	
	def show_config(self):
		win = Dialog.ConfigDialog(self)
		win.load_config(self.config)
		win.exec_()
		if win.valided():
			win.write_config(self.config)
			self.config.save_to_file(self.CONFIG_FILE)
			#updating application
			self.info.set_musics(self.config["musics"])
			TiledCanvas.TILESIZE = int(self.config["tilesize"])
			self.tileset.set_tileset_image(self.config["tileset"])
			self.tileset.make_cursor()
			self.map.set_tileset_image(self.config["tileset"])
			
			self.factory.load_units(self.config["units"])
			self.factory.load_decors(self.config["decors"])
			self.factory.load_items(self.config["items"])
			
			self.map.reload()
			
	
	def about(self):
		QMessageBox.about(self, u"À propos",
			u"Éditeur de cartes XML en Python / Qt\n\n" +
			"Python " + sys.version[:sys.version.find(" ")] + " / " +
			"PyQt " + PYQT_VERSION_STR)
	
	
	def add_unit(self, unit_id):
		self.map.place_unit(unit_id)
		self.statusBar().showMessage(u"Cliquez pour placer l'unité \"%s\"" %
			self.factory.get_unit_by_id(unit_id).name)
	
	
	def add_decor(self, decor_id):
		self.map.place_decor(decor_id)
		self.statusBar().showMessage(u"Cliquez pour placer le décor \"%s\"" %
			self.factory.get_decor_by_id(decor_id).name)
	
	
	def add_item(self, name_id):
		self.map.place_item(name_id)
		self.statusBar().showMessage(u"Cliquez pour placer l'unité \"%s\"" %
			self.factory.get_item_by_name(name_id).label)

				
	def delete_entity(self):
		if self.map.get_current_zone().count_entities() == 0:
			QMessageBox.warning(self, "Action impossible", u"Il n'y a pas d'entités à supprimer dans cette zone")
		else:
			self.statusBar().showMessage(u"Cliquez sur l'entité à supprimer")
			self.map.delete_entity()
	
	
	def move_entity(self):
		if self.map.get_current_zone().count_entities() == 0:
			QMessageBox.warning(self, "Action impossible", u"Il n'y a pas d'entités à déplacer dans cette zone")
		else:
			self.statusBar().showMessage(u"Cliquez sur l'entité à déplacer")
			self.map.move_entity()
	
	
	def ask_open_map(self):
		# QString to str
		map_name = str(QFileDialog.getOpenFileName(self,
			"Ouvrir",
			self.default_map_path,
			"Cartes (*.xml);;Tous (*)"))
		if map_name != "":
			self.open_map(map_name)
	
	
	def open_map(self, map_name):
		if self.map.open(map_name):
			self.statusBar().showMessage(u"Carte %s chargée" % map_name)
			self.info.set_map_name(map_name)
			self.info.txt_width.setText(str(self.map.get_width()))
			self.info.txt_height.setText(str(self.map.get_height()))
			self.info.set_current_zone(*self.map.get_current_zone_pos())
		else:
			QMessageBox.critical(
				self,
				"Erreur",
				u"Impossible d'ouvrir la carte %s" % map_name)
	
	
	def create_map(self):
		win = Dialog.AskMapSize(self)
		win.exec_()
		if win.valided():
			self.map.create(win.get_width(), win.get_height(),
				win.get_zone_width(), win.get_zone_height())
			
			self.statusBar().showMessage(u"Nouvelle carte créée")
			self.info.set_map_name("")
			self.info.txt_width.setText(str(win.get_width()))
			self.info.txt_height.setText(str(win.get_height()))
			self.info.set_current_zone(*self.map.get_current_zone_pos())

	
	def save(self):
		# si la carte possède déjà un nom de fichier, sinon on en demande un
		filename = self.map.get_filename()
		if filename:
			self.map.save()
			self.statusBar().showMessage(u"Carte %s enregistrée" % filename)
		else:
			self.save_as()
	
	
	def save_as(self):
		map_name = str(QFileDialog.getSaveFileName(self,
			"Enregistrer sous ...",
			self.default_map_path,
			"Cartes (*.xml);;Tous (*)"))
		if map_name != "":
			self.map.save_as(map_name)
			self.info.set_map_name(map_name)
			self.statusBar().showMessage(u"Carte %s enregistrée" % map_name)
	
	
	def change_zone(self, dx, dy):
		if self.map.change_zone(dx, dy):
			self.map.mode_place_tile()
			self.map.toggle_units(self.show_units.isChecked())
			self.map.toggle_decors(self.show_decors.isChecked())
			self.map.toggle_items(self.show_items.isChecked())
			self.info.set_current_zone(*self.map.get_current_zone_pos())
		else:
			QMessageBox.warning(self, "Stop", "Limites de la carte atteintes")
	
	
	def add_line(self):
	
		value, ok = QInputDialog.getInteger(self, "Ajouter une ligne",
			u"Indiquez la position à laquelle insérer une ligne de zones",
			0, 0, self.map.get_height())
		
		if ok:
			self.map.add_line(value, 0)
			self.info.txt_height.setText(str(self.map.get_height()))
			self.info.set_current_zone(*self.map.get_current_zone_pos())
			self.statusBar().showMessage("Zones ajoutées à la ligne %d" % value)
	
	
	def add_column(self):
	
		value, ok = QInputDialog.getInteger(self, "Ajouter une colonne",
			u"Indiquez la position à laquelle insérer une colonne de zones",
			0, 0, self.map.get_width())
		
		if ok:
			self.map.add_column(value, 0)		
			self.info.txt_width.setText(str(self.map.get_width()))
			self.info.set_current_zone(*self.map.get_current_zone_pos())
			self.statusBar().showMessage("Zones ajoutées à la colonne %d" % value)

	
	def center(self):
		"Centrer la fenêtre à l'écran"
		
		screen = QDesktopWidget().screenGeometry()
		
		size = self.geometry()
		self.move(
			(screen.width() - size.width()) / 2,
			(screen.height() - size.height()) / 2)

		
	def closeEvent(self, event):
		"Confirmation avant de quitter"
		
		reply = QMessageBox.question(self, "Quitter", u"Quitter l'éditeur ?",
			QMessageBox.Yes, QMessageBox.No)
		if reply == QMessageBox.Yes:
			event.accept()
		else:
			event.ignore()


if __name__ == "__main__":
	app = QApplication(sys.argv)
	
	editor = MainWindow()
	editor.show()
	if sys.argv[1:]:
		editor.open_map(sys.argv[1])
	sys.exit(app.exec_())

