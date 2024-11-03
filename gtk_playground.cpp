#include "GraphicsToolkit/scene/obj.h"
#include "GraphicsToolkit/math/vec.h"

#include <iostream>

#include <fmt/core.h>


int main() {
  ObjMesh mesh("cube.obj");

  auto n = mesh.Normals();

  Vec3f a {1, 2, 3};
  Vec3f b {4, 5, 6};

  std::cout << fmt::format("({}, {}, {})\n", a[0], a[1], a[2]);

}