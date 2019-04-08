// This test contains cheap test cases, completes in a few seconds.

#include "test.h"

namespace ruy {

using LhsScalar = RUY_TEST_LHSSCALAR;
using RhsScalar = RUY_TEST_RHSSCALAR;
using AccumScalar = RUY_TEST_ACCUMSCALAR;
using DstScalar = RUY_TEST_DSTSCALAR;

using TestSetType =
    TestSet<LhsScalar, RhsScalar, BasicSpec<AccumScalar, DstScalar>>;

TEST(RuyTest, TestSquareMuls) {
  const std::vector<int> sizes{
      // small sizes
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      9,
      10,
      // multiplies of 16
      16,
      32,
      48,
      64,
      // pot-minus-1 sizes
      15,
      31,
      63,
      // pot-plus-1 sizes
      17,
      33,
      65,
  };

  for (int size : sizes) {
    TestPackedLinearRCC<TestSetType>(size, size, size);
    TestLinearAllOrders<TestSetType>(size, size, size);
  }
}

TEST(RuyTest, TestMiscMuls) {
  const int shapes[][3] = {
      {2, 3, 4},    {7, 6, 5},    {12, 23, 6},  {19, 3, 11},   {3, 10, 17},
      {30, 21, 43}, {7, 57, 9},   {49, 69, 71}, {38, 111, 29}, {87, 98, 76},
      {16, 96, 16}, {16, 88, 16}, {16, 84, 16}, {16, 92, 16},  {16, 82, 16},
      {16, 81, 16}, {16, 95, 16}, {3, 128, 5}};
  for (const auto& shape : shapes) {
    TestLinearAllOrders<TestSetType>(shape[0], shape[1], shape[2]);
  }
}

TEST(RuyTest, TestDeepMuls) {
  TestPackedLinearRCC<TestSetType>(1, 50001, 1);
  TestLinearAllOrders<TestSetType>(5, 5001, 4);
  TestLinearAllOrders<TestSetType>(9, 1025, 10);
}

TEST(RuyTest, TestShallowMuls) {
  TestLinearAllOrders<TestSetType>(101, 1, 103);
  TestLinearAllOrders<TestSetType>(71, 2, 53);
  TestLinearAllOrders<TestSetType>(51, 3, 73);
  TestLinearAllOrders<TestSetType>(51, 4, 43);
}

TEST(RuyTest, TestNarrowMuls) {
  for (int width : {1, 2, 3, 4, 5, 8}) {
    TestLinearAllOrders<TestSetType>(width, 12, 13);
    TestLinearAllOrders<TestSetType>(15, 19, width);
    TestLinearAllOrders<TestSetType>(width, 123, 137);
    TestLinearAllOrders<TestSetType>(158, 119, width);
  }
}

TEST(RuyTest, TestNonLinear) {
  TestNonLinearAllOrders<TestSetType>(10, 11, 12, 2, 1, 4);
  TestNonLinearAllOrders<TestSetType>(10, 12, 11, 2, 4, 1);
  TestNonLinearAllOrders<TestSetType>(8, 2, 4, 8, 2, 4);
  TestNonLinearAllOrders<TestSetType>(24, 32, 16, 8, 16, 4);
}
}  // namespace ruy
