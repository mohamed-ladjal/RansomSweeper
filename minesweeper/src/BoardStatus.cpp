#include "BoardStatus.hpp"
#include "ResourceHolder.hpp"

BoardStatus::BoardStatus(const sf::IntRect &space) {
  sf::Vector2f bomb_counter_pos = {(float)(space.left + DISPLAY_PADDING),
                                   (float)(space.top + space.height / 4.f)};
  m_bomb_counter = std::make_unique<NormalDigitalDisplay>(bomb_counter_pos);

  sf::Vector2f timer_pos = {(float)(space.left + space.width -
                                    NormalDigitalDisplay::get_size().x -
                                    DISPLAY_PADDING),
                            (float)(space.top + space.height / 4.f)};
  m_timer = std::make_unique<NormalDigitalDisplay>(timer_pos);

  m_reset_button =
      std::make_unique<Button>(sf::Vector2f(0, 0), sf::Vector2f(0.5f, 0.5f));
  sf::Vector2f button_pos = {(float)(space.left + space.width / 2.f -
                                     m_reset_button->get_size().x / 2.f),
                             (float)(space.top + space.height / 4.f)};
  m_reset_button->set_position(button_pos);
  m_timer->set_number(0);
}

void BoardStatus::update(const int bombs_left) {
  if (m_clock_started) {
    std::chrono::time_point now = std::chrono::system_clock::now();
    int seconds_elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - m_clock_start)
            .count();
    m_timer->set_number(seconds_elapsed);
  }
  m_bomb_counter->set_number(bombs_left);
}

void BoardStatus::start_clock() {
  m_clock_start = std::chrono::system_clock::now();
  m_clock_started = true;
}

void BoardStatus::stop_clock() {
  m_timer->set_number(0);
  m_clock_started = false;
}

bool BoardStatus::is_clock_started() const { return m_clock_started; }

Button &BoardStatus::get_button() { return *m_reset_button; }

void BoardStatus::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(*m_bomb_counter);
  target.draw(*m_timer);
  target.draw(*m_reset_button);
}