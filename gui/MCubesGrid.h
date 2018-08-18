/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.h
*/ /*!   
   Grid of points 
*/ /*
 =======================================*/
#ifndef MCubesGrid_H
#define MCubesGrid_H

// C / C++
#include <array>
#include <list>
#include <memory>
#include <vector>

// MCubes
// #include "MCubes.h"
// #include "MCubesTools.h"
#include "MCubesCube.h"
#include "MCubesData.h"

class MCubesGrid {

  /*==================
      Constructors
  *==================*/
public:
  MCubesGrid();
  virtual ~MCubesGrid();

private:
  void init();

  /*==================
         Data
  *==================*/
private:
  std::array<MCubesData, 3> mData;

public:
  const MCubesData &getData(size_t iAxis) const { return mData[iAxis]; }
  MCubesData &getData(size_t iAxis) { return mData[iAxis]; }
  size_t getGridSize() const {
    return mData[I_XAXIS].size() * mData[I_YAXIS].size() *
           mData[I_ZAXIS].size();
  }

public:
  size_t getGlobalIndex(size_t iX, size_t iY, size_t iZ) {
    return (iZ * mData[I_YAXIS].size() + iY) * mData[I_XAXIS].size() + iX;
  }

  /*==================
     Scan the grid
  *==================*/
public:
  template <class Functor> void scanAllPoints(Functor &functor) const;

  /*==================
    Predefined grids
  *==================*/
public:
  static std::unique_ptr<MCubesGrid>
  createGrid(double xMin, double xMax, double yMin, double yMax, double zMin,
             double zMax, const unsigned int xNbPoints,
             const unsigned int yNbPoints, const unsigned int zNbPoints);
};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.h
*/ /*!   
   Apply the given functor to all the points 
*/ /*
 =======================================*/
template <class Functor>
void MCubesGrid::scanAllPoints(Functor &functor) const {
  functor.setGrid(*this);
  for (unsigned int iZ = 0; iZ < mData[I_ZAXIS].size(); iZ++) {
    for (unsigned int iY = 0; iY < mData[I_YAXIS].size(); iY++) {
      for (unsigned int iX = 0; iX < mData[I_XAXIS].size(); iX++) {
        functor.execute(mData[I_XAXIS][iX], mData[I_YAXIS][iY],
                        mData[I_ZAXIS][iZ]);
      }
    }
  }
}

#endif
