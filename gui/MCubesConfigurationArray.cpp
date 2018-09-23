/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   Contains marching cubes configurations 
   and the associated triangles, 
   that are defined with the intersected edges
*/ /*
 =======================================*/

// Class definition
#include "MCubesConfigurationArray.h"

// C / C++
#include <cassert>
#include <cstdlib>

// MCubes
#include "MCubesCube.h"

const size_t MCubesConfigurationArray::UNDEFINED_CONFIGURATION =
    std::numeric_limits<size_t>::min();

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesConfigurationArray::MCubesConfigurationArray(unsigned int configSize)
    : mConfigSize(configSize) {
  assert(mConfigSize > 0 && mConfigSize <= NB_MAX_CONFIG);
  mSigns = new MCubesCubeSigns[mConfigSize];
  mNbTriangles = new unsigned int[mConfigSize];
  mTriangles = new unsigned int *[mConfigSize];

  for (unsigned int iConfig = 0; iConfig < mConfigSize; iConfig++) {
    mNbTriangles[iConfig] = 0;
    mTriangles[iConfig] = nullptr;
  }

  for (unsigned int iConfig = 0; iConfig < NB_MAX_CONFIG; iConfig++) {
    mConfigurationIndex[iConfig] = UNDEFINED_CONFIGURATION;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   Destructor
*/ /*
 =======================================*/
MCubesConfigurationArray::~MCubesConfigurationArray() {
  delete[] mSigns;

  for (unsigned int iConfig = 0; iConfig < mConfigSize; iConfig++)
    delete[] mTriangles[iConfig];

  delete[] mNbTriangles;
  delete[] mTriangles;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   set
*/ /*
 =======================================*/
void MCubesConfigurationArray::setSigns(unsigned int iConfig, bool s0, bool s1,
                                        bool s2, bool s3, bool s4, bool s5,
                                        bool s6, bool s7) {
  assert(iConfig < mConfigSize);
  mSigns[iConfig].setConfiguration(s0, s1, s2, s3, s4, s5, s6, s7);
  unsigned int configIndex = bool2Int(s0, s1, s2, s3, s4, s5, s6, s7);
  mConfigurationIndex[configIndex] = iConfig;
}

/*
void MCubesConfigurationArray::setSigns( unsigned int iConfig,
                                         const MCubesCubeSigns & signs )
{
   assert( iConfig < mConfigSize ) ;
   mSigns[iConfig] = signs ;
}
*/

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   set the number of triangles for a given configuration
*/ /*
 =======================================*/
void MCubesConfigurationArray::setNbTriangles(unsigned int iConfig,
                                              unsigned int nbTriangles) {
  assert(iConfig < mConfigSize);
  if (nbTriangles != mNbTriangles[iConfig]) {
    mNbTriangles[iConfig] = nbTriangles;
    delete[] mTriangles[iConfig];
    if (nbTriangles > 0)
      mTriangles[iConfig] = new unsigned int[3 * nbTriangles];
    else
      mTriangles[iConfig] = nullptr;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   set the triangles
*/ /*
 =======================================*/
void MCubesConfigurationArray::setTriangle(unsigned int iConfig,
                                           unsigned int iTriangle,
                                           unsigned int iPoint1,
                                           unsigned int iPoint2,
                                           unsigned int iPoint3) {
  assert(iConfig < mConfigSize);
  assert(iTriangle < mNbTriangles[iConfig]);
  unsigned int *triangle = mTriangles[iConfig] + 3 * iTriangle;
  triangle[0] = iPoint1;
  triangle[1] = iPoint2;
  triangle[2] = iPoint3;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.cpp
*/ /*!   
   Get the signs of the given configuration
*/ /*
 =======================================*/
const MCubesCubeSigns &
MCubesConfigurationArray::getSigns(size_t iConfig) const {
  assert(iConfig < mConfigSize);
  return mSigns[iConfig];
}
