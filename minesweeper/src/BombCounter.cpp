#include "BombCounter.hpp"
#include "ResourceHolder.hpp"
#include <string>

std::map<int, sf::Color> BombCounter::m_color = {
    {1, sf::Color(42, 33, 202)}, {2, sf::Color(0, 126, 16)},
    {3, sf::Color(219, 2, 5)},   {4, sf::Color(17, 7, 113)},
    {5, sf::Color(120, 6, 22)},  {6, sf::Color(255, 99, 23)},
    {7, sf::Color(0, 0, 0)},     {8, sf::Color(166, 52, 226)}};

BombCounter::BombCounter() {
  m_digit.setFont(*FontHolder::get(Fonts::OPENSANS_BOLD));
  m_digit.setStyle(sf::Text::Bold);
  m_digit.setCharacterSize(70);
}

void BombCounter::set_amount(int bombs) {
  m_digit.setFillColor(m_color[bombs]);
  m_digit.setString((bombs == 0) ? "" : std::to_string(bombs));
}

sf::Text &BombCounter::get_drawable() { return m_digit; }

void BombCounter::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(m_digit, states);
}
