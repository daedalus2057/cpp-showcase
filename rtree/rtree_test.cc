/**
 * A small test suite for the application.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 11 - 602.202.82
 */
#include <gtest/gtest.h>

#include <random>

#include "rational.h"
#include "rtree.h"

TEST(Rational, Basic) {
  rational r1 = { 2, 3 };
  rational r2 = { 1, 3 };

  rational e1 = { 1, 1 };

  EXPECT_EQ(r1 + r2, e1);

  rational e2 = { 2, 9 };

  EXPECT_EQ(r1 * r2, e2);

  rational e3 = { 8, 27 };

  EXPECT_EQ(r1^3, e3);

  EXPECT_EQ(e2 / r2, r1);

  rational e4 = { 27, 8 };

  EXPECT_EQ(e3.reciprocal(), e4);
}

TEST(Rational, Parse) {
  rational r1("2/3");
  rational e1 {2, 3};
  EXPECT_EQ(r1, e1);

  EXPECT_EQ(r1.to_string(), "2/3");
}

// TEST(RTree, DivideAndConq) {
//   // T(n) = 3T(n/4) + cn^2
//   auto levels = rt::div_and_conq(3, rational{1, 4}, rational{ 1, 1 }, rational { 2, 1 });
//
//   EXPECT_EQ(levels.size(), 4);
//
//   rational expect_sz_0{ 1, 1 };
//   rational expect_cost_0{ 1, 1 };
//   EXPECT_EQ(levels[0].size(), expect_sz_0);
//   EXPECT_EQ(levels[0].cost(), expect_cost_0);
//   EXPECT_EQ(levels[0].total_cost(), expect_cost_0);
//
//   rational expect_sz_1{ 1, 4 };
//   rational expect_cost_1{ 1, 16 };
//   rational expect_tcost_1{ 3, 16 };
//   EXPECT_EQ(levels[1].size(), expect_sz_1);
//   EXPECT_EQ(levels[1].cost(), expect_cost_1);
//   EXPECT_EQ(levels[1].total_cost(), expect_tcost_1);
//
//   rational expect_sz_2{ 1, 16 };
//   rational expect_cost_2{ 1, 256 };
//   rational expect_tcost_2{ 9, 256 };
//   EXPECT_EQ(levels[2].size(), expect_sz_2);
//   EXPECT_EQ(levels[2].cost(), expect_cost_2);
//   EXPECT_EQ(levels[2].total_cost(), expect_tcost_2);
//
//   rational expect_sz_3{ 1, 64 };
//   rational expect_cost_3{ 1, 4096 };
//   rational expect_tcost_3{ 27, 4096 };
//   EXPECT_EQ(levels[3].size(), expect_sz_3);
//   EXPECT_EQ(levels[3].cost(), expect_cost_3);
//   EXPECT_EQ(levels[3].total_cost(), expect_tcost_3);
// }
//
// TEST(RTree, DivideAndConqLog) {
//   // T(n) = 3T(n/4) + c*log_2^2(n)
//   auto levels = rt::div_and_conq_log(3, rational{1, 4}, rational{ 1, 1 }, rational { 2, 1 }, rational {2, 1 });
//
//   EXPECT_EQ(levels.size(), 4);
// }
//
// TEST(RTree, ChipAndConq) {
//   // T(n) = 3T(n - 1) + c*(n^d)
//   auto levels = rt::chip_and_conq(3, 1, rational{ 1, 1 }, rational { 2, 1 });
//
//   EXPECT_EQ(levels.size(), 4);
// }
//
// TEST(RTree, ChipAndConqLog) {
//   // T(n) = 3T(n - 1) + c*log_2^2(n)
//   auto levels = rt::chip_and_conq(3, 1, rational{ 1, 1 }, rational { 2, 1 });
//
//   EXPECT_EQ(levels.size(), 4);
// }
