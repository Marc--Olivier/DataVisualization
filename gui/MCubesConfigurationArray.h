/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationArray.h
*/ /*!   
   Contains marching cubes configurations 
   and the associated triangles, 
   that are defined with the intersected edges
*/ /*
 =======================================*/
#ifndef MCubesConfigurationArray_H
#define MCubesConfigurationArray_H

// C / C++
#include <assert.h>

// MCubes
#include "MCubes.h"
// #include "MCubesCube.h"
class MCubesCubeSigns;

#define NB_MAX_CONFIG 256

class MCubesConfigurationArray {
public:
  MCubesConfigurationArray(unsigned int configSize);
  ~MCubesConfigurationArray();

private:
  const unsigned int mConfigSize;
  MCubesCubeSigns *mSigns;
  unsigned int *mNbTriangles;
  unsigned int **mTriangles;

public:
  void setSigns(unsigned int iConfig, bool s0, bool s1, bool s2, bool s3,
                bool s4, bool s5, bool s6, bool s7);
  // void setSigns( unsigned int iConfig, const MCubesCubeSigns & signs ) ;

  void setNbTriangles(unsigned int iConfig, unsigned int nbTriangles);
  void setTriangle(unsigned int iConfig, unsigned int iTriangle,
                   unsigned int iPoint1, unsigned int iPoint2,
                   unsigned int iPoint3);

  INLINE unsigned int getNbConfigurations() const { return mConfigSize; }
  const MCubesCubeSigns &getSigns(size_t iConfig) const;
  INLINE unsigned int getNbTriangles(size_t iConfig) const {
    assert(iConfig < mConfigSize);
    return mNbTriangles[iConfig];
  }
  INLINE const unsigned int *getTriangles(size_t iConfig) const {
    assert(iConfig < mConfigSize);
    return mTriangles[iConfig];
  }

  ///  Variables to find the configuration associated to a sign configuration
  ///  faster

public:
  INLINE unsigned int getConfiguration(bool s0, bool s1, bool s2, bool s3,
                                       bool s4, bool s5, bool s6,
                                       bool s7) const {
    unsigned int configIndex = bool2Int(s0, s1, s2, s3, s4, s5, s6, s7);
    return mConfigurationIndex[configIndex];
  }

public:
  static const size_t UNDEFINED_CONFIGURATION;

private:
  INLINE unsigned int bool2Int(bool s0, bool s1, bool s2, bool s3, bool s4,
                               bool s5, bool s6, bool s7) const {
    const bool boolArray[8] = {s0, s1, s2, s3, s4, s5, s6, s7};
    unsigned int intFromBool = 0;
    for (unsigned int iBool = 0; iBool < 8; iBool++) {
      intFromBool = (intFromBool << 1) | (boolArray[iBool] ? 1 : 0);
    }
    return intFromBool;
  }

private:
  unsigned int mConfigurationIndex[NB_MAX_CONFIG];
};

#endif
