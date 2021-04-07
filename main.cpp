#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"
#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <string>
#include <unordered_set>
#include <vector>

static float TOLERANCE = 0.01f;

struct sweep_info_key {
  float _length;
  int _nb_points;
  std::vector<float> _contour;
};

bool operator==(const sweep_info_key &lhs, const sweep_info_key &rhs) {
  return std::abs(lhs._length - rhs._length) < TOLERANCE &&
         lhs._nb_points == rhs._nb_points &&
         lhs._contour.size() == rhs._contour.size() &&
         std::equal(lhs._contour.begin(), lhs._contour.end(),
                    rhs._contour.begin());
}

std::size_t hash_value_1(const sweep_info_key &the_sweep_info) {
  std::size_t seed = 0;
  boost::hash_combine(seed, the_sweep_info._length);
  boost::hash_combine(seed, the_sweep_info._nb_points);
  return seed;
}

std::size_t hash_value_2(const sweep_info_key &the_sweep_info) {
  std::size_t seed = 0;
  boost::hash_combine(seed, the_sweep_info._length);
  boost::hash_combine(seed, the_sweep_info._nb_points);
  boost::hash_combine(seed, the_sweep_info._contour);
  return seed;
}

struct SweepInfoHash {
  std::size_t operator()(const sweep_info_key &key) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, key._length);
    boost::hash_combine(seed, key._nb_points);
    boost::hash_combine(seed, key._contour);
    return seed;
  }
};
TEST_CASE("sweep_info hash calculation") {
  sweep_info_key sw_info;
  sw_info._length = 100.;
  sw_info._nb_points = 10;
  sw_info._contour = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  sweep_info_key sw_info2;
  sw_info2._length = 100.;
  sw_info2._nb_points = 10;
  sw_info2._contour = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  sweep_info_key sw_info_1000;
  sw_info_1000._length = 20.;
  sw_info_1000._nb_points = 1000;
  sw_info_1000._contour.resize(1000);

  CHECK(hash_value_2(sw_info) == hash_value_2(sw_info2));

  CHECK(hash_value_2(sw_info) != hash_value_2(sw_info_1000));

  std::unordered_set<sweep_info_key, SweepInfoHash> us;
  auto [it, success] = us.insert(sw_info);
  CHECK(success == true);

  auto [it2, success2] = us.insert(sw_info2);
  CHECK(success2 == false);

  std::unordered_map<sweep_info_key, std::string, SweepInfoHash> um;
  um[sw_info] =
      "TopoDs_Shape and triangulation for shape with key {100., 10, (0,1,2..";
  CHECK(um.find(sw_info2) != um.end());
  CHECK(
      um[sw_info2] ==
      "TopoDs_Shape and triangulation for shape with key {100., 10, (0,1,2..");
  um[sw_info_1000] = "TopoDS_Sahape for shape with key {20, 1000, (0,0,0...)}";
  CHECK(um.size() == 2);

  BENCHMARK("Calculate hash using length and nb_points") {
    auto hash = hash_value_1(sw_info);
    return hash;
  };

  BENCHMARK("Calculate hash using length, nb_points and contour") {
    auto hash = hash_value_2(sw_info);
    return hash;
  };

  BENCHMARK("Calculate hash with 1000 values in the contour") {
    auto hash = hash_value_2(sw_info_1000);
    return hash;
  };

  BENCHMARK("Create set and unordered map") {
    std::unordered_set<sweep_info_key, SweepInfoHash> us{sw_info};

    std::unordered_map<sweep_info_key, std::string, SweepInfoHash> um;
    um[sw_info] = "TopoDS_Shape with the geometry";
  };
}
