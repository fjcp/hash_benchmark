#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include "catch.hpp"

struct point
{
  double x;
  double y;
  double z;
};

struct prd_part
{
  int OID;
  std::string code;
  double weight;
  point cog;
};

bool
operator<(const prd_part& lhs, const prd_part& rhs)
{
  return lhs.OID < rhs.OID;
}

TEST_CASE("benchmark prd_part operations")
{
  int initial_oid{ 0 };

  auto create_part = [&initial_oid]() {
    initial_oid++;
    prd_part a_part;
    a_part.OID = initial_oid;
    a_part.code = "code";
    a_part.weight = initial_oid % 10;
    a_part.cog = point{ a_part.weight / 3, a_part.weight / 5, a_part.weight / 7 };

    return a_part;
  };
  std::vector<prd_part> parts(10000000);
  std::generate(parts.begin(), parts.end(), create_part);

  auto add_weight = [](double lhs, const prd_part& part) { return lhs + part.weight; };

  BENCHMARK("Calculate total weight using algorithm")
  {
    auto total_weight = std::accumulate(parts.begin(), parts.end(), 0.0, add_weight);
    return total_weight;
  };

  BENCHMARK("Calculate total weight using loop")
  {
    double total_weight = 0.0;

    for (int i = 0; i < parts.size(); ++i)
    {
      total_weight += parts[i].weight;
    }
    return total_weight;
  };

  BENCHMARK("Calculate center of gravity using loop")
  {
    double total_weight = 0.0;
    double cog_x = 0.0;
    double cog_y = 0.0;
    double cog_z = 0.0;

    for (int i = 0; i < parts.size(); ++i)
    {
      cog_x += parts[i].weight * parts[i].cog.x;
      cog_y += parts[i].weight * parts[i].cog.y;
      cog_z += parts[i].weight * parts[i].cog.z;
      total_weight += parts[i].weight;
    }
    point COG = { cog_x / total_weight, cog_y / total_weight, cog_z / total_weight };
    return COG;
  };

  BENCHMARK("Calculate min and max weight using algorithm")
  {
    auto [item_min, item_max] =
      std::minmax_element(parts.cbegin(), parts.cend(), [](const auto& lhs, const auto& rhs) {
        return lhs.weight < rhs.weight;
      });

    auto min_weight = *item_min;
    auto max_weight = *item_max;
    return std::make_pair(min_weight, max_weight);
  };

  BENCHMARK("Calculate min and max weight using loop")
  {
    double min_weight = std::numeric_limits<double>::max();
    double max_weight = -std::numeric_limits<double>::max();

    for (int i = 0; i < parts.size(); ++i)
    {
      if (parts[i].weight < min_weight)
      {
        min_weight = parts[i].weight;
      }

      if (parts[i].weight > max_weight)
      {
        max_weight = parts[i].weight;
      }
    }
    return std::make_pair(min_weight, max_weight);
  };
}
