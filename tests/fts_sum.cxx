#include <fts.hpp>
#include <gtest/gtest.h>

TEST(SumTest, Sum)
{
    int expect = 42;
    int real = fts::sum(40, 2);

    EXPECT_EQ(real, expect);
}