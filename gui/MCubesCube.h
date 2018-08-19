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

template <class ObjectType>
class MCubesCube : public std::array<ObjectType, 8> {

public:
  INLINE void applyXYSymmetry() {
    MCubesCube &arr = *this;
    std::swap(arr[0], arr[4]);
    std::swap(arr[1], arr[5]);
    std::swap(arr[2], arr[6]);
    std::swap(arr[3], arr[7]);
  }

  INLINE void applyXZSymmetry() {
    MCubesCube &arr = *this;
    std::swap(arr[0], arr[3]);
    std::swap(arr[1], arr[2]);
    std::swap(arr[4], arr[7]);
    std::swap(arr[5], arr[6]);
  }

  INLINE void applyYZSymmetry() {
    MCubesCube &arr = *this;
    std::swap(arr[0], arr[1]);
    std::swap(arr[2], arr[3]);
    std::swap(arr[4], arr[5]);
    std::swap(arr[6], arr[7]);
  }

  INLINE void apply0167Symmetry() {
    MCubesCube &arr = *this;
    std::swap(arr[3], arr[4]);
    std::swap(arr[2], arr[5]);
  }

  INLINE void apply0246Symmetry() {
    MCubesCube &arr = *this;
    std::swap(arr[1], arr[3]);
    std::swap(arr[5], arr[7]);
  }

  INLINE void apply0356Symmetry() {
    MCubesCube &arr = *this;
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
class MCubesCubeIndexes : public MCubesCube<unsigned int> {

public:
  void initDefault();
  void inversePermutation();

public:
  unsigned int getEdgeNewIndex(unsigned int oldIndex);

public:
  inline static const unsigned int *edgeToPoint(unsigned int iEdge) {
    return EDGE_TO_POINT[iEdge];
  }

private:
  static unsigned int EDGE_TO_POINT[12][2];
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
class MCubesCubeSigns : public MCubesCube<bool> {

public:
  INLINE void init() {
    for (unsigned int iSign = 0; iSign < 8; iSign++)
      operator[](iSign) = false;
  }

  void setConfiguration(bool s0, bool s1, bool s2, bool s3, bool s4, bool s5,
                        bool s6, bool s7);

public:
  INLINE void reverseSigns() {
    for (unsigned int iSign = 0; iSign < 8; iSign++)
      operator[](iSign) = !operator[](iSign);
  }

  INLINE unsigned int getNbPlusSigns() const {
    unsigned int nbPlus = 0;
    for (unsigned int iSign = 0; iSign < 8; iSign++)
      if (operator[](iSign))
        nbPlus++;
    return nbPlus;
  }

  void setMinusMajority();
};
