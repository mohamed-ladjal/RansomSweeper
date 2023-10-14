#include "DigitalDisplay.hpp"
#include "ResourceHolder.hpp"
#include <array>

template <unsigned int N>
DigitalDisplay<N>::DigitalDisplay(sf::Vector2f position, sf::Vector2f scale) {
  for (unsigned int i = 0; i < N; i++) {
    m_digits[i].setPosition(position);
    m_digits[i].setScale(scale);
    position.x += Digit::WIDTH * scale.x;
  }
  m_txt_digit = TextureHolder::get(Textures::DIGITAL_DISPLAY_DIGITS);
}

template <unsigned int N>
void DigitalDisplay<N>::set_number(unsigned int number) {
  if (m_txt_digit == nullptr) {
    return;
  }
  std::array<unsigned int, N> digits = split_into_digits(number);
  sf::IntRect digit_position = {0, 0, Digit::WIDTH, Digit::HEIGHT};

  for (unsigned int i = 0; i < N; i++) {
    m_digits[i].setTexture(*m_txt_digit);
    digit_position.left = digits[i] * Digit::WIDTH;
    m_digits[i].setTextureRect(digit_position);
  }
}

template <unsigned int N>
void DigitalDisplay<N>::set_position(sf::Vector2f position) {
  for (unsigned int i = 0; i < N; i++) {
    m_digits[i].setPosition(position);
    position.x += Digit::WIDTH;
  }
}

template <unsigned int N>
std::array<unsigned int, N>
DigitalDisplay<N>::split_into_digits(unsigned int number) {
  std::array<unsigned int, N> digits = {0};
  int i = 0;
  while (number > 0) {
    digits[N - i - 1] = number % 10;
    number /= 10;
    i += 1;
  }
  return digits;
}

template <unsigned int N> sf::Vector2f DigitalDisplay<N>::get_size() {
  return sf::Vector2f(Digit::WIDTH * N, Digit::HEIGHT);
}

template <unsigned int N>
void DigitalDisplay<N>::draw(sf::RenderTarget &target,
                             sf::RenderStates states) const {
  for (unsigned int i = 0; i < N; i++) {
    target.draw(m_digits[i], states);
  }
}