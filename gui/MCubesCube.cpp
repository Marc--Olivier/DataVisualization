/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesTools.cpp
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

// Class definition
#include "MCubesCube.h"

// C / C++
#include <assert.h>

// Qt

// MCubes

/*=======================================*/
/*=======================================*/
/*=======================================*/

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

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesCube.cpp
*/ /*!   
   Initialize default values
*/ /*
 =======================================*/
void MCubesCubeIndexes::initDefault() {
  for (unsigned int iVertex = 0; iVertex < 8; iVertex++)
    operator[](iVertex) = iVertex;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   13/07/2010
   \file   MCubesCube.cpp
*/ /*!   
   compute the inverse transformation
*/ /*
 =======================================*/
void MCubesCubeIndexes::inversePermutation() {
  MCubesCubeIndexes oldPermutation = *this;
  for (unsigned int iPermutation = 0; iPermutation < 8; iPermutation++) {
    assert(oldPermutation[iPermutation] < 8);
    operator[](oldPermutation[iPermutation]) = iPermutation;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesCube.cpp
*/ /*!   
   get the index of the edge in the new cube index
   from the mesh index in the 0..8 cube index
*/ /*
 =======================================*/
unsigned int MCubesCubeIndexes::EDGE_TO_POINT[12][2] = {
    {0, 1}, {2, 3}, {6, 7}, {4, 5}, {0, 3}, {1, 2},
    {5, 6}, {4, 7}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

unsigned int MCubesCubeIndexes::getEdgeNewIndex(unsigned int oldIndex) {
  unsigned int iOldP1 = EDGE_TO_POINT[oldIndex][0];
  unsigned int iOldP2 = EDGE_TO_POINT[oldIndex][1];

  unsigned int iNewP1 = operator[](iOldP1);
  unsigned int iNewP2 = operator[](iOldP2);
  if (iNewP1 > iNewP2) {
    unsigned int tmp = iNewP1;
    iNewP1 = iNewP2;
    iNewP2 = tmp;
  }

  unsigned int edgeNewIndex = (unsigned int)-1;
  for (unsigned int iEdgeIndex = 0; iEdgeIndex < 12; iEdgeIndex++) {
    if (EDGE_TO_POINT[iEdgeIndex][0] == iNewP1 &&
        EDGE_TO_POINT[iEdgeIndex][1] == iNewP2) {
      edgeNewIndex = iEdgeIndex;
      break;
    }
  }

  assert(edgeNewIndex != (unsigned int)-1);
  return edgeNewIndex;
}

/*=======================================*/
/*=======================================*/
/*=======================================*/

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

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesCube.cpp
*/ /*!   
   Set the signs so that there are more minus than plus
*/ /*
 =======================================*/
void MCubesCubeSigns::setMinusMajority() {
  unsigned int nbPlus = getNbPlusSigns();
  if (nbPlus > 4)
    reverseSigns();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesCube.cpp
*/ /*!   
   Set the signs so that there are more minus than plus
*/ /*
 =======================================*/
void MCubesCubeSigns::setConfiguration(bool s0, bool s1, bool s2, bool s3,
                                       bool s4, bool s5, bool s6, bool s7) {
  operator[](0) = s0;
  operator[](1) = s1;
  operator[](2) = s2;
  operator[](3) = s3;
  operator[](4) = s4;
  operator[](5) = s5;
  operator[](6) = s6;
  operator[](7) = s7;
}

/*=======================================*/
/*=======================================*/
/*=======================================*/
