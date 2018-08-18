/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.h
*/ /*!   
   3D surface
*/ /*
 =======================================*/
#pragma once

// Inherits

// C / C++
#include <array>
#include <list>

// Qt

// MCubes
#include "MCubes.h"
#include "MCubesData.h"
#include "MCubesTools.h"

class MCubesSurface {

  /*==================
      Constructors
  *==================*/
public:
  MCubesSurface();

private:
  void init();

  /*==================
         Data
  *==================*/
private:
  std::array<MCubesData, 3> mData;

public:
  void setNbData(size_t nbData);
  inline size_t getNbData() const { return mData[0].size(); }
  double getData(size_t iAxis, size_t iData) const {
    return mData[iAxis][iData];
  }
  void setData(size_t iData, double xData, double yData, double zData) {
    mData[I_XAXIS][iData] = xData;
    mData[I_YAXIS][iData] = yData;
    mData[I_ZAXIS][iData] = zData;
  }

public:
  void updateProperties() const;
  std::pair<double, double> getMinMaxValue(size_t iAxis) const;

  /*==================
          Faces
  *==================*/
private:
  std::list<MCubesTriangle> mFaceList;

public:
  void addFace(size_t iPoint1, size_t iPoint2, size_t iPoint3);
  void removeLastFace();
  const std::list<MCubesTriangle> &getFaceList() const;

/*=======================
   Predefined surfaces
*======================*/
#ifdef _DEBUG
public:
  static MCubesSurface *createSinus(double xMin, double xMax, double yMin,
                                    double yMax, double zMin, double zMax,
                                    const unsigned int xNbPoints,
                                    const unsigned int yNbPoints);

  static MCubesSurface *createParaboloide(double xMin, double xMax, double yMin,
                                          double yMax, double zMin, double zMax,
                                          const unsigned int xNbPoints,
                                          const unsigned int yNbPoints);

#endif
};
