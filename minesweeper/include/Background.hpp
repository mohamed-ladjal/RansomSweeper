#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background : public sf::Drawable {
public:
  Background(sf::IntRect space);
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  sf::Sprite m_background;
};

#endif