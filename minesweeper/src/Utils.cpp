#include "Utils.hpp"
#include <chrono>
#include <cstdio>
#include <random>

sf::Vector2u Utils::get_random_vector(unsigned int max_x, unsigned int max_y) {
  static std::mt19937 rng(
      std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> w1(0, max_x - 1),
      w2(0, max_y - 1);
  return sf::Vector2u(w1(rng), w2(rng));
}

std::vector<sf::Vector2u>
Utils::get_random_unique_vectors(unsigned int max_x, unsigned int max_y,
                                 unsigned int vectors) {
  std::vector<sf::Vector2u> result;
  for (unsigned int i = 0; i < vectors;) {
    sf::Vector2u v = Utils::get_random_vector(max_x, max_y);
    auto it =
        std::find_if(result.begin(), result.end(), [&v](sf::Vector2u current) {
          return (v.x == current.x && v.y == current.y) ? true : false;
        });
    if (it == result.end()) {
      result.push_back(v);
      i += 1;
    }
  }

  return result;
}