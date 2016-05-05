//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/dijkstra_shortest_paths.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/directed_graph.hpp> // directed_graph
#include <cstddef>                      // size_t
#include <vector>                       // vector

using cpl::dijkstra_shortest_paths;
using cpl::directed_graph;
using std::size_t;

TEST(DijkstraShortestPathsTest, WorksOnDirectedGraphs) {
  directed_graph graph(6);
  std::vector<unsigned> weight_of;
  auto add_bidir_edge = [&](size_t u, size_t v, unsigned weight) {
    graph.add_edge(u, v);
    weight_of.push_back(weight);
    graph.add_edge(v, u);
    weight_of.push_back(weight);
  };

  add_bidir_edge(0, 1, 7);
  add_bidir_edge(0, 2, 9);
  add_bidir_edge(0, 5, 14);
  add_bidir_edge(1, 2, 10);
  add_bidir_edge(1, 3, 15);
  add_bidir_edge(2, 3, 11);
  add_bidir_edge(2, 5, 2);
  add_bidir_edge(3, 4, 6);
  add_bidir_edge(4, 5, 9);

  const auto spaths = dijkstra_shortest_paths(graph, 0, weight_of);
  const std::vector<unsigned> expected = {0, 7, 9, 20, 20, 11};

  EXPECT_EQ(expected, spaths);
}

// Complexity: O(V*avg_degree)
// It might generate parallel edges.
// You can visualize the generated graph on http://g.ivank.net/

// static graph_t make_random_graph(std::size_t num_vertices,
//                                 double out_degree_mean) {
//  assert(num_vertices != 0);
//
//  graph_t graph(num_vertices);
//
//  std::random_device rd;
//  std::default_random_engine gen1(rd());
//  std::mt19937 gen2(rd());
//  std::default_random_engine gen3(rd());
//
//  std::poisson_distribution<size_t> degree_dist(out_degree_mean);
//  std::uniform_int_distribution<size_t> dst_dist(0, num_vertices - 1);
//  std::poisson_distribution<size_t> weight_dist(5.0);
//
//  auto gen_degree = std::bind(degree_dist, std::ref(gen1));
//  auto gen_dst = std::bind(dst_dist, std::ref(gen2));
//  auto gen_weight = std::bind(weight_dist, std::ref(gen3));
//
//  for (size_t src = 0; src < num_vertices; ++src) {
//    size_t degree = gen_degree();
//    while (degree--)
//      graph.add_edge(src, gen_dst()).weight = gen_weight();
//  }
//
//  return graph;
//}
