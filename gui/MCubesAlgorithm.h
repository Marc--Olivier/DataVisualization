/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesAlgorithm.h
*/ /*!   
   Algorithm computing the marching cubes
   // Grid containing 3D points with their values
*/ /*
 =======================================*/
#pragma once

#include <memory>
#include <vector>

class MCubesData;
class MCubesSurface;
class MCubesGrid;
class MCubesAllConfigurations;

class MCubesAlgorithm {

  /*==================
      Constructors
  *==================*/
public:
  MCubesAlgorithm();

  /*==================
       Points
  *==================*/
private:
  const MCubesGrid *mGrid;
  const MCubesData *mValues;

public:
  void setGridPoints(const MCubesGrid *grid);
  void setValues(const MCubesData *values);

  /*==================
       Iso surface
  *==================*/
public:
  std::unique_ptr<MCubesSurface> createIsoSurface(double surfaceValue);

  /*==================
    Auxiliary definitions
    for the marching
    cubes algorithm
  *==================*/
private:
  static const MCubesAllConfigurations mAllConfigurations; // agregate variable
};
