template <typename T> void Utils::center_origin(T &entity) {
  sf::FloatRect rect = entity.getLocalBounds();
  entity.setOrigin(
      {rect.left + rect.width / 2.f, rect.top + rect.height / 2.f});
}