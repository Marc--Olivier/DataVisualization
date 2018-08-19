/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesCube.h
*/ /*!   
   Cube

      7_____6        z
     /|    /|        |  y
    4_____5 |        | /
    | |   | |        |/
    | |   | |        #----- x
    | |   | |   
    | 3___|_2 
    |/    |/    
    0_____1   

*/ /*
 =======================================*/
#pragma once

#include "MCubesTools.h"

#include <bitset>

class MCubesCube {

public:
  MCubesCube() = delete;

public:
  template <typename TCube> static void applyXYSymmetry(TCube &arr) {
    std::swap(arr[0], arr[4]);
    std::swap(arr[1], arr[5]);
    std::swap(arr[2], arr[6]);
    std::swap(arr[3], arr[7]);
  }

  template <typename TCube> static void applyXZSymmetry(TCube &arr) {
    std::swap(arr[0], arr[3]);
    std::swap(arr[1], arr[2]);
    std::swap(arr[4], arr[7]);
    std::swap(arr[5], arr[6]);
  }

  template <typename TCube> static void applyYZSymmetry(TCube &arr) {
    std::swap(arr[0], arr[1]);
    std::swap(arr[2], arr[3]);
    std::swap(arr[4], arr[5]);
    std::swap(arr[6], arr[7]);
  }

  template <typename TCube> static void apply0167Symmetry(TCube &arr) {
    std::swap(arr[3], arr[4]);
    std::swap(arr[2], arr[5]);
  }

  template <typename TCube> static void apply0246Symmetry(TCube &arr) {
    std::swap(arr[1], arr[3]);
    std::swap(arr[5], arr[7]);
  }

  template <typename TCube> static void apply0356Symmetry(TCube &arr) {
    std::swap(arr[1], arr[4]);
    std::swap(arr[2], arr[7]);
  }
};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesCube.h
*/ /*!   
   Cube with integer indexes

      7_____6        z
     /|    /|        |  y
    4_____5 |        | /
    | |   | |        |/
    | |   | |        #----- x
    | |   | |   
    | 3___|_2 
    |/    |/    
    0_____1   

*/ /*
 =======================================*/
class MCubesCubeIndexes {

public:
  MCubesCubeIndexes() = delete;
  using Array = std::array<size_t, 8>;

public:
  static Array createIndices();
  static void inversePermutation(Array &arr);
  static size_t getEdgeNewIndex(const Array &arr, size_t oldIndex);

public:
  static const std::array<size_t, 2> &edgeToPoint(unsigned int iEdge) {
    return EDGE_TO_POINT[iEdge];
  }

private:
  static std::array<size_t, 2> EDGE_TO_POINT[12];
};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesCube.h
*/ /*!   
   Signs on cube vertices

      7_____6          +_____+
     /|    /|     =>  /|    /|  
    4_____5 |        +_____- |
    | |   | |        | |   | |
    | |   | |        | |   | |
    | |   | |        | |   | |
    | 3___|_2        | -___|_+ 
    |/    |/         |/    |/ 
    0_____1          +_____+

*/ /*
 =======================================*/
class MCubesCubeSigns : public std::bitset<8> {

public:
  void setConfiguration(bool s0, bool s1, bool s2, bool s3, bool s4, bool s5,
                        bool s6, bool s7);

public:
  void reverseSigns() { flip(); }

  INLINE unsigned int getNbPlusSigns() const {
    unsigned int nbPlus = 0;
    for (unsigned int iSign = 0; iSign < 8; iSign++)
      if (operator[](iSign))
        nbPlus++;
    return nbPlus;
  }

  void setMinusMajority();
};
