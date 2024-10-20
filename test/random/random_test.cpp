#include <gtest/gtest.h>

#include "gtk_utils.h"
#include "sample.h"

#include <algorithm>
#include <vector>

TEST(Random, Sample1D) {
  using namespace std;
  // Test SampleUniform1D on [0, 1]
  {
    int colCount = 10;
    vector<int> bin(colCount, 0);
    int sampleCount = 500000;

    for (int i = 0; i < sampleCount; ++i) {
      double x = SampleUniform1D();
      int index = Clamp<int>(x * colCount, 0, colCount - 1);
      ++bin[index];
    }

    double probPerCol = 1.0 / colCount;
    double error = 0.01;
    std::for_each(begin(bin), end(bin), [&](int ratio) {
      double p = 1.0 * ratio / sampleCount;
      EXPECT_NEAR(p, probPerCol, error);
    });
  }

  // Test SampleUniform1D on [2, 10]
  {
    double l = 2.0;
    double r = 10.0;
    int colCount = 10;
    double colWidth = (r - l) / colCount;
    vector<int> bin(colCount, 0);
    int sampleCount = 500000;

    for (int i = 0; i < sampleCount; ++i) {
      double x = SampleUniform1D(l, r);
      int index = (x - 2.0) / colWidth;
      index = Clamp(index, 0, colCount - 1);
      ++bin[index];
    }

    double probPerCol = 1.0 / colCount;
    double error = 0.01;
    std::for_each(begin(bin), end(bin), [&](int ratio) {
      double p = 1.0 * ratio / sampleCount;
      EXPECT_NEAR(p, probPerCol, error);
    });
  }
}