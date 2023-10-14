#include "Background.hpp"
#include "ResourceHolder.hpp"

Background::Background(sf::IntRect space) {
  m_background.setPosition(space.left, space.top);
  m_background.setTextureRect(space);
  sf::Texture *txt_background = TextureHolder::get(Textures::BACKGROUND);
  txt_background->setRepeated(true);
  m_background.setTexture(*txt_background);
}

void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_background, states);
}