#include <chrono>
#include <gtest/gtest.h>
#include <initializer_list>
#include <sstream>
#include <iostream>
#include <chrono>

#include "test_data.h"
#include "deter.h"

/**
 * The tests for the deter application. A simple set of tests for the
 * functionality expected. Uses googletest.
 * 
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 602.202.82
 */
TEST(DeterTest, ReadMatrix) {

  std::stringbuf sbuf {ALL_MATRIX};
  std::istream is(&sbuf);
  int i = 0;
  int sz[] = {1, 2, 3, 4, 4, 4, 4, 6};
  int val[] = {5, 9, 4, 5, 5, 5, 5, 1};
  double det[] = {5, 3, 64, 270, 0, 270, 0, 0};

  auto compute = [&](const int order, const std::unique_ptr<int[]> &m)  {
    EXPECT_EQ(order, sz[i]);
    EXPECT_EQ(m[order*order - 1], val[i]);
    i++;

    return 1;
  };

  auto report = [&](std::ostream &outs, const int size, const std::unique_ptr<int[]> &m, const int det, std::chrono::milliseconds ms) {
    // no op
  };

  deter::read_matrices<int>(is, std::cout, compute, report);
}

TEST(DeterTest, ParseInvalidData) {
  std::stringbuf sbuf {R"(
3
1 2 3
4 5 6
7 8 9
1 2 1
3
11 22 33 44
6 8 9
9 9 9)"};

  std::istream is{&sbuf};
  std::stringbuf obuf;
  std::ostream os{&obuf};
  int result_count = 0;

  auto compute = [&](const int order, const std::unique_ptr<int[]> &m)  {
    return 1;
  };

  auto report = [&](std::ostream &outs, const int size, const std::unique_ptr<int[]> &m, const int det, std::chrono::milliseconds ms) {
    // expect 1 result 
    result_count++;
  };

  int result = deter::read_matrices<int>(is, os, compute, report);
  EXPECT_EQ(result_count, 1); 
  EXPECT_EQ(result, 1) << "Return code from read_matrices";
}

TEST(DeterTest, CreateMinor) {
  // a 3x3 matrix
  auto m = std::make_unique<int[]>(9);
  for (int i=1; i<10; i++) m[i-1] = i;

  auto mi = deter::minor(3, m, 1, 2);

  int x[] { 1, 2, 7, 8 };
  for (int i=0; i < 4; i++) {
    EXPECT_EQ(mi[i], x[i]);
  }

  auto mi2 = deter::minor(3, m, 0, 1);
  int y[] { 4, 6, 7, 9 };
  for (int i=0; i < 4; i++) {
    EXPECT_EQ(mi2[i], y[i]);
  }
}

TEST(DeterTest, ComputeDeter) {

  std::stringbuf sbuf {ALL_MATRIX};
  std::istream is(&sbuf);
  int i = 0;
  int det[] = {5, 3, 64, 270, 0, 270, 0, 0};

  auto report = [&](std::ostream &outs, const int size, const std::unique_ptr<int[]> &m, const int detv, std::chrono::milliseconds ms) {
    EXPECT_EQ(detv, det[i]);
    i++;
  };

  deter::read_matrices<int>(is, std::cout, &deter::computeDeterminant<int>, report);
}

TEST(DeterTest, ComputeRealDeter) {
  std::stringbuf sbuf {R"(
3
3.45 -2.2 4
-1 5.999 2
-3 6.7 4.1)"};

  std::istream is{&sbuf};
  std::stringbuf obuf;
  std::ostream os{&obuf};
  int result_count = 0;


  auto report = [&](std::ostream &outs, const int size, const std::unique_ptr<double[]> &m, const double det, std::chrono::milliseconds ms) {
    EXPECT_DOUBLE_EQ(det, 87.99385499999996);
  };

  int result = deter::read_matrices<double>(is, os, &deter::computeDeterminant<double>, report);
  EXPECT_EQ(result, 0);
}
