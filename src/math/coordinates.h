#ifndef COORDINATES_H_
#define COORDINATES_H_

struct Cartesian {
  double x;
  double y;
  double z;
};

struct Spherical {
  double r;
  double phi;
  double theta;
};

Cartesian ToCartesian(Spherical p);

Spherical ToSpherical(Cartesian p);

#endif  // COORDINATES_H_