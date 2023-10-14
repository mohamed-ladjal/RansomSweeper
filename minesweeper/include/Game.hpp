#ifndef GAME_HPP
#define GAME_HPP

#include "Background.hpp"
#include "Board.hpp"
#include "BoardStatus.hpp"
#include <SFML/Graphics.hpp>

class Game : public sf::NonCopyable {
public:
  Game();
  void run();

private:
  void update();
  void handleEvents();
  void render();

  void restart_game();

  void load_resources();
  void load_icon();

  sf::RenderWindow m_window;
  std::unique_ptr<Board> m_board = nullptr;
  std::unique_ptr<BoardStatus> m_board_status = nullptr;
  std::unique_ptr<Background> m_background = nullptr;

  static constexpr double FPS = 60.f;
  struct Window {
    static constexpr unsigned int WIDTH = 480;
    static constexpr unsigned int HEIGHT = 540;
  };
};

#endif
