# -*- coding: utf-8 -*-

import Tkinter as Tk

class PopUp(Tk.Toplevel):
	"Toplevel unique et au premier plan"
	
	def __init__(self, title, border=5):
	
		Tk.Toplevel.__init__(self)
		self.title(title)
		self.configure(borderwidth=border)
		self.resizable(width=False, height=False)
		self.transient(self.master)
		self.grab_set()
		
		self.protocol("WM_DELETE_WINDOW", self.close)
		self.bind("<Escape>", self.close)
		
		self.frm_body = Tk.Frame(self)
		self.frm_body.pack()
		
		self.frm_buttons = Tk.Frame(self, padx=5, pady=5)
		self.frm_buttons.pack()
		
		Tk.Button(self.frm_buttons, text="Valider", command=self.close).pack(side=Tk.RIGHT)
		#Tk.Button(self.frm_buttons, text="Annuler", command=self.quit).pack(side=Tk.RIGHT)
		
		self.body(self.frm_body)
		#self.wait_window(self)
		
	def body(self, master):
		raise NotImplementedError, "body(self, master) method must be overridden"
	
			
	def close(self, event=None):
		"destruction de la fenêtre"
		
		self.master.focus_set()
		self.destroy()


class TeleportDialog(PopUp):
	"Interface de la gestion des téléporteurs"
	
	def __init__(self, teleporters):
		PopUp.__init__(self, "Gestion des téléporteurs")
		self.teleporters = teleporters
		for tp in sorted(teleporters):
			self.add_teleport(tp)
		
	def body(self, master):
		
		frame_left = Tk.LabelFrame(master, text="Téléporteurs", padx=5, pady=5)
		frame_left.pack(side=Tk.LEFT)
		
		# listbox des tp
		self.lbx_teleport = Tk.Listbox(frame_left, width=50, height=10, bg="white")
		self.lbx_teleport.pack(side=Tk.LEFT)
		
		# barre de défilement verticale à droite du listbox
		scb_teleport = Tk.Scrollbar(frame_left, command=self.lbx_teleport.yview, orient=Tk.VERTICAL)
		scb_teleport.pack(expand=1, fill=Tk.BOTH)
		self.lbx_teleport.configure(yscrollcommand=scb_teleport.set)
		
	def add_teleport(self, teleport):
		"ajouter d'une entité dans la liste et la listbox"
		
		self.lbx_teleport.insert(Tk.END, str(teleport))


class EntityDialog(PopUp):
	"Interface de la gestion des entités"
	
	def __init__(self, entities):
		PopUp.__init__(self, "Gestion des entités")
		self.entities = entities
		for entity in sorted(entities):
			self.add_entity(entity)
		
	def body(self, master):
		
		frame_left = Tk.LabelFrame(master, text="Entités", padx=5, pady=5)
		frame_left.pack(side=Tk.LEFT)
		
		# listbox des entités existantes
		self.lbx_entity = Tk.Listbox(frame_left, width=50, height=10, bg="white")
		self.lbx_entity.pack(side=Tk.LEFT)
		
		# barre de défilement verticale à droite du listbox
		scb_entity = Tk.Scrollbar(frame_left, command=self.lbx_entity.yview, orient=Tk.VERTICAL)
		scb_entity.pack(expand=1, fill=Tk.BOTH)
		self.lbx_entity.configure(yscrollcommand=scb_entity.set)
		
#		frame_right = Tk.Frame(master, padx=5, pady=5)
#		self.btn_add = Tk.Button(frame_right, text="Ajouter")#, command=self.ask_entity)
#		self.btn_add.pack(fill=Tk.X)
#		self.btn_edit = Tk.Button(frame_right, text="Modifier")
#		self.btn_edit.pack(fill=Tk.X)
#		self.btn_delete = Tk.Button(frame_right, text="Supprimer")#, command=self.delete_entity)
#		self.btn_delete.pack(fill=Tk.X)
#		self.btn_del_all = Tk.Button(frame_right, text="Supprimer tous")#, command=self.delete_all)
#		self.btn_del_all.pack(fill=Tk.X)
#		frame_right.pack(side=Tk.RIGHT)
		
	def add_entity(self, entity):
		"ajouter d'une entité dans la liste et la listbox"
		
		self.lbx_entity.insert(Tk.END, str(entity))
		

		
#	def ask_entity(self):
#		"saisir une entité pour l'ajouter"
#		
#		entity = AskEntityDialog(self, "add").get_entity()
#		if entity:
#			self.add_entity(entity)	
#	

#	
#	def delete_entity(self):
#		"supprimer l'entité sélectionnée"
#		
#		self.lbx_entity.delete(self.current)
#		self.lbx_entity.select_set(self.current)
#		self.lbx_entity.select_anchor(self.current)

#		self.current -= 1
#		print "current:", self.current
#		
#	def delete_all(self):
#		"supprimer toutes les entités de la liste"
#		
#		if self.current != -1:
#			self.lbx_entity.delete(0, Tk.END)
#			self.current = -1

#	def get_current(self):
#		"retourne l'entité sélectionnée si elle existe"
#		
#		return self.entities[self.current] if self.current != -1 else None

#	def get_result(self):
#		"retourne toutes les entités si validation"
#		
#		return self.entities if self.save else None

#class AskEntityDialog(PopUp):
#	
#	def __init__(self, master, mode):
#		title = None
#		if mode == "add":
#			pass
#		elif mode == "edit":
#			pass
#		else:
#			raise Exception("bad mode")
#		self.entity = {}
#		PopUp.__init__(self, master)
#		
#		
#	def body(self, master):
#		"construction des widgets"
#		
#		Tk.Label(master, text="Identifiant : ").grid(row=0)
#		Tk.Label(master, text="Position X :").grid(row=1)
#		Tk.Label(master, text="Position Y :").grid(row=2)
#		
#		self.ent_id = Tk.Entry(master)
#		self.ent_id.grid(row=0, column=1)
#		self.ent_posx = Tk.Entry(master)
#		self.ent_posx.grid(row=1, column=1)
#		self.ent_posy = Tk.Entry(master)
#		self.ent_posy.grid(row=2, column=1)
#		

#	def validate(self):
#		"contrôle des valeurs entrées"
#		
#		self.entity.clear()
#		success = False
#		try:
#			id_value = self.ent_id.get()
#			if len(id_value) == 0:
#				raise ValueError
#			self.entity["id"] = id_value
#			self.entity["x"] = int(self.ent_posx.get())
#			self.entity["y"] = int(self.ent_posx.get())
#			success = True
#		except ValueError:
#			tkMessageBox.showwarning("Erreur", "Données invalides")
#			self.entity.clear()
#			
#		return success
#	
#	def get_entity(self):
#		return self.entity

