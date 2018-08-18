/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesAllConfigurations.cpp
*/ /*!   
   Contains all the marching cubes configurations 
*/ /*
 =======================================*/

// Class definition
#include "MCubesAllConfigurations.h"

// C / C++
#include <assert.h>

// MCubes
#include "MCubesCube.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesAllConfigurations.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesAllConfigurations::MCubesAllConfigurations()
    : MCubesConfigurationArray(NB_MAX_CONFIG) {
  // Initialization done in the generated file MCubesAllConfigurations_init.cpp
  initAllConfigurations();
}
