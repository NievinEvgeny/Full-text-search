#include <gtest/gtest.h>
#include <fts.hpp>

TEST(SumTest, Sum)
{
    int expect = 42;
    int real = sum(40, 2);

    EXPECT_EQ(real, expect);
}