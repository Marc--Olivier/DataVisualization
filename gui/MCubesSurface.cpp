/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   3D surface
*/ /*
 =======================================*/

// Class definition
#include "MCubesSurface.h"

// C / C++
#include <cassert>
#include <cmath>

// Qt

// MCubes
#include "MCubesTools.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesSurface::MCubesSurface() { init(); }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Initialization method
*/ /*
 =======================================*/
void MCubesSurface::init() {
  assert(mData[I_XAXIS].size() == 0);
  assert(mData[I_YAXIS].size() == 0);
  assert(mData[I_ZAXIS].size() == 0);
  assert(mFaceList.size() == 0);
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Set the surface number of points
*/ /*
 =======================================*/
void MCubesSurface::setNbData(size_t nbData) {
  assert(mData[I_XAXIS].size() == mData[I_YAXIS].size() &&
         mData[I_XAXIS].size() == mData[I_ZAXIS].size());
  size_t oldNbPoints = mData[I_XAXIS].size();
  if (nbData != oldNbPoints) {
    for (size_t iAxis = I_XAXIS; iAxis <= I_ZAXIS; iAxis++)
      mData[iAxis].resize(nbData);
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Update the surface properties
*/ /*
 =======================================*/
void MCubesSurface::updateProperties() const {
  for (size_t iAxis = I_XAXIS; iAxis <= I_ZAXIS; iAxis++)
    mData[iAxis].computeProperties();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Get the min and max values for the given axis
*/ /*
 =======================================*/
std::pair<double, double> MCubesSurface::getMinMaxValue(size_t iAxis) const {
  assert(iAxis == I_XAXIS || iAxis == I_YAXIS || iAxis == I_ZAXIS);
  return std::make_pair(mData[iAxis].getMin(), mData[iAxis].getMax());
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Add a face to the list of faces
*/ /*
 =======================================*/
void MCubesSurface::addFace(size_t iPoint1, size_t iPoint2, size_t iPoint3) {
  assert(mData[I_XAXIS].size() == mData[I_YAXIS].size() &&
         mData[I_XAXIS].size() == mData[I_ZAXIS].size());
/*
assert( iPoint1 < mData[I_XAXIS].size() ) ;
assert( iPoint2 < mData[I_XAXIS].size() ) ;
assert( iPoint3 < mData[I_XAXIS].size() ) ;
*/
#ifdef _DEBUG
  if (iPoint1 >= mData[I_XAXIS].size() || iPoint2 >= mData[I_XAXIS].size() ||
      iPoint3 >= mData[I_XAXIS].size()) {
    bool breakPoint = true;
    breakPoint = breakPoint;
  }
#endif

  MCubesTriangle face;
  face[0] = iPoint1;
  face[1] = iPoint2;
  face[2] = iPoint3;
  mFaceList.push_back(face);
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Remove the last face
*/ /*
 =======================================*/
void MCubesSurface::removeLastFace() { mFaceList.pop_back(); }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   Get the list of surface
*/ /*
 =======================================*/
const std::list<MCubesTriangle> &MCubesSurface::getFaceList() const {
  return mFaceList;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   create predefined sinus
*/ /*
 =======================================*/
#ifdef _DEBUG
MCubesSurface *MCubesSurface::createSinus(double xMin, double xMax, double yMin,
                                          double yMax, double zMin, double zMax,
                                          const unsigned int xNbPoints,
                                          const unsigned int yNbPoints) {
  assert(xNbPoints > 1);
  assert(yNbPoints > 1);

  MCubesSurface *sinusSurface = new MCubesSurface();

  MCubesRange xValueRange(xMin, xMax);
  MCubesRange yValueRange(yMin, yMax);
  MCubesRange zValueRange(zMin, zMax);

  MCubesRange xIndexRange(0.0, (double)(xNbPoints - 1));
  MCubesRange yIndexRange(0.0, (double)(yNbPoints - 1));
  MCubesRange zValueWithoutRescale(-2.0, 2.0);
  MCubesRange twoPiRange(0.0, 2 * PI);

  unsigned int totalNbPoints = xNbPoints * yNbPoints;
  sinusSurface->setNbData(totalNbPoints);

  /////  Compute the x-data  /////

  // The x data are computed here to avoid computing them for each y line
  std::vector<double> xValueArray(xNbPoints);
  std::vector<double> x02PiValueArray(xNbPoints);
  for (unsigned int iX = 0; iX < xNbPoints; iX++) {
    double xIndexValue = (double)iX;
    xValueArray[iX] = xValueRange.getTransformedValue(xIndexRange, xIndexValue);
    x02PiValueArray[iX] =
        twoPiRange.getTransformedValue(xIndexRange, xIndexValue);
  }

  /////  Fill the surface  /////

  unsigned int currentIndex = 0;
  for (unsigned int iY = 0; iY < yNbPoints; iY++) {
    double yIndexValue = (double)iY;
    double yValue = yValueRange.getTransformedValue(yIndexRange, yIndexValue);
    double y02PiValue =
        twoPiRange.getTransformedValue(yIndexRange, yIndexValue);

    for (unsigned int iX = 0; iX < xNbPoints; iX++) {
      double zValue = sin(x02PiValueArray[iX]) + sin(y02PiValue);
      double zValueRescaled =
          zValueRange.getTransformedValue(zValueWithoutRescale, zValue);
      sinusSurface->setData(currentIndex, xValueArray[iX], yValue,
                            zValueRescaled);

      // Create the faces
      if (iY > 0 && iX > 0) {
        unsigned int previousLineIndex = currentIndex - xNbPoints;
        sinusSurface->addFace(currentIndex, currentIndex - 1,
                              previousLineIndex - 1);
        sinusSurface->addFace(currentIndex, previousLineIndex - 1,
                              previousLineIndex);
      }
      currentIndex++;
    }
  }

  return sinusSurface;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesSurface.cpp
*/ /*!   
   create predefined paraboloide
*/ /*
 =======================================*/
MCubesSurface *MCubesSurface::createParaboloide(double xMin, double xMax,
                                                double yMin, double yMax,
                                                double zMin, double zMax,
                                                const unsigned int xNbPoints,
                                                const unsigned int yNbPoints) {
  assert(xNbPoints > 1);
  assert(yNbPoints > 1);

  MCubesSurface *paraboloideSurface = new MCubesSurface();

  MCubesRange xValueRange(xMin, xMax);
  MCubesRange yValueRange(yMin, yMax);
  MCubesRange zValueRange(zMin, zMax);

  MCubesRange xIndexRange(0.0, (double)(xNbPoints - 1));
  MCubesRange yIndexRange(0.0, (double)(yNbPoints - 1));
  double zMaxWithoutRescale =
      xIndexRange.getValue2() * xIndexRange.getValue2() +
      yIndexRange.getValue2() * yIndexRange.getValue2();
  MCubesRange zValueWithoutRescale(0.0, zMaxWithoutRescale);

  unsigned int totalNbPoints = xNbPoints * yNbPoints;
  paraboloideSurface->setNbData(totalNbPoints);

  /////  Compute the x-data  /////

  // The x data are computed here to avoid computing them for each y line
  std::vector<double> xValueArray(xNbPoints);
  for (unsigned int iX = 0; iX < xNbPoints; iX++) {
    double xIndexValue = (double)iX;
    xValueArray[iX] = xValueRange.getTransformedValue(xIndexRange, xIndexValue);
  }

  /////  Fill the surface  /////

  unsigned int currentIndex = 0;
  for (unsigned int iY = 0; iY < yNbPoints; iY++) {
    double yIndexValue = (double)iY;
    double yValue = yValueRange.getTransformedValue(yIndexRange, yIndexValue);

    for (unsigned int iX = 0; iX < xNbPoints; iX++) {
      double xIndexValue = (double)iX;
      double zValue = xIndexValue * xIndexValue + yIndexValue * yIndexValue;
      double zValueRescaled =
          zValueRange.getTransformedValue(zValueWithoutRescale, zValue);
      paraboloideSurface->setData(currentIndex, xValueArray[iX], yValue,
                                  zValueRescaled);

      // Create the faces
      if (iY > 0 && iX > 0) {
        unsigned int previousLineIndex = currentIndex - xNbPoints;
        paraboloideSurface->addFace(currentIndex, currentIndex - 1,
                                    previousLineIndex - 1);
        paraboloideSurface->addFace(currentIndex, previousLineIndex - 1,
                                    previousLineIndex);
      }
      currentIndex++;
    }
  }

  return paraboloideSurface;
}
#endif
