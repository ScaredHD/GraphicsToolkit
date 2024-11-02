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
  using IndexArray = std::vector<uint32_t>;
  using NormalArray = std::vector<double>;
  using TexCoordArray = std::vector<double>;
  using ColorArray = std::vector<double>;

  virtual VertexArray Vertices() const = 0;
  virtual IndexArray Indices() const = 0;
  virtual NormalArray Normals() const = 0;
  virtual TexCoordArray TexCoords() const = 0;
  virtual ColorArray Colors() const = 0;
};

#endif  // MESH_H