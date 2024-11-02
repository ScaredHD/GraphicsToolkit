#include "obj.h"

#include <algorithm>
#include <exception>
#include <iterator>

#include "fmt/core.h"

ObjMesh::ObjMesh(const std::string& file) {
  if (!reader_.ParseFromFile(file, config_)) {
    if (!reader_.Error().empty()) {
      throw std::runtime_error{fmt::format("TinyObjReader: {}", reader_.Error())};
    }
  }

  using std::begin;
  using std::end;

  const auto& attr = reader_.GetAttrib();

  auto copy = [&](const auto& src, auto& dest) {
    dest.resize(src.size());
    std::copy(begin(src), end(src), begin(dest));
  };

  copy(attr.vertices, vertices_);
  copy(attr.normals, normals_);
  copy(attr.texcoords, texCoords_);
  copy(attr.colors, colors_);

  const auto& shapes = reader_.GetShapes();
  for (const auto& s : shapes) {
    const auto& v = s.mesh.indices;
    std::transform(begin(v), end(v), std::back_inserter(indices_),
                   [](const tinyobj::index_t& index) { return index.vertex_index; });
  }
}