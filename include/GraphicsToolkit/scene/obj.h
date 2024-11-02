#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <variant>
#include <vector>

#include "mesh.h"
#include "tiny_obj_loader.h"

class ObjMesh : public IMesh {
public:
  ObjMesh(const std::string& file);

  // bool HasVertex() const;

  // bool HasIndex() const;

  // bool HasNormal() const;

  // bool HasTexCoord() const;

  // bool HasColor() const;

  VertexArray Vertices() const override { return vertices_; }

  IndexArray Indices() const override { return indices_; }

  NormalArray Normals() const override { return normals_; }

  ColorArray Colors() const override { return colors_; }

  TexCoordArray TexCoords() const override { return texCoords_; }

private:
  tinyobj::ObjReader reader_;
  tinyobj::ObjReaderConfig config_;

  VertexArray vertices_;
  IndexArray indices_;
  NormalArray normals_;
  TexCoordArray texCoords_;
  ColorArray colors_;
};

#endif  // OBJ_H