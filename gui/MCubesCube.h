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
class MCubesCube : public MCubesArray<8, ObjectType> {

public:
  INLINE void applyXYSymmetry() {
    MCubesArray<8, ObjectType>::exchange(0, 4);
    MCubesArray<8, ObjectType>::exchange(1, 5);
    MCubesArray<8, ObjectType>::exchange(2, 6);
    MCubesArray<8, ObjectType>::exchange(3, 7);
  }

  INLINE void applyXZSymmetry() {
    MCubesArray<8, ObjectType>::exchange(0, 3);
    MCubesArray<8, ObjectType>::exchange(1, 2);
    MCubesArray<8, ObjectType>::exchange(4, 7);
    MCubesArray<8, ObjectType>::exchange(5, 6);
  }

  INLINE void applyYZSymmetry() {
    MCubesArray<8, ObjectType>::exchange(0, 1);
    MCubesArray<8, ObjectType>::exchange(2, 3);
    MCubesArray<8, ObjectType>::exchange(4, 5);
    MCubesArray<8, ObjectType>::exchange(6, 7);
  }

  INLINE void apply0167Symmetry() {
    MCubesArray<8, ObjectType>::exchange(3, 4);
    MCubesArray<8, ObjectType>::exchange(2, 5);
  }

  INLINE void apply0246Symmetry() {
    MCubesArray<8, ObjectType>::exchange(1, 3);
    MCubesArray<8, ObjectType>::exchange(5, 7);
  }

  INLINE void apply0356Symmetry() {
    MCubesArray<8, ObjectType>::exchange(1, 4);
    MCubesArray<8, ObjectType>::exchange(2, 7);
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
