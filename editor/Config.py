# -*- coding: utf-8 -*-

class Config(dict):
	SEPARATOR = "="
	
	def load_from_file(self, filename):
		for line in open(filename):
			line = line[:-1]
			if line:
				key, ignore_this, value = line.partition(Config.SEPARATOR)
				self[key.strip()] = value.strip()
	
	def save_to_file(self, filename):
		file = open(filename, "w")
		for key, value in self.iteritems():
			file.write("%s = %s\n" % (key, value))
		file.close()

