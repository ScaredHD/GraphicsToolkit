#include "obj.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <iterator>
#include <numeric>

#include "fmt/core.h"

#include "vec.h"


using std::back_inserter;
using std::begin;
using std::end;
using std::vector;

ObjMesh::ObjMesh(const std::string& file) {
  if (!reader_.ParseFromFile(file, config_)) {
    if (!reader_.Error().empty()) {
      throw std::runtime_error{fmt::format("TinyObjReader: {}", reader_.Error())};
    }
  }

  const auto& attr = reader_.GetAttrib();

  auto append = [](const auto& src, auto& dst, auto proj) {
    std::transform(cbegin(src), cend(src), back_inserter(dst), proj);
  };

  // Copy vertices
  auto identity = [](auto x) { return x; };
  append(attr.vertices, vertices_, identity);
  vertexCount_ = vertices_.size() / 3;

  const auto& shapes = reader_.GetShapes();

  // Read vertex/normal/texcoord indices from reader
  // Record total face count
  for (const auto& s : shapes) {
    const auto& ind = s.mesh.indices;
    faceCount_ += s.mesh.num_face_vertices.size();

    if (ind[0].vertex_index >= 0) {
      auto vertexIndexProj = [](auto index) { return index.vertex_index; };
      append(ind, vertexIndices_, vertexIndexProj);
    }
    if (ind[0].normal_index >= 0) {
      auto normalIndexProj = [](auto index) { return index.normal_index; };
      append(ind, normalIndices_, normalIndexProj);
    }
    if (ind[0].texcoord_index >= 0) {
      auto texCoordIndexProj = [](auto index) { return index.texcoord_index; };
      append(ind, texCoordIndices_, texCoordIndexProj);
    }
  }


  if (normalIndices_.empty()) {
    // Generate normals if no normal data
    bool reverseWinding = false;
    GenerateVertexNormals(reverseWinding);
  } else {

    auto recordVertexNormals = [&](size_t index) {
      float nx = attr.normals[normalIndices_[index]];
      float ny = attr.normals[normalIndices_[index] + 1];
      float nz = attr.normals[normalIndices_[index] + 2];
      Vec3d n = {nx, ny, nz};
      normals_[vertexIndices_[index]].emplace_back(Normalized(n));
    };

    for (size_t f = 0; f < faceCount_; ++f) {
      // Assume triangulation is enabled. Every face is triangle.
      recordVertexNormals(3 * f);
      recordVertexNormals(3 * f + 1);
      recordVertexNormals(3 * f + 2);
    }
  }

  // Merge vertex normals
  for (vector<Vec3d>& vn : normals_) {
    Vec3d sum = std::accumulate(begin(vn), end(vn), Vec3d::Zero(), std::plus<Vec3d>{});
    vn = {Normalized(sum)};
  }
}

void ObjMesh::GenerateVertexNormals(bool reverseWinding) {
  assert(!vertices_.empty() && !vertexIndices_.empty());
  normals_.clear();
  normals_.resize(vertexCount_);

  for (size_t f = 0; f < faceCount_; ++f) {
    size_t i0 = vertexIndices_[3 * f];
    size_t i1 = vertexIndices_[3 * f + 1];
    size_t i2 = vertexIndices_[3 * f + 2];

    Vec3d v0 = {vertices_[3 * i0], vertices_[3 * i0 + 1], vertices_[3 * i0 + 2]};
    Vec3d v1 = {vertices_[3 * i1], vertices_[3 * i1 + 1], vertices_[3 * i1 + 2]};
    Vec3d v2 = {vertices_[3 * i2], vertices_[3 * i2 + 1], vertices_[3 * i2 + 2]};

    auto v01 = v1 - v0;
    auto v02 = v2 - v0;
    auto n = Cross(v01, v02).Normalized();
    if (reverseWinding) {
      n = -n;
    }

    normals_[i0].emplace_back(n);
    normals_[i1].emplace_back(n);
    normals_[i2].emplace_back(n);
  }
}

bool ObjMesh::HasVertex() const {
  return !vertices_.empty();
}

IMesh::VertexArray ObjMesh::Vertices() const {
  return vertices_;
}

bool ObjMesh::HasNormal() const {
  return !normals_.empty();
}

IMesh::NormalArray ObjMesh::Normals() const {
  NormalArray res;
  for (const vector<Vec3d>& vn : normals_) {
    assert(vn.size() == 1);
    Vec3d n = vn[0];
    std::copy(begin(n), end(n), back_inserter(res));
  }
  return res;
}

bool ObjMesh::HasIndex() const {
  return !vertexIndices_.empty();
}

IMesh::IndexArray ObjMesh::Indices() const {
  return vertexIndices_;
}

bool ObjMesh::HasColor() const {
  return !colors_.empty();
}

IMesh::ColorArray ObjMesh::Colors() const {
  return colors_;
}

IMesh::TexCoordArray ObjMesh::TexCoords() const {
  return texCoords_;
}

bool ObjMesh::HasTexCoord() const {
  return !texCoords_.empty();
}
