#define DEBUG
#include "../../src/Misc.cpp"
#include "../../src/ConfigParser.hpp"
#include "../../src/ConfigParser.cpp"

/*
exemple d'utilisation de la classe ConfigParser
note: pour une utilisation robuste, il faut tester les retours
des fonctions ReadItem et WriteItem
*/

int main()
{
	ConfigParser config;
	
	// lecture
	if (!config.LoadFromFile("config.txt"))
	{
		return 0;
	}
	
	config.SeekSection("Capitaine");
	
	int age, bateaux;
	std::string nom;
	config.ReadItem("age", age);
	config.ReadItem("nom", nom);
	config.ReadItem("bateaux", bateaux);
	
	printf("Le capitaine %s a %d ans et %d bateaux\n",
		nom.c_str(), age, bateaux);
	
	config.SeekSection("Lapin");
	int pattes;
	std::string couleur;
	config.ReadItem("age", age);
	config.ReadItem("pattes", pattes);
	config.ReadItem("couleur", couleur);
	
	printf("Le lapin %s a %d ans et %d pattes\n",
		couleur.c_str(), age, pattes);
	
	// ecriture
	config.WriteItem("age", 7);
	
	config.SeekSection("Capitaine");
	config.WriteItem("nom", "Crochet");
	
	config.SaveToFile("config-new.txt");
	puts("nouvelle config ecrite");
	return 0;
}
