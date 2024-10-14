#include <gtest/gtest.h>

#include "ref_ptr.h"

TEST(CoreTest, RefPtr) {
  {
    RefPtr<int> p1 = new int{42};
    {
      auto p2 = p1;
      {
        auto p3 = p2;

        EXPECT_EQ(p1.RefCount(), 3);
        EXPECT_EQ(p2.RefCount(), 3);
        EXPECT_EQ(p3.RefCount(), 3);

        EXPECT_EQ(*p1, 42);
        EXPECT_EQ(*p2, 42);
        EXPECT_EQ(*p3, 42);

        ++*p1;

        EXPECT_EQ(*p1, 43);
        EXPECT_EQ(*p2, 43);
        EXPECT_EQ(*p3, 43);
      }

      EXPECT_EQ(p1.RefCount(), 2);
      EXPECT_EQ(p2.RefCount(), 2);
    }
    EXPECT_EQ(p1.RefCount(), 1);

    RefPtr<int> p4 = std::move(p1);
    EXPECT_EQ(p4.RefCount(), 1);
  }
}