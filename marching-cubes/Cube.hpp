/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/Geometry3D.hpp"
#include "utils/TinyPermutation.hpp"

#include <bitset>

/*!
 * \namespace marchingcubes::cube
 * \brief The namespace marchingcubes::cube contains some structures related to
 * a cube, such as Vertex, Edge and Cube3D.
 */
namespace marchingcubes::cube {

/*!
 * \enum Vertex
 * \brief The Vertex enum defines a vertex of a cube.
 *
 * The vertices are represented as follows:
 *
 *      6_____7
 *     /|    /|        z
 *    4_____5 |        |  y
 *    | |   | |        | /
 *    | |   | |        |/
 *    | |   | |        #----- x
 *    | 2___|_3
 *    |/    |/
 *    0_____1
 */
enum class Vertex : uint8_t { v0, v1, v2, v3, v4, v5, v6, v7 };

constexpr const std::size_t VERTEX_COUNT = 8;

inline const char *to_string(Vertex v) {
  switch (v) {
  case Vertex::v0:
    return "v0";
  case Vertex::v1:
    return "v1";
  case Vertex::v2:
    return "v2";
  case Vertex::v3:
    return "v3";
  case Vertex::v4:
    return "v4";
  case Vertex::v5:
    return "v5";
  case Vertex::v6:
    return "v6";
  case Vertex::v7:
    return "v7";
  }
}

constexpr Vertex v0 = Vertex::v0;
constexpr Vertex v1 = Vertex::v1;
constexpr Vertex v2 = Vertex::v2;
constexpr Vertex v3 = Vertex::v3;
constexpr Vertex v4 = Vertex::v4;
constexpr Vertex v5 = Vertex::v5;
constexpr Vertex v6 = Vertex::v6;
constexpr Vertex v7 = Vertex::v7;

constexpr size_t toInt(Vertex v) { return static_cast<size_t>(v); }

/*!
 * \enum Edge
 * \brief The Edge enum defines an edge of a cube. Its name contains the
 * the indices of the Vertices it connects.
 *
 *             e67
 *         6---------7
 *        /|        /|
 *    e46/ |    e57/ |
 *      /  | e45  /  |        z
 *     4---------5   |        |  y
 *     |e26|     |   |e37     | /
 *  e04|   |     |e15|        |/
 *     |   |     |   |        #----- x
 *     |   2-----|---3
 *     |  /  e23 |  /
 *     | /e02    | /e13
 *     |/        |/
 *     0---------1
 *         e01
 */
enum class Edge : uint8_t {
  e01,
  e02,
  e04,
  e13,
  e15,
  e23,
  e26,
  e37,
  e45,
  e46,
  e57,
  e67
};

inline const char *to_string(Edge e) {
  switch (e) {
  case Edge::e01:
    return "0->1";
  case Edge::e02:
    return "0->2";
  case Edge::e04:
    return "0->4";
  case Edge::e13:
    return "1->3";
  case Edge::e15:
    return "1->5";
  case Edge::e23:
    return "2->3";
  case Edge::e26:
    return "2->6";
  case Edge::e37:
    return "3->7";
  case Edge::e45:
    return "4->5";
  case Edge::e46:
    return "4->6";
  case Edge::e57:
    return "5->7";
  case Edge::e67:
    return "6->7";
  }
}
constexpr size_t EDGE_COUNT = 12;
constexpr size_t ONE_EDGE_SIZE = 4;

constexpr Edge e01 = Edge::e01;
constexpr Edge e02 = Edge::e02;
constexpr Edge e04 = Edge::e04;
constexpr Edge e13 = Edge::e13;
constexpr Edge e15 = Edge::e15;
constexpr Edge e23 = Edge::e23;
constexpr Edge e26 = Edge::e26;
constexpr Edge e37 = Edge::e37;
constexpr Edge e45 = Edge::e45;
constexpr Edge e46 = Edge::e46;
constexpr Edge e57 = Edge::e57;
constexpr Edge e67 = Edge::e67;

inline Edge invalidEdge(Vertex start, Vertex end) {
  throw std::invalid_argument(to_string(start) + std::string(" -> ") +
                              to_string(end));
}

inline Edge edge(Vertex start, Vertex end) {
  switch (start) {
  case Vertex::v0:
    switch (end) {
    case Vertex::v1:
      return Edge::e01;
    case Vertex::v2:
      return Edge::e02;
    case Vertex::v4:
      return Edge::e04;
    default:
      return invalidEdge(start, end);
    }
  case Vertex::v1:
    switch (end) {
    case Vertex::v3:
      return Edge::e13;
    case Vertex::v5:
      return Edge::e15;
    default:
      return invalidEdge(start, end);
    }
  case Vertex::v2:
    switch (end) {
    case Vertex::v3:
      return Edge::e23;
    case Vertex::v6:
      return Edge::e26;
    default:
      return invalidEdge(start, end);
    }
  case Vertex::v3:
    return end == Vertex::v7 ? Edge::e37 : invalidEdge(start, end);
  case Vertex::v4:
    switch (end) {
    case Vertex::v5:
      return Edge::e45;
    case Vertex::v6:
      return Edge::e46;
    default:
      return invalidEdge(start, end);
    }
  case Vertex::v5:
    return end == Vertex::v7 ? Edge::e57 : invalidEdge(start, end);
  case Vertex::v6:
    return end == Vertex::v7 ? Edge::e67 : invalidEdge(start, end);
  default:
    return invalidEdge(start, end);
  }
}

inline Vertex startVertex(Edge e) {
  switch (e) {
  case e01:
    return v0;
  case e02:
    return v0;
  case e04:
    return v0;
  case e13:
    return v1;
  case e15:
    return v1;
  case e23:
    return v2;
  case e26:
    return v2;
  case e37:
    return v3;
  case e45:
    return v4;
  case e46:
    return v4;
  case e57:
    return v5;
  case e67:
    return v6;
  }
}

inline Vertex endVertex(Edge e) {
  switch (e) {
  case e01:
    return v1;
  case e02:
    return v2;
  case e04:
    return v4;
  case e13:
    return v3;
  case e15:
    return v5;
  case e23:
    return v3;
  case e26:
    return v6;
  case e37:
    return v7;
  case e45:
    return v5;
  case e46:
    return v6;
  case e57:
    return v7;
  case e67:
    return v7;
  }
}
} // namespace marchingcubes::cube

