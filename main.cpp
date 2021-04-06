#include <bits/c++config.h>
#include <string>
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"
#include <boost/container_hash/hash.hpp>
#include <unordered_set>
#include <vector>

struct sweep_info {
  float _length;
  int _nb_points;
  std::vector<float> _contour;
  std::string _shape;
};

std::size_t hash_value_1(const sweep_info &the_sweep_info) {
  std::size_t seed = 0;
  boost::hash_combine(seed, the_sweep_info._length);
  boost::hash_combine(seed, the_sweep_info._nb_points);
  return seed;
}

std::size_t hash_value_2(const sweep_info &the_sweep_info) {
  std::size_t seed = 0;
  boost::hash_combine(seed, the_sweep_info._length);
  boost::hash_combine(seed, the_sweep_info._nb_points);
  boost::hash_combine(seed, the_sweep_info._contour);
  return seed;
}
TEST_CASE("sweep_info hash calculation") {
  sweep_info sw_info;
  sw_info._length = 100.;
  sw_info._nb_points = 10;
  sw_info._contour = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  sw_info._shape = "Shape information for thish shape";

  sweep_info sw_info2;
  sw_info2._length = 100.;
  sw_info2._nb_points = 10;
  sw_info2._contour = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  BENCHMARK("Calculate hash using length and nb_points") {
    auto hash = hash_value_1(sw_info);
    return hash;
  };

  BENCHMARK("Calculate hash using length, nb_points and contour") {
    auto hash = hash_value_2(sw_info);
    return hash;
  };
}
