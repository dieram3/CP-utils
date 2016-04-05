//          Copyright Jorge Aguirre, Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/convex_hull.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp>
#include <cpl/geometry/point_order.hpp>

#include <algorithm> // For std::sort
#include <iterator>  // For std::begin, std::end
#include <cstddef>   // For std::size_t
#include <cstdint>   // For std::int32_t

using namespace cpl;
using std::int32_t;

template <class ForwardIt, class Point>
static bool is_ccw_sorted(const Point &center, ForwardIt first,
                          ForwardIt last) {
  if (first == last)
    return true;

  const auto start = *first - center;
  return std::is_sorted(first, last,
                        [center, start](const Point &lhs, const Point &rhs) {
                          return ccw_less(center, start, lhs, rhs);
                        });
}

TEST(ConvexHullTest, SortsPointsInCounterClockwiseOrder) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  vector_t points = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

  std::sort(begin(points), end(points));

  auto hull = convex_hull(begin(points), end(points), true);
  EXPECT_TRUE(is_ccw_sorted(hull.front(), begin(hull), end(hull)));

  points.erase(begin(points),
               convex_hull_partition(begin(points), end(points), true));

  EXPECT_EQ(hull.size(), points.size());

  for (size_t i = 0; i < points.size(); ++i) {
    EXPECT_EQ(hull[i].x, points[i].x);
    EXPECT_EQ(hull[i].y, points[i].y);
  }

  EXPECT_TRUE(is_ccw_sorted(hull.front(), begin(points), end(points)));
}

TEST(ConvexHullTest, WithCollinearPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  vector_t points = {// Square
                     {0, 0},
                     {0, 5},
                     {5, 5},
                     {5, 0},
                     // Collinear points
                     {0, 2},
                     {2, 0},
                     {2, 5},
                     {5, 2},
                     // Inner points
                     {1, 1},
                     {2, 2},
                     {3, 3},
                     {4, 4},
                     {1, 2},
                     {3, 2}};

  std::sort(begin(points), end(points));

  auto hull = convex_hull(begin(points), end(points), true);

  points.erase(begin(points),
               convex_hull_partition(begin(points), end(points), true));

  const size_t expected_len = 8;
  EXPECT_EQ(expected_len, hull.size());
  EXPECT_EQ(expected_len, points.size());

  vector_t result = {
      {0, 0}, {2, 0}, {5, 0}, {5, 2}, {5, 5}, {2, 5}, {0, 5}, {0, 2}};

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(result[i].x, points[i].x);
    EXPECT_EQ(result[i].y, points[i].y);
    EXPECT_EQ(result[i].x, hull[i].x);
    EXPECT_EQ(result[i].y, hull[i].y);
  }
}

TEST(ConvexHullTest, WithoutCollinearPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  vector_t points = {// Square
                     {0, 0},
                     {0, 5},
                     {5, 5},
                     {5, 0},
                     // Collinear points
                     {0, 2},
                     {2, 0},
                     {2, 5},
                     {5, 2},
                     // Inner points
                     {1, 1},
                     {2, 2},
                     {3, 3},
                     {4, 4},
                     {1, 2},
                     {3, 2}};

  std::sort(begin(points), end(points));

  auto hull = convex_hull(begin(points), end(points));

  points.erase(begin(points),
               convex_hull_partition(begin(points), end(points)));

  const size_t expected_len = 4;
  EXPECT_EQ(expected_len, hull.size());
  EXPECT_EQ(expected_len, points.size());

  vector_t result = {{0, 0}, {5, 0}, {5, 5}, {0, 5}};

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(result[i].x, points[i].x);
    EXPECT_EQ(result[i].y, points[i].y);
    EXPECT_EQ(result[i].x, hull[i].x);
    EXPECT_EQ(result[i].y, hull[i].y);
  }
}
