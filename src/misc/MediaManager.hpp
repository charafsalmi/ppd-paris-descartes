#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <string>

#ifdef DUMB_MUSIC
#include "Music.hpp"
#endif
#include "../core/Animation.hpp"
#include "BitmapFont.hpp"
#include "LoadingWindow.hpp"

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

	/**
	 * Récupérer une musique
	 * @param[in] key: identifiant de la musique
	 * @return adresse de la musique
	 */
	sf::Music* GetMusic(const char* key);

#ifdef DUMB_MUSIC
	/**
	 * Obtenir une musique
	 * @param[in] key: identifiant de la musique
	 * @return pointeur sur la musique
	 */
	Music* GetDumbMusic(const char* key) const;
#endif

	const std::string& GetPostFX(const char* key) const;

	/**
	 * Obtenir la police de caractères
	 * @return référence sur la police
	 */
	const BitmapFont& GetBitmapFont(const char* key) const;

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
	~MediaManager();

	std::map<std::string, sf::Image> images_;
	std::map<std::string, sf::SoundBuffer> sounds_;
	std::map<std::string, Animation> animations_;
	std::map<std::string, BitmapFont*> fonts_;
	std::map<std::string, sf::Music*> musics_;
#ifdef DUMB_MUSIC
	std::map<std::string, std::string> dumb_musics_;
#endif
	std::map<std::string, std::string> post_fx_;
	
	LoadingWindow &lw_;
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
inline Music* GET_DUMB_MUSIC(const char* key)
{
	return MediaManager::GetInstance().GetMusic(key);
}
#endif


inline const BitmapFont& GET_BITMAP_FONT(const char* key)
{
	return MediaManager::GetInstance().GetBitmapFont(key);
}

inline const Animation& GET_ANIM(const char* key)
{
	return MediaManager::GetInstance().GetAnimation(key);
}

inline const std::string& GET_FX(const char* key)
{
	return MediaManager::GetInstance().GetPostFX(key);
}

#endif /* MEDIAMANAGER_HPP */

