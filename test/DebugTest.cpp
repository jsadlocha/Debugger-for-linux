#include <gtest/gtest.h>
#include <tools/debugger.hpp>

TEST(TestInit, TestInit)
{
  tools::Debugger a;
  EXPECT_EQ(true, true);
}