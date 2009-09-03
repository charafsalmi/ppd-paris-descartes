#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <string>
#include <SFML/Audio.hpp>


class SoundSystem
{
public:
	static SoundSystem& GetInstance();

	/**
	 * Jouer un son
	 * @param sound_name: identifiant du son dans le media manager
	 */
	void PlaySound(const char* sound_name);

	/**
	 * Jouer une musique
	 * @param music_name: identifiant de la musique dans le media manager
	 */
	void PlayMusic(const char* music_name);
	void PlayMusic(const std::string& music_name);

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Activer/Désactiver la musique
	 */
	void EnableMusic(bool enabled);

	/**
	 * @return true si la musique est activée, sinon false
	 */
	bool IsMusicEnabled() const;

	/**
	 * Changer le volume de la musique (de 0 à 100)
	 */
	void SetMusicVolume(float volume);

	/**
	 * Obtenir le volume de la musique (de 0 à 100)
	 */
	float GetMusicVolume() const;

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
	sf::Music* music_;
	bool enable_music_;
};

#endif // SOUNDSYSTEM_HPP
