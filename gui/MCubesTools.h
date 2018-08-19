/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.h
*/ /*!   
   Definitions and tools
*/ /*
 =======================================*/
#pragma once

#include "MCubes.h"

#include <array>

extern const unsigned int I_XAXIS;
extern const unsigned int I_YAXIS;
extern const unsigned int I_ZAXIS;
extern const unsigned int I_VALUE_AXIS;

extern const double PI;

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesTools.h
*/ /*!   
   Triangle
*/ /*
 =======================================*/
class MCubesTriangle : public std::array<size_t, 3> {};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesTools.h
*/ /*!   
   Range, able to execute linear transformations
*/ /*
 =======================================*/
class MCubesRange {

  /*==================
      Constructors
  *==================*/
public:
  MCubesRange();
  MCubesRange(const MCubesRange &otherRange);
  MCubesRange(const double value1, const double value2);
  ~MCubesRange();

  /*==================
        Points
  *==================*/
private:
  double mValue1;
  double mValue2;
  double mRangeSize;

public:
  INLINE double getValue1() const { return mValue1; }
  INLINE double getValue2() const { return mValue2; }
  INLINE double getRange() const { return mRangeSize; }
  void setValue1(double value);
  void setValue2(double value);

  /*=======================================*/
  /**
      \author M.O. Andrez
      \date   07/07/2010
      \file   MCubesTools.h
   */ /*!   
      Get the affine transformation from the value into the other range
      to the value of this range.
      Property: (valueCurrentRange - mValue1)/(mValue2-mValue1) 
                  == (valueOtherRange - otherRange->mValue1)/(otherRange->mValue2-otherRange->mValue1)
   */ /*
    =======================================*/
  INLINE double getTransformedValue(const MCubesRange &otherRange,
                                    double valueInOtherRange) const {
    return (valueInOtherRange - otherRange.mValue1) * mRangeSize /
               otherRange.mRangeSize +
           mValue1;
  }

  /*
  static INLINE double getTransformedValue( const double value1Range1, const
  double value2Range1, const double value1Range2, const double value2Range2,
                                            const double valueInRange1 )
  {
     return ( valueInRange1 - value1Range1)* ( value2Range2-value1Range2 ) /
              (value2Range1-value1Range1) + value1Range2 ;
  } ;
  */
};
