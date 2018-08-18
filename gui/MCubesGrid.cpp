/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Grid of points 
*/ /*
 =======================================*/

// Class definition
#include "MCubesGrid.h"

// C / C++
#include <assert.h>

// MCubes
#include "MCubesData.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesGrid::MCubesGrid() { init(); }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Destructor
*/ /*
 =======================================*/
MCubesGrid::~MCubesGrid() {}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Initialization method
*/ /*
 =======================================*/
void MCubesGrid::init() {
  assert(mData[I_XAXIS].size() == 0);
  assert(mData[I_YAXIS].size() == 0);
  assert(mData[I_ZAXIS].size() == 0);
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Create cubes
*/ /*
 =======================================*/
std::unique_ptr<MCubesGrid>
MCubesGrid::createGrid(double xMin, double xMax, double yMin, double yMax,
                       double zMin, double zMax, const unsigned int xNbPoints,
                       const unsigned int yNbPoints,
                       const unsigned int zNbPoints) {
  assert(xNbPoints > 1);
  assert(yNbPoints > 1);
  assert(zNbPoints > 1);

  auto grid = std::make_unique<MCubesGrid>();
  const double min[3] = {xMin, yMin, zMin};
  const double max[3] = {xMax, yMax, zMax};
  const unsigned int nbPoints[3] = {xNbPoints, yNbPoints, zNbPoints};

  for (unsigned int iAxis = I_XAXIS; iAxis <= I_ZAXIS; iAxis++) {
    MCubesData &data = grid->getData(iAxis);
    data.resize(nbPoints[iAxis]);
    MCubesRange valueRange(min[iAxis], max[iAxis]);
    MCubesRange indexRange(0.0, (double)(nbPoints[iAxis] - 1));

    /////  Compute the data  /////
    for (unsigned int iData = 0; iData < nbPoints[iAxis]; iData++)
      data[iData] = valueRange.getTransformedValue(indexRange, (double)iData);

    data.computeProperties();
  }

  return grid;
}
