/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.h
*/ /*!   
   Definitions
*/ /*
 =======================================*/

// Class definition
#include "MCubesTools.h"

// C / C++
#include <math.h>

// Qt

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.cpp
*/ /*!   
   Global variables
*/ /*
 =======================================*/
extern const unsigned int I_XAXIS = 0;
extern const unsigned int I_YAXIS = I_XAXIS + 1;
extern const unsigned int I_ZAXIS = I_YAXIS + 1;
extern const unsigned int I_VALUE_AXIS = I_ZAXIS + 1;

extern const double PI = 4.0 * atan(1.0);

/*=======================================*/
/*=======================================*/
/*=======================================*/

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.cpp
*/ /*!   
   Range, able to execute linear transformations
*/ /*
 =======================================*/

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesRange::MCubesRange() : mValue1(0.0), mValue2(0.0), mRangeSize(0.0) {}

MCubesRange::MCubesRange(const MCubesRange &transformer)
    : mValue1(transformer.mValue1), mValue2(transformer.mValue2),
      mRangeSize(transformer.mRangeSize) {}

MCubesRange::MCubesRange(const double value1, const double value2)
    : mValue1(value1), mValue2(0.0), mRangeSize(0.0) {
  setValue2(value2);
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.cpp
*/ /*!   
   Destructor
*/ /*
 =======================================*/
MCubesRange::~MCubesRange() {}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.cpp
*/ /*!   
   Setting the values
*/ /*
 =======================================*/
void MCubesRange::setValue1(double value) {
  mValue1 = value;
  mRangeSize = mValue2 - mValue1;
}

void MCubesRange::setValue2(double value) {
  mValue2 = value;
  mRangeSize = mValue2 - mValue1;
}

/*=======================================*/
/*=======================================*/
/*=======================================*/
