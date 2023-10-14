#include "Tile.hpp"
#include "ResourceHolder.hpp"
#include "SoundManager.hpp"
#include "Utils.hpp"

Tile::Tile(const sf::Vector2u &coords, sf::Vector2f position, float scale)
    : m_coords(coords) {
  sf::Vector2f v_scale = {scale, scale};
  m_s_tile.setTexture(*TextureHolder::get(Textures::TILE_UNCLICKED));
  m_s_tile.setScale(v_scale);

  m_s_bomb.setTexture(*TextureHolder::get(Textures::BOMB));
  m_s_bomb.setScale(v_scale);
  Utils::center_origin<sf::Sprite>(m_s_bomb);

  m_s_flag.setTexture(*TextureHolder::get(Textures::FLAG));
  m_s_flag.setScale(v_scale);
  Utils::center_origin<sf::Sprite>(m_s_flag);

  sf::Text &bomb_counter = m_bomb_counter.get_drawable();
  bomb_counter.setScale(v_scale);
  bomb_counter.setString("0");
  Utils::center_origin<sf::Text>(bomb_counter);

  sf::Vector2f position_center = {
      position.x + m_s_tile.getGlobalBounds().width / 2.f,
      position.y + m_s_tile.getGlobalBounds().height / 2.f};

  m_s_tile.setPosition(position);
  m_s_bomb.setPosition(position_center);
  m_s_flag.setPosition(position_center);
  m_bomb_counter.get_drawable().setPosition(position_center);
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_s_tile, states);
  if (m_clicked) {
    if (m_bomb_planted) {
      target.draw(m_s_bomb, states);
    } else {
      target.draw(m_bomb_counter, states);
    }
  } else if (m_flagged) {
    target.draw(m_s_flag, states);
  }
}

void Tile::click(int amount) {
  if (m_flagged || m_clicked) {
    return;
  }
  if (m_bomb_planted) {
    m_s_tile.setTexture(*TextureHolder::get(Textures::TILE_EXPLODED));
    SoundManager::play(Sounds::BOMB_EXPLOSION);
  } else {
    m_s_tile.setTexture(*TextureHolder::get(Textures::TILE_CLICKED));
    SoundManager::play(Sounds::TILE_CLICK);
    m_bomb_counter.set_amount(amount);
  }
  m_clicked = true;
}

bool Tile::is_clicked() const { return m_clicked; }

void Tile::set_flag(bool mode) {
  m_flagged = mode;
  SoundManager::play(Sounds::TILE_FLAG_TOGGLE);
}

bool Tile::is_flagged() const { return m_flagged; }

void Tile::plant_bomb() { m_bomb_planted = true; }

bool Tile::is_bomb_planted() const { return m_bomb_planted; }

bool Tile::contains(const sf::Vector2f &position) const {
  return m_s_tile.getGlobalBounds().contains(position);
}

void Tile::set_hover(bool mode) {
  if (m_clicked) {
    return;
  }
  m_hover = mode;
  if (mode) {
    m_s_tile.setTexture(*TextureHolder::get(Textures::TILE_ACTIVE));
  } else {
    m_s_tile.setTexture(*TextureHolder::get(Textures::TILE_UNCLICKED));
  }
}

bool Tile::is_hover() const { return m_hover; }

sf::Vector2u Tile::get_coords() const { return m_coords; }