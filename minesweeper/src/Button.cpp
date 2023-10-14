#include "Button.hpp"
#include "ResourceHolder.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f scale) {
  m_button.setPosition(position);
  m_button.setScale(scale);
  m_button.setTexture(*TextureHolder::get(Textures::BUTTON_UNCLICKED));
  m_callback = []() {};
}

bool Button::is_hover(const sf::Vector2f &position) const {
  return m_button.getGlobalBounds().contains(position);
}

void Button::on_button_click(buttonCallback callback) { m_callback = callback; }

void Button::click() {
  m_button.setTexture(*TextureHolder::get(Textures::BUTTON_CLICKED));
  m_callback();
}

void Button::release() {
  m_button.setTexture(*TextureHolder::get(Textures::BUTTON_UNCLICKED));
}

sf::Vector2f Button::get_size() const {
  return sf::Vector2f(m_button.getGlobalBounds().width,
                      m_button.getGlobalBounds().height);
}

void Button::set_position(const sf::Vector2f &position) {
  m_button.setPosition(position);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_button, states);
}