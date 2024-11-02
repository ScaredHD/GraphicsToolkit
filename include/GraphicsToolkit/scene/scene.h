#ifndef SCENE_H
#define SCENE_H

#include <string_view>
#include <unordered_map>

#include "model.h"

class Scene {
public:
  using SceneType = std::unordered_map<std::string_view, std::unique_ptr<Model>>;

  void AddModel(std::string_view name);

  void DeleteModel(std::string_view name);

  Model& GetModel(std::string_view name);

  const SceneType& All() const;

private:
  SceneType models_;
};

#endif  // SCENE_H