#include "obj.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <iterator>
#include <numeric>

#include "fmt/core.h"
#include "tiny_obj_loader.h"
#include "vec.h"


using std::back_inserter;
using std::begin;
using std::end;
using std::vector;

ObjMesh::ObjMesh(const std::string& file) {
  using namespace tinyobj;
  ObjReader reader;
  ObjReaderConfig config;

  if (!reader.ParseFromFile(file, config)) {
    if (!reader.Error().empty()) {
      throw std::runtime_error{fmt::format("TinyObjReader: {}", reader.Error())};
    }
  }

  auto append = [](const auto& src, auto& dst, auto proj) {
    std::transform(cbegin(src), cend(src), back_inserter(dst), proj);
  };
  auto identity = [](auto x) { return x; };


  const auto& attr = reader.GetAttrib();
  // Copy vertices
  append(attr.vertices, vertices_, identity);

  vertexCount_ = vertices_.size() / 3;

  // Record total face count
  const auto& shapes = reader.GetShapes();
  for (const auto& s : shapes) {
    faceCount_ += s.mesh.num_face_vertices.size();
  }

  // Read vertex/normal/texcoord indices from reader
  for (const auto& s : shapes) {
    const auto& ind = s.mesh.indices;

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
    // Use normal data from obj file
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

size_t ObjMesh::VertexCount() const {
  return vertices_.size() / 3;
}

ITriangleMesh::VertexPos_t ObjMesh::Vertex(VertexIndex_t idx) const {
  double x = vertices_[3 * idx];
  double y = vertices_[3 * idx + 1];
  double z = vertices_[3 * idx + 2];
  return {x, y, z};
}

bool ObjMesh::HasIndex() const {
  return !vertexIndices_.empty();
}

size_t ObjMesh::FaceCount() const {
  return vertexIndices_.size() / 3;
}

ITriangleMesh::FaceIndices_t ObjMesh::FaceIndices(FaceIndex_t idx) const {
  VertexIndex_t i0 = 3 * idx;
  VertexIndex_t i1 = 3 * idx + 1;
  VertexIndex_t i2 = 3 * idx + 2;
  return {i0, i1, i2};
}

bool ObjMesh::HasNormal() const {
  return normals_.empty();
}

ITriangleMesh::VertexNormal_t ObjMesh::VertexNormal(VertexIndex_t idx) const {
  assert(normals_[idx].size() == 1 && "Vertex normal count should be one");
  auto n = normals_[idx][0];
  return {n[0], n[1], n[2]};
}

bool ObjMesh::HasColor() const {
  return !colors_.empty();
}

ITriangleMesh::VertexColor_t ObjMesh::VertexColor(VertexIndex_t idx) const {
  double r = colors_[3 * idx];
  double g = colors_[3 * idx + 1];
  double b = colors_[3 * idx + 2];
  return {r, g, b};
}

bool ObjMesh::HasTexCoord() const {
  return !texCoords_.empty();
}

ITriangleMesh::VertexTexCoord_t ObjMesh::VertexTexCoord(VertexIndex_t idx) const {
  // TODO:
  return {-1, -1};
}
