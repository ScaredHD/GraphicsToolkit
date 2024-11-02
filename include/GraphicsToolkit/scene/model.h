#ifndef MODEL_H
#define MODEL_H

#include <memory>

#include "mesh.h"
#include "transform.h"

struct Model {
  std::shared_ptr<IMesh> mesh;
  Transform transform;
};



#endif  // MODEL_H