#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <string>

#ifdef DUMB_MUSIC
#include "Music.hpp"
#endif
#include "Animation.hpp"

/**
 * Gestionnaire de ressource (singleton)
 */
class MediaManager
{
public:
	/**
	 * Récupérer l'instance unique
	 * @return référence sur le gestionnaire de ressources
	 */
	static MediaManager& GetInstance();
	
	/**
	 * Récupérer une image
	 * @param[in] key: identifiant de l'image
	 * @return référence sur l'image
	 */
	const sf::Image& GetImage(const char* key) const;
	
	/**
	 * Récupérer un buffer audio
	 * @param[in] key: identifiant du buffer
	 * @return référence sur le buffer
	 */
	const sf::SoundBuffer& GetSoundBuf(const char* key) const;
	
#ifdef DUMB_MUSIC	
	/**
	 * Obtenir une musique
	 * @param[in] key: identifiant de la musique
	 * @return pointeur sur la musique
	 */
	Music* GetMusic(const char* key) const;
	
	Music* GetMusic(unsigned short id) const;
#endif

	const std::string& GetPostFX(const char* key) const;
	/**
	 * Obtenir la police de caractères
	 * @return référence sur la police
	 */
	const sf::Font& GetFont() const;
	
	/**
	 * Obtenir une animation
	 * @param[in] key: identifiant de l'animation
	 */
	const Animation& GetAnimation(const char* key) const;
	
private:
	/**
	 * Initialisation (chargement des images)
	 */
	MediaManager();
	MediaManager(const MediaManager& other);
	MediaManager& operator=(const MediaManager& other);
	
	/**
	 * Construire les frames d'une animation
	 * @param[in] name: nom de l'animation
	 * @param[in] width: largeur d'une frame
	 * @param[in] height: hauteur d'une frame
	 * @param[in] count: nombre de frames
	 * @param[in] delay: temps d'attente entre chaque frame
	 * @param[in] offset: décalage y sur le tileset
	 */
	void BuildAnimation(const char* name, int width, int height, int count,
		float delay, int x_offset=0, int y_offset=0);
	
	std::map<std::string, sf::Image> images_;
	std::map<std::string, sf::SoundBuffer> sounds_;
#ifdef DUMB_MUSIC
	std::map<std::string, std::string> musics_;
#endif
	std::map<std::string, std::string> post_fx_;
	std::map<std::string, Animation> animations_;
	sf::Font font_;
};


// fonctions inline pour alléger la syntaxe

inline const sf::Image& GET_IMG(const char* key)
{
	return MediaManager::GetInstance().GetImage(key);
}

inline const sf::SoundBuffer& GET_SOUNDBUF(const char* key)
{
	return MediaManager::GetInstance().GetSoundBuf(key);
}

#ifdef DUMB_MUSIC
inline Music* GET_MUSIC(const char* key)
{
	return MediaManager::GetInstance().GetMusic(key);
}

inline Music* GET_MUSIC(unsigned short id)
{	// Plus compact, pour utilisation dans les zones. Premier index: 1.
	return MediaManager::GetInstance().GetMusic(id);
}
#endif


inline const sf::Font& GET_FONT()
{
	return MediaManager::GetInstance().GetFont();
}

inline const Animation& GET_ANIM(const char* key)
{
	return MediaManager::GetInstance().GetAnimation(key);
}

inline const std::string& GET_FX(const char* key)
{
	return MediaManager::GetInstance().GetPostFX(key);
}

#endif /* guard MEDIAMANAGER_HPP */

