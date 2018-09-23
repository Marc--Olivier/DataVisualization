/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesAllConfigurations.h
*/ /*!   
   Contains all the marching cubes configurations 
*/ /*
 =======================================*/
#ifndef MCubesAllConfigurations_H
#define MCubesAllConfigurations_H

// Inherits
#include "MCubesConfigurationArray.h"

// C / C++

// MCubes
#include "MCubes.h"
// #include "MCubesCube.h"
class MCubesCubeSigns;

class MCubesAllConfigurations : public MCubesConfigurationArray {
public:
  MCubesAllConfigurations();

private:
  void initAllConfigurations();
};

#endif
