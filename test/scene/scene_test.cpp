#include <gtest/gtest.h>


#include "obj.h"

TEST(Scene, ObjMesh) {
  ObjMesh mesh("cube.obj");

  auto v = mesh.Vertices();
}