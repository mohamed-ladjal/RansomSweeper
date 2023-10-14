#ifndef BOMBCOUNTER_HPP
#define BOMBCOUNTER_HPP

#include <SFML/Graphics.hpp>
#include <map>

class BombCounter : public sf::Drawable {
public:
  BombCounter();
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void set_amount(int bombs);

  sf::Text &get_drawable();

private:
  sf::Text m_digit;
  static std::map<int, sf::Color> m_color;
};

#endif