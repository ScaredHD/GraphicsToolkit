#include "mesh.h"

std::vector<double> VertexArray(const ITriangleMesh& mesh) {
  std::vector<double> res;
  for (size_t i = 0; i < mesh.VertexCount(); ++i) {
    auto v = mesh.Vertex(i);
    res.push_back(v.x);
    res.push_back(v.y);
    res.push_back(v.z);
  }
  return res;
}

std::vector<ITriangleMesh::VertexIndex_t> IndexArray(const ITriangleMesh& mesh) {
  std::vector<ITriangleMesh::VertexIndex_t> res;
  for (size_t i = 0; i < mesh.FaceCount(); ++i) {
    auto f = mesh.FaceIndices(i);
    res.push_back(f.i0);
    res.push_back(f.i1);
    res.push_back(f.i2);
  }
  return res;
}

std::vector<double> NormalArray(const ITriangleMesh& mesh) {
  std::vector<double> res;
  for (size_t i = 0; i < mesh.VertexCount(); ++i) {
    auto n = mesh.VertexNormal(i);
    res.push_back(n.x);
    res.push_back(n.y);
    res.push_back(n.z);
  }
  return res;
}

std::vector<double> TexCoordArray(const ITriangleMesh& mesh) {}

std::vector<double> ColorArray(const ITriangleMesh& mesh) {}