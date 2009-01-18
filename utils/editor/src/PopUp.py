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
		
		self.protocol("WM_DELETE_WINDOW", self.quit)
		self.bind("<Escape>", self.quit)
		
		self.frm_body = Tk.Frame(self)
		self.frm_body.pack()
		
		self.frm_buttons = Tk.Frame(self, padx=5, pady=5)
		self.frm_buttons.pack()
		
		Tk.Button(self.frm_buttons, text="Valider", command=self.validate).pack(side=Tk.RIGHT)
		Tk.Button(self.frm_buttons, text="Annuler", command=self.quit).pack(side=Tk.RIGHT)
		
		self.body(self.frm_body)
		#self.wait_window(self)
		
	def body(self, master):
		raise NotImplementedError, "body(self, master) method must be overridden"
	
	def validate(self):
		if self.control():
			quit()
	
	def control(self):
		return True
			
	def quit(self, event=None):
		"destruction de la fenêtre"
		
		self.master.focus_set()
		self.destroy()
