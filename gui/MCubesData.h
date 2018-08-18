/*=======================================*/
/**
   \author M.O. Andrez
   \date   07/07/2010
   \file   MCubesData.h
*/ /*!   
   Array of double
*/ /*
 =======================================*/
#ifndef MCubesData_H
#define MCubesData_H

// Inherits
#include <vector>

// Qt

// MCubes
#include "MCubes.h"

class MCubesData : public std::vector<double> {

  /*==================
      Constructors
  *==================*/
public:
  MCubesData();
  MCubesData(const MCubesData &other) = default;

private:
  void init();

  /*==================
      Data Properties
  *==================*/
public:
  inline size_t size() const { return std::vector<double>::size(); }

public:
  void computeProperties() const;
  void updateProperties(unsigned int startIndex,
                        unsigned int nbUpdatedElements) const;

  // These variables are used to reduce the number of operations
  // They can be modified without changing the data
  // that's why they are mutable
private:
  mutable double mMinValue;
  mutable double mMaxValue;

public:
  INLINE double getMin() const { return mMinValue; }
  INLINE double getMax() const { return mMaxValue; }
};

#endif
