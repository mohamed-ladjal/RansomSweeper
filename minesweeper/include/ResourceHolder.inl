template <typename T, typename U>
bool RH<T, U>::load(U id, const std::string &path) {
  std::unique_ptr<T> resource(new T);
  if (resource->loadFromFile(path) == false) {
    return false;
  }
  container.insert({id, std::move(resource)});
  return true;
}

template <typename T, typename U> T *RH<T, U>::get(U id) {
  auto it = container.find(id);
  if (it == container.end()) {
    return nullptr;
  }
  return it->second.get();
}

template <typename T, typename U>
std::map<U, std::unique_ptr<T>> RH<T, U>::container;