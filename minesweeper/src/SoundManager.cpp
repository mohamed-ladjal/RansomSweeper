#include "SoundManager.hpp"

std::map<Sounds, std::unique_ptr<sf::SoundBuffer>> SoundManager::m_buffer;
std::map<Sounds, std::unique_ptr<sf::Sound>> SoundManager::m_sound;

bool SoundManager::load(Sounds id, const std::string &path) {
  std::unique_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer);
  if (buffer->loadFromFile(path) == false) {
    return false;
  }
  std::unique_ptr<sf::Sound> sound(new sf::Sound);
  sound->setBuffer(*buffer);
  m_sound.insert({id, std::move(sound)});
  m_buffer.insert({id, std::move(buffer)});
  return true;
}

void SoundManager::play(Sounds id) {
  auto it = m_sound.find(id);
  if (it == m_sound.end()) {
    return;
  }
  it->second->play();
}