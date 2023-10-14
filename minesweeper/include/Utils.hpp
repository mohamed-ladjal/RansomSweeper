#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

namespace Utils {
sf::Vector2u get_random_vector(unsigned int max_x, unsigned int max_y);
std::vector<sf::Vector2u> get_random_unique_vectors(unsigned int max_x,
                                                    unsigned int max_y,
                                                    unsigned int vectors);
template <typename T> void center_origin(T &entity);
}; // namespace Utils

#include "Utils.inl"

#endif