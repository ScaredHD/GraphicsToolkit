#include <iostream>

#include "tensor.h"
#include "tensor_operations.h"

int main() {

  Tensor<float> scalar{3.14f};

  TDimension<> dim;

  auto x = Split<1, 2, 3>::innerMost;
  auto y = Split<1, 2, 3>::rest;


  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 4; ++k) {
        std::cout << Tensor<float, 3, 2, 4>::DimensionType::FlattenedIndex(i, j, k) << "\n";
      }
    }
  }
}