namespace std {
inline std::string to_string(marchingcubes::cube::Vertex v) {
  return marchingcubes::cube::to_string(v);
}
inline std::string to_string(marchingcubes::cube::Edge e) {
  return marchingcubes::cube::to_string(e);
}
} // namespace std

namespace marchingcubes::cube {

using Permutation = utils::Permutation<uint8_t, VERTEX_COUNT>;

inline Edge toPermutatedCube(Edge src, const Permutation permutation) {
  auto newStart = permutation[toInt(startVertex(src))];
  auto newEnd = permutation[toInt(endVertex(src))];
  return newStart < newEnd
             ? edge(static_cast<Vertex>(newStart), static_cast<Vertex>(newEnd))
             : edge(static_cast<Vertex>(newEnd), static_cast<Vertex>(newStart));
}

/*!
 * \class Cube3D
 * \brief The class Cube3D stores the 3D coordinates of its vertices
 * in an array.
 *
 *      6_____7
 *     /|    /|        z
 *    4_____5 |        |  y
 *    | |   | |        | /
 *    | |   | |        |/
 *    | |   | |        #----- x
 *    | 2___|_3
 *    |/    |/
 *    0_____1
 */
class Cube3D {
public:
  constexpr Cube3D(
      const std::array<std::pair<double, double>, 3> &dimsMinMax) noexcept
      : minMax{dimsMinMax} {
    for (std::size_t i = 0; i < dimsMinMax.size(); ++i) {
      assert(dimsMinMax[i].first <= dimsMinMax[i].second);
    }
  }

  constexpr double min(size_t dim) const { return minMax[dim].first; }
  constexpr double max(size_t dim) const { return minMax[dim].second; }

  constexpr Point3D interpolatedPoint(cube::Edge edge, double offset_) const
      noexcept {
    switch (edge) {
    case Edge::e01:
      return Point3D{{fromOffset(min(X), max(X), offset_), min(Y), min(Z)}};
    case Edge::e02:
      return Point3D{{min(X), fromOffset(min(Y), max(Y), offset_), min(Z)}};
    case Edge::e04:
      return Point3D{{min(X), min(Y), fromOffset(min(Z), max(Z), offset_)}};
    case Edge::e13:
      return Point3D{{max(X), fromOffset(min(Y), max(Y), offset_), min(Z)}};
    case Edge::e15:
      return Point3D{{max(X), min(Y), fromOffset(min(Z), max(Z), offset_)}};
    case Edge::e23:
      return Point3D{{fromOffset(min(X), max(X), offset_), max(Y), min(Z)}};
    case Edge::e26:
      return Point3D{{min(X), max(Y), fromOffset(min(Z), max(Z), offset_)}};
    case Edge::e37:
      return Point3D{{max(X), max(Y), fromOffset(min(Z), max(Z), offset_)}};
    case Edge::e45:
      return Point3D{{fromOffset(min(X), max(X), offset_), min(Y), max(Z)}};
    case Edge::e46:
      return Point3D{{min(X), fromOffset(min(Y), max(Y), offset_), max(Z)}};
    case Edge::e57:
      return Point3D{{max(X), fromOffset(min(Y), max(Y), offset_), max(Z)}};
    case Edge::e67:
      return Point3D{{fromOffset(min(X), max(X), offset_), max(Y), max(Z)}};
    }
  }

