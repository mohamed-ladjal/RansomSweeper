#ifndef DIGITALDISPLAY_HPP
#define DIGITALDISPLAY_HPP

#include <SFML/Graphics.hpp>

/*
  This class should come along with digital_display_digits.png
*/

template <unsigned int N> class DigitalDisplay : public sf::Drawable {
public:
  DigitalDisplay(sf::Vector2f position, sf::Vector2f scale = {1.f, 1.f});
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
  void set_number(unsigned int number);
  void set_position(sf::Vector2f position);
  static sf::Vector2f get_size();

private:
  std::array<unsigned int, N> split_into_digits(unsigned int number);

  sf::Sprite m_digits[N];
  sf::Texture *m_txt_digit = nullptr;

  struct Digit {
    static constexpr unsigned int WIDTH = 16;
    static constexpr unsigned int HEIGHT = 31;
  };
};

template class DigitalDisplay<1>;
template class DigitalDisplay<4>;
template class DigitalDisplay<6>;

typedef DigitalDisplay<1> SmallDigitalDisplay;
typedef DigitalDisplay<4> NormalDigitalDisplay;
typedef DigitalDisplay<6> BigDigitalDisplay;

#endif