#ifndef TILE_HPP
#define TILE_HPP

#include "BombCounter.hpp"
#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable {
public:
  Tile() = default;
  Tile(const sf::Vector2u &coords, sf::Vector2f position, float scale = 1.f);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void plant_bomb();
  void click(int bombs_nearby);
  void set_flag(bool mode);
  void set_hover(bool mode);

  bool is_bomb_planted() const;
  bool is_clicked() const;
  bool is_flagged() const;
  bool is_hover() const;

  bool contains(const sf::Vector2f &position) const;
  sf::Vector2u get_coords() const;

private:
  sf::Vector2u m_coords;

  sf::Sprite m_s_tile, m_s_bomb, m_s_flag;
  BombCounter m_bomb_counter;

  bool m_bomb_planted = false;
  bool m_clicked = false;
  bool m_flagged = false;
  bool m_hover = false;
};

#endif