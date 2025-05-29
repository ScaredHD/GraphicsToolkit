#include "math.h"


double ToRad(double deg)
{
  return deg * gtk::pi / 180;
}

double ToDeg(double rad)
{
  return rad * 180 / gtk::pi;
}