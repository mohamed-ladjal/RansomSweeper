#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <map>

enum class Sounds {
  TILE_CLICK = 0,
  TILE_FLAG_TOGGLE = 1,
  BOMB_EXPLOSION = 2,
  WIN_SOUND = 3
};

class SoundManager : sf::NonCopyable {
public:
  static bool load(Sounds id, const std::string &path);
  static void play(Sounds id);

private:
  static std::map<Sounds, std::unique_ptr<sf::SoundBuffer>> m_buffer;
  static std::map<Sounds, std::unique_ptr<sf::Sound>> m_sound;
};

#endif