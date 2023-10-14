#ifndef BOARDSTATUS_HPP
#define BOARDSTATUS_HPP

#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>

#include "Button.hpp"
#include "DigitalDisplay.hpp"

class BoardStatus : public sf::Drawable {
public:
  BoardStatus(const sf::IntRect &space);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void update(const int bombs_left);
  void start_clock();
  void reset_clock();
  void stop_clock();

  bool is_clock_started() const;
  Button &get_button();

private:
  std::unique_ptr<NormalDigitalDisplay> m_bomb_counter = nullptr,
                                        m_timer = nullptr;
  std::unique_ptr<Button> m_reset_button = nullptr;
  bool m_clock_started = false;

  std::chrono::time_point<std::chrono::system_clock> m_clock_start;
  static constexpr unsigned int DISPLAY_PADDING = 5;
};

#endif