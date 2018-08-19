/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.cpp
*/ /*!   
   Algorithm computing the marching cubes
*/ /*
 =======================================*/

// Class definition
#include "MCubesAlgorithm.h"

#include <cassert>
#include <cmath>

// MCubes
#include "MCubesAllConfigurations.h"
#include "MCubesData.h"
#include "MCubesGrid.h"
#include "MCubesSurface.h"
//#include "MCubesCube.h"
//#include "MCubesTools.h"

const MCubesAllConfigurations MCubesAlgorithm::mAllConfigurations;

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesAlgorithm::MCubesAlgorithm() : mGrid(nullptr), mValues(nullptr) {}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.cpp
*/ /*!   
   Set the grid points
*/ /*
 =======================================*/
void MCubesAlgorithm::setGridPoints(const MCubesGrid *grid) {
  assert(grid == NULL || mValues == NULL ||
         grid->getGridSize() == mValues->size());
  mGrid = grid;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.cpp
*/ /*!   
   Set the values
*/ /*
 =======================================*/
void MCubesAlgorithm::setValues(const MCubesData *values) {
  assert(mGrid == NULL || values == NULL ||
         mGrid->getGridSize() == values->size());
  mValues = values;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.cpp
*/ /*!   
   Create the iso surface using the marching cubes algorithm
*/ /*
 =======================================*/
#ifdef _DEBUG
#define DEBUG_MCUBES
#endif

std::unique_ptr<MCubesSurface>
MCubesAlgorithm::createIsoSurface(double surfaceValue) {
  auto isoSurface = std::make_unique<MCubesSurface>();
  unsigned int nbSurfacePointAllocated = 10000;
  isoSurface->setNbData(nbSurfacePointAllocated);
  unsigned int iLastSurfacePoint = 0;

  const MCubesData &xData = mGrid->getData(I_XAXIS);
  const MCubesData &yData = mGrid->getData(I_YAXIS);
  const MCubesData &zData = mGrid->getData(I_ZAXIS);
  const size_t nbXPoints = xData.size();
  const size_t nbYPoints = yData.size();
  const size_t nbZPoints = zData.size();

  //
  // Link between cube edges and the points of the surface
  // The x and y edges for the previous z are stored
  // so that only one point for the intersection between the iso-surface and the
  // edge is stored. That's why there are xEdgeArray1/xEdgeArray2 and
  // yEdgeArray1/yEdgeArray2
  //
  const size_t nbXEdgeForOneZ = (nbXPoints - 1) * nbYPoints;
  const size_t nbYEdgeForOneZ = (nbYPoints - 1) * nbXPoints;
  const size_t nbZEdgeForOneZ = nbXPoints * nbYPoints;
  std::vector<size_t> xEdgeArray1(
      nbXEdgeForOneZ); // Edges parallel to the x axis
  std::vector<size_t> xEdgeArray2(
      nbXEdgeForOneZ); // Edges parallel to the x axis
  std::vector<size_t> yEdgeArray1(
      nbYEdgeForOneZ); // Edges parallel to the y axis
  std::vector<size_t> yEdgeArray2(
      nbYEdgeForOneZ); // Edges parallel to the y axis
  std::vector<size_t> zEdgeArray(
      nbZEdgeForOneZ); // Edges parallel to the z axis

  //// Initialize the "current" edge arrays

  const size_t UNDEFINED_EDGE = std::numeric_limits<size_t>::min();
  for (size_t iXEdge = 0; iXEdge < nbXEdgeForOneZ; iXEdge++)
    xEdgeArray1[iXEdge] = UNDEFINED_EDGE;
  for (size_t iYEdge = 0; iYEdge < nbYEdgeForOneZ; iYEdge++)
    yEdgeArray1[iYEdge] = UNDEFINED_EDGE;

#ifdef DEBUG_MCUBES
  std::vector<MCubesArray<4, unsigned int>> cubeConfigArray;
  cubeConfigArray.reserve((nbZPoints - 1) * (nbYPoints - 1) * (nbXPoints - 1));
  std::vector<MCubesArray<4, unsigned int>> surfPointIndexArray;
#endif

  ////  Fill the iso-surface  ////

  for (size_t iZ = 0; iZ < nbZPoints - 1; iZ++) {
    bool isInterchange = (iZ % 2 == 1);
    std::vector<size_t> &currentXEdgeArray =
        (isInterchange ? xEdgeArray2 : xEdgeArray1);
    std::vector<size_t> &nextXEdgeArray =
        (isInterchange ? xEdgeArray1 : xEdgeArray2);
    std::vector<size_t> &currentYEdgeArray =
        (isInterchange ? yEdgeArray2 : yEdgeArray1);
    std::vector<size_t> &nextYEdgeArray =
        (isInterchange ? yEdgeArray1 : yEdgeArray2);

    // Initialize the next edges
    for (size_t iXEdge = 0; iXEdge < nbXEdgeForOneZ; iXEdge++)
      nextXEdgeArray[iXEdge] = UNDEFINED_EDGE;
    for (size_t iYEdge = 0; iYEdge < nbYEdgeForOneZ; iYEdge++)
      nextYEdgeArray[iYEdge] = UNDEFINED_EDGE;
    for (size_t iZEdge = 0; iZEdge < nbZEdgeForOneZ; iZEdge++)
      zEdgeArray[iZEdge] = UNDEFINED_EDGE;

    for (size_t iY = 0; iY < nbYPoints - 1; iY++) {
      for (size_t iX = 0; iX < nbXPoints - 1; iX++) {
        //
        // The current cube has the grid coordinates
        //
        //      A7________A6
        //      /|       /|          z
        //    A4________A5|          |  y
        //    |  |      | |          | /
        //    |  |      | |          |/
        //    |  |  DX1 | |          #----- x
        //    | A3______|_A2
        //    |/        |/
        //   A0_________A1
        //        DX0
        //
        // With A0 = iX, iY, iZ
        // and  A6 = iX+1, iY+1 iZ+1
        //

        size_t iCurrentZ = (iZ * nbYPoints + iY) * nbXPoints + iX;
        size_t iNextZ = iCurrentZ + nbZEdgeForOneZ;
        size_t cubeIndex[8] = {iCurrentZ,
                               iCurrentZ + 1,
                               iCurrentZ + 1 + nbXPoints,
                               iCurrentZ + nbXPoints,
                               iNextZ,
                               iNextZ + 1,
                               iNextZ + 1 + nbXPoints,
                               iNextZ + nbXPoints};
        double cubesValues[8];
        for (size_t iValue = 0; iValue < 8; iValue++)
          cubesValues[iValue] = mValues->operator[](cubeIndex[iValue]);

        size_t iConfiguration = mAllConfigurations.getConfiguration(
            cubesValues[0] >= surfaceValue, cubesValues[1] >= surfaceValue,
            cubesValues[2] >= surfaceValue, cubesValues[3] >= surfaceValue,
            cubesValues[4] >= surfaceValue, cubesValues[5] >= surfaceValue,
            cubesValues[6] >= surfaceValue, cubesValues[7] >= surfaceValue);

#ifdef DEBUG_MCUBES
        {
          MCubesArray<4, unsigned int> config;
          config[0] = iX;
          config[1] = iY;
          config[2] = iZ;
          config[3] = iConfiguration;
          cubeConfigArray.push_back(config);
        }
#endif

        size_t nbTriangles = mAllConfigurations.getNbTriangles(iConfiguration);
        if (nbTriangles > 0) {
          ////  Compute the indexes in the surface
          ////  corresponding to the intersecting planes indexes
          std::vector<size_t> surfacePointIndexes(3 * nbTriangles);

          size_t startXIndex = iX + iY * (nbXPoints - 1);
          size_t startYIndex = iX + iY * nbXPoints;
          std::array<size_t *, 12> absoluteEdgeIndexArray = {
              &currentXEdgeArray[startXIndex],
              &currentXEdgeArray[startXIndex + nbXPoints - 1],
              &nextXEdgeArray[startXIndex + nbXPoints - 1],
              &nextXEdgeArray[startXIndex],

              &currentYEdgeArray[startYIndex],
              &currentYEdgeArray[startYIndex + 1],
              &nextYEdgeArray[startYIndex + 1],
              &nextYEdgeArray[startYIndex],

              &zEdgeArray[startYIndex],
              &zEdgeArray[startYIndex + 1],
              &zEdgeArray[startYIndex + nbXPoints + 1],
              &zEdgeArray[startYIndex + nbXPoints]};

          double xPositions[8] = {xData[iX],     xData[iX + 1], xData[iX + 1],
                                  xData[iX],     xData[iX],     xData[iX + 1],
                                  xData[iX + 1], xData[iX]};

          double yPositions[8] = {yData[iY],     yData[iY],    yData[iY + 1],
                                  yData[iY + 1], yData[iY],    yData[iY],
                                  yData[iY + 1], yData[iY + 1]};

          double zPositions[8] = {zData[iZ],     zData[iZ],     zData[iZ],
                                  zData[iZ],     zData[iZ + 1], zData[iZ + 1],
                                  zData[iZ + 1], zData[iZ + 1]};

          const unsigned int *refTriangles =
              mAllConfigurations.getTriangles(iConfiguration);
          for (unsigned int iPoint = 0; iPoint < 3 * nbTriangles; iPoint++) {
            unsigned int relativeEdge = refTriangles[iPoint];
            size_t &iCurrentSurfacePoint =
                *absoluteEdgeIndexArray[relativeEdge];
#ifdef DEBUG_MCUBES
            {
              bool isAlreadyTreatedInThisCube = false;
              for (unsigned int iAuxPoint = 0; iAuxPoint < iPoint;
                   iAuxPoint++) {
                if (refTriangles[iAuxPoint] == relativeEdge) {
                  isAlreadyTreatedInThisCube = true;
                  break;
                }
              }

              assert(!isAlreadyTreatedInThisCube ||
                     iCurrentSurfacePoint != UNDEFINED_EDGE);
              if (!isAlreadyTreatedInThisCube) {
                bool isXEdgeNotComputed =
                    (relativeEdge == 2 || (iY == 0 && relativeEdge == 3) ||
                     (iZ == 0 &&
                      (relativeEdge == 1 || (iY == 0 && relativeEdge == 0))));

                bool isYEdgeNotComputed =
                    (relativeEdge == 6 || (iX == 0 && relativeEdge == 7) ||
                     (iZ == 0 &&
                      (relativeEdge == 5 || (iX == 0 && relativeEdge == 4))));

                bool isZEdgeNotComputed =
                    (relativeEdge == 10 || (iX == 0 && relativeEdge == 11) ||
                     (iY == 0 &&
                      (relativeEdge == 9 || (iX == 0 && relativeEdge == 8))));

                bool edgeNotComputed = isXEdgeNotComputed ||
                                       isYEdgeNotComputed || isZEdgeNotComputed;

                if ((iCurrentSurfacePoint == UNDEFINED_EDGE &&
                     !edgeNotComputed) ||
                    (iCurrentSurfacePoint != UNDEFINED_EDGE &&
                     edgeNotComputed)) {
                  bool breakPoint = true;
                  breakPoint = breakPoint;
                }
              }
            }
#endif
            if (iCurrentSurfacePoint == UNDEFINED_EDGE) {
              const auto &pointIndexes =
                  MCubesCubeIndexes::edgeToPoint(relativeEdge);
              // The iso surface is crossing the edge
              // => the values cannot be equal on the vertices
              assert(fabs(cubesValues[pointIndexes[1]] -
                          cubesValues[pointIndexes[0]]) > 0.0);
              double interpFactor =
                  (surfaceValue - cubesValues[pointIndexes[0]]) /
                  (cubesValues[pointIndexes[1]] - cubesValues[pointIndexes[0]]);
              assert(interpFactor >= 0.0 && interpFactor <= 1.0);
              double xPos = interpFactor * (xPositions[pointIndexes[1]] -
                                            xPositions[pointIndexes[0]]) +
                            xPositions[pointIndexes[0]];
              double yPos = interpFactor * (yPositions[pointIndexes[1]] -
                                            yPositions[pointIndexes[0]]) +
                            yPositions[pointIndexes[0]];
              double zPos = interpFactor * (zPositions[pointIndexes[1]] -
                                            zPositions[pointIndexes[0]]) +
                            zPositions[pointIndexes[0]];

              if (iLastSurfacePoint == nbSurfacePointAllocated) {
                nbSurfacePointAllocated *= 2;
                isoSurface->setNbData(nbSurfacePointAllocated);
              }

              isoSurface->setData(iLastSurfacePoint, xPos, yPos, zPos);
#ifdef DEBUG_MCUBES
              {
                surfPointIndexArray.resize(iLastSurfacePoint + 1);
                surfPointIndexArray[iLastSurfacePoint][0] = iX;
                surfPointIndexArray[iLastSurfacePoint][1] = iY;
                surfPointIndexArray[iLastSurfacePoint][2] = iZ;
                surfPointIndexArray[iLastSurfacePoint][3] = relativeEdge;
              }
#endif

              iCurrentSurfacePoint = iLastSurfacePoint;
              iLastSurfacePoint++;
            }

            surfacePointIndexes[iPoint] = iCurrentSurfacePoint;
          }

          ////  Add the triangles in the surface
          for (unsigned int iTriangle = 0; iTriangle < nbTriangles;
               iTriangle++) {
            size_t *triangle = surfacePointIndexes.data() + 3 * iTriangle;
            isoSurface->addFace(triangle[0], triangle[1], triangle[2]);
          }
        }
      }
    }
  }

  isoSurface->setNbData(iLastSurfacePoint);
  isoSurface->updateProperties();

  return isoSurface;
}
