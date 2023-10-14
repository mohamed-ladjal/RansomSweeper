#include "Game.hpp"
#include "ResourceHolder.hpp"
#include "SoundManager.hpp"

Game::Game()
    : m_window(sf::VideoMode(Window::WIDTH, Window::HEIGHT), "Mineswepper",
               sf::Style::Close) {}

void Game::run() {
  load_resources();

  m_background = std::make_unique<Background>(
      sf::IntRect({0, 0, Window::WIDTH, Window::HEIGHT}));
  m_board = std::make_unique<Board>(sf::IntRect({0, 60, 480, 480}));
  m_board_status = std::make_unique<BoardStatus>(sf::IntRect({0, 0, 480, 60}));

  sf::Clock clock;
  sf::Time timer = sf::Time::Zero;
  const sf::Time TIME_PER_FRAME = sf::seconds(1.f / Game::FPS);
  while (m_window.isOpen()) {
    while (timer < TIME_PER_FRAME) {
      timer += clock.restart();
    }
    timer -= TIME_PER_FRAME;
    handleEvents();
    update();
    render();
  }
}

void Game::handleEvents() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_window.close();
    } else if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Key::Escape) {
        m_window.close();
      }
    } else if (event.type == sf::Event::MouseButtonPressed) {
      Button &reset_button = m_board_status->get_button();
      if (reset_button.is_hover(
              sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
        reset_button.click();
        restart_game();
      } else {
        if (m_board_status->is_clock_started() == false) {
          m_board_status->start_clock();
        }
        m_board->click(event.mouseButton.button,
                       sf::Vector2u(event.mouseButton.x, event.mouseButton.y));
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      Button &reset_button = m_board_status->get_button();
      reset_button.release();
    } else if (event.type == sf::Event::MouseMoved) {
      m_board->highlight_active_tile(
          sf::Vector2u(event.mouseMove.x, event.mouseMove.y));
    }
  }
}

void Game::update() {
  if (m_board->is_board_active()) {
    m_board_status->update(m_board->get_bombs_left());
    m_board->update();
  }
}

void Game::render() {
  m_window.clear(sf::Color::White);
  m_window.draw(*m_background);
  m_window.draw(*m_board_status);
  m_window.draw(*m_board);
  m_window.display();
}

void Game::load_resources() {
  FontHolder::load(Fonts::OPENSANS_BOLD, "fonts/opensans_bold.ttf");

  TextureHolder::load(Textures::TILE_CLICKED, "assets/tile_clicked.png");
  TextureHolder::load(Textures::TILE_UNCLICKED, "assets/tile_unclicked.png");
  TextureHolder::load(Textures::TILE_ACTIVE, "assets/tile_active.png");
  TextureHolder::load(Textures::TILE_EXPLODED, "assets/tile_exploded.png");
  TextureHolder::load(Textures::BOMB, "assets/bomb.png");
  TextureHolder::load(Textures::FLAG, "assets/flag.png");
  TextureHolder::load(Textures::BACKGROUND, "assets/background.png");
  TextureHolder::load(Textures::DIGITAL_DISPLAY_DIGITS,
                      "assets/digital_display_digits.png");
  TextureHolder::load(Textures::BUTTON_CLICKED, "assets/button_clicked.png");
  TextureHolder::load(Textures::BUTTON_UNCLICKED,
                      "assets/button_unclicked.png");

  ImageHolder::load(Images::ICON, "assets/icon.png");
  load_icon();

  SoundManager::load(Sounds::TILE_CLICK, "audio/tile_click.wav");
  SoundManager::load(Sounds::TILE_FLAG_TOGGLE, "audio/tile_flag_toggle.wav");
  SoundManager::load(Sounds::BOMB_EXPLOSION, "audio/bomb_explosion.wav");
  SoundManager::load(Sounds::WIN_SOUND, "audio/win_sound.wav");
}

void Game::load_icon() {
  sf::Image *icon = ImageHolder::get(Images::ICON);
  m_window.setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
}

void Game::restart_game() {
  m_board->reset();
  m_board_status->stop_clock();
}