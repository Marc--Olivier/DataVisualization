/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.cpp
*/ /*!   
   Array of double
*/ /*
 =======================================*/

// Class definition
#include "MCubesData.h"

// C / C++
#include <assert.h>
#include <float.h>

// Qt

// MCubes

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesData::MCubesData() : std::vector<double>() { init(); }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.cpp
*/ /*!   
   Initialization method
*/ /*
 =======================================*/
void MCubesData::init() {
  mMinValue = DBL_MAX;
  mMaxValue = -mMinValue;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.cpp
*/ /*!   
   Update the data properties (min-max, etc) 
   using data between startIndex and startIndex+nbUpdatedElements-1
*/ /*
 =======================================*/
void MCubesData::updateProperties(unsigned int startIndex,
                                  unsigned int nbUpdatedElements) const {
  unsigned int arraySize = size();
  assert(nbUpdatedElements > 0 && startIndex < arraySize);

  unsigned int endIndex = startIndex + nbUpdatedElements - 1;
  if (endIndex >= arraySize)
    endIndex = arraySize - 1;

  for (unsigned int iData = startIndex; iData <= endIndex; iData++) {
    double currentData = operator[](iData);

    ////  Min-max
    if (currentData > mMaxValue)
      mMaxValue = currentData;
    if (currentData < mMinValue)
      mMinValue = currentData;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.cpp
*/ /*!   
   compute the data properties (min-max, etc) 
*/ /*
 =======================================*/
void MCubesData::computeProperties() const {
  mMinValue = DBL_MAX;
  mMaxValue = -mMinValue;
  if (size() > 0)
    MCubesData::updateProperties(0, size());
}
