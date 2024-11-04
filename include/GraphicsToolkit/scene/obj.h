#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <variant>
#include <vector>

#include "mesh.h"
#include "vec.h"

class ObjMesh : public IMesh {
public:
  ObjMesh(const std::string& file);

  bool HasVertex() const override;

  VertexArray Vertices() const override;

  bool HasNormal() const override;

  NormalArray Normals() const override;

  bool HasIndex() const override;

  IndexArray Indices() const override;

  bool HasColor() const override;

  ColorArray Colors() const override;

  TexCoordArray TexCoords() const override;

  bool HasTexCoord() const override;

private:
  void GenerateVertexNormals(bool reverseWinding);

  IndexArray vertexIndices_;
  IndexArray normalIndices_;
  IndexArray texCoordIndices_;

  size_t vertexCount_ = 0;
  size_t faceCount_ = 0;

  VertexArray vertices_;
  std::vector<std::vector<Vec3d>> normals_;
  TexCoordArray texCoords_;
  ColorArray colors_;
};

#endif  // OBJ_H