#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Tile.hpp"

class Board : public sf::Drawable {
public:
  Board() = delete;
  explicit Board(const sf::IntRect &board);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void click(sf::Mouse::Button button, sf::Vector2u click_position);
  void highlight_active_tile(const sf::Vector2u &mouse_position);
  void update();
  void reset();

  bool is_board_active() const;
  int get_bombs_left() const;

private:
  static constexpr double BOMB_COVERAGE = 0.2f;
  static constexpr unsigned int BOARD_SIZE = 20;

  sf::IntRect m_global_position;
  float m_tile_size, m_scale_factor;
  bool m_board_active = false;

  Tile m_tiles[Board::BOARD_SIZE][Board::BOARD_SIZE];
  Tile *m_last_highlighted_tile = nullptr;

  std::vector<sf::Vector2u> generate_bombs();
  int get_flagged_tiles() const;
  int get_unclicked_tiles() const;
  int get_bombs_number() const;

  void uncover_empty_tiles(Tile &current);
  int count_bombs_nearby(Tile &tile);
  void detonate_all_bombs();

  void lose_game();
  void win_game();

  Tile *find_tile_by_position(const sf::Vector2u &position);

  Tile *get_top_tile(Tile &tile);
  Tile *get_right_tile(Tile &tile);
  Tile *get_bottom_tile(Tile &tile);
  Tile *get_left_tile(Tile &tile);
  std::array<Tile *, 8> get_sides(Tile &current);
};

#endif