  void setMinMax(size_t dim, double min, double max) {
    assert(min <= max);
    minMax[dim] = std::make_pair(min, max);
  }

private:
  std::array<std::pair<double, double>, 3> minMax;

  static constexpr double fromOffset(double v1, double v2,
                                     double offset) noexcept {
    return v1 + (v2 - v1) * offset;
  }
};

/*!
 * \class Configuration
 * \brief The class Configuration stores the sign of f(V_i)-isovalue on
 * each vertex V_i of the cube.
 *
 *      6_____7
 *     /|    /|        z
 *    4_____5 |        |  y
 *    | |   | |        | /
 *    | |   | |        |/
 *    | |   | |        #----- x
 *    | 2___|_3
 *    |/    |/
 *    0_____1
 */
struct Configuration {

  constexpr Configuration() noexcept {}
  constexpr Configuration(const Configuration &rhs) noexcept
      : config(rhs.config) {}

  constexpr Configuration(std::bitset<VERTEX_COUNT> config) noexcept
      : config(config) {}

  constexpr Configuration(uint8_t config) noexcept : config(config) {}

  constexpr std::uint8_t toUint8() const noexcept {
    assert(config.to_ulong() <= std::numeric_limits<uint8_t>::max());
    return static_cast<std::uint8_t>(config.to_ulong());
  }

  constexpr std::uint8_t trueValueCount() const noexcept {
    return config[0] + config[1] + config[2] + config[3] + config[4] +
           config[5] + config[6] + config[7];
  }

  bool operator==(const Configuration &rhs) const {
    return config == rhs.config;
  }

  bool operator!=(const Configuration &rhs) const {
    return config != rhs.config;
  }

  constexpr bool operator[](std::size_t pos) const {
    assert(pos < VERTEX_COUNT);
    return config[pos];
  }

  std::bitset<VERTEX_COUNT>::reference operator[](std::size_t pos) {
    assert(pos < VERTEX_COUNT);
    return config[pos];
  }

  constexpr Configuration flip() const noexcept {
    return Configuration(std::bitset<VERTEX_COUNT>(config).flip());
  }

  std::string to_string() const { return config.to_string(); }
  std::string to_indices() const {
    auto toIndex = [this](auto index) {
      return config[static_cast<size_t>(index)] ? std::to_string(index) + ", "
                                                : "";
    };
    return toIndex(0) + toIndex(1) + toIndex(2) + toIndex(3) + toIndex(4) +
           toIndex(5) + toIndex(6) + toIndex(7);
  }

  static constexpr std::size_t size() noexcept { return VERTEX_COUNT; }

private:
  std::bitset<VERTEX_COUNT> config;
};

template <typename... TArgs>
constexpr uint8_t configurationFromPositiveVertices(uint8_t positiveVertex,
                                                    TArgs... positiveVertices) {
  assert(positiveVertex < 8);
  if constexpr (sizeof...(positiveVertices) == 0) {
    return static_cast<uint8_t>(1 << positiveVertex);
  } else {
    return static_cast<uint8_t>(
        1 << positiveVertex |
        configurationFromPositiveVertices(positiveVertices...));
  }
}

} // namespace marchingcubes::cube

namespace utils {

using namespace marchingcubes;

template <>
inline cube::Configuration applyPermutation(cube::Permutation permutation,
                                            const cube::Configuration &config) {
  auto bitmask = config[permutation[0]] | config[permutation[1]] << 1 |
                 config[permutation[2]] << 2 | config[permutation[3]] << 3 |
                 config[permutation[4]] << 4 | config[permutation[5]] << 5 |
                 config[permutation[6]] << 6 | config[permutation[7]] << 7;
  assert(bitmask < std::numeric_limits<std::uint8_t>::max());
  return cube::Configuration(static_cast<std::uint8_t>(bitmask));
}

} // namespace utils
