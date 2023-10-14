#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

template <typename T, typename U> class RH : sf::NonCopyable {
public:
  static bool load(U id, const std::string &path);
  static T *get(U id);

private:
  static std::map<U, std::unique_ptr<T>> container;
};

#include "ResourceHolder.inl"

enum class Textures {
  TILE_CLICKED = 0,
  TILE_UNCLICKED = 1,
  TILE_EXPLODED = 2,
  TILE_ACTIVE = 3,
  BOMB = 4,
  FLAG = 5,
  BACKGROUND = 6,
  DIGITAL_DISPLAY_DIGITS = 7,
  BUTTON_CLICKED = 8,
  BUTTON_UNCLICKED = 9
};

enum class Fonts { ARIAL = 0, OPENSANS_BOLD = 1 };

enum class Images { ICON = 0 };

typedef RH<sf::Texture, Textures> TextureHolder;
typedef RH<sf::Font, Fonts> FontHolder;
typedef RH<sf::Image, Images> ImageHolder;

#endif