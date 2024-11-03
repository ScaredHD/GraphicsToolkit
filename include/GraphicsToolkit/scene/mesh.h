#ifndef MESH_H
#define MESH_H

#include <cstdint>
#include <vector>

class IMesh {
public:
  struct VertexData {
    struct {
      double x, y, z;
    } pos;

    struct {
      double x, y, z;
    } normal;

    struct {
      double r, g, b;
    } color;

    struct {
      double u, v;
    } texCoord;
  };

  using VertexArray = std::vector<double>;
  virtual VertexArray Vertices() const = 0;
  virtual bool HasVertex() const = 0;

  using IndexArray = std::vector<uint32_t>;
  virtual IndexArray Indices() const = 0;
  virtual bool HasIndex() const = 0;

  using NormalArray = std::vector<double>;
  virtual NormalArray Normals() const = 0;
  virtual bool HasNormal() const = 0;

  using TexCoordArray = std::vector<double>;
  virtual TexCoordArray TexCoords() const = 0;
  virtual bool HasTexCoord() const = 0;

  using ColorArray = std::vector<double>;
  virtual ColorArray Colors() const = 0;
  virtual bool HasColor() const = 0;
};

#endif  // MESH_H