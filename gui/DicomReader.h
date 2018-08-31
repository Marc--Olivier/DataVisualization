/*=======================================*/
/**
   \author M.O. Andrez
   \date   14/07/2010
   \file   MCubesGridDicom.h
*/ /*!   
   Grid of points constructed from a DICOM file
*/ /*
 =======================================*/
#pragma once

#include <list>
#include <memory>
#include <vector>

namespace marchingcubes {
class Grid3D;
class Tensor3D;
} // namespace marchingcubes

/*!
 * \class DicomData
 * \brief The DicomData class stores DICOM data as a tensor 3D and a grid 3D.
 *
 * DicomData objects are usually created by a DicomReader.
 */
class DicomData {

public:
  DicomData(std::unique_ptr<marchingcubes::Grid3D> grid,
            std::unique_ptr<marchingcubes::Tensor3D> values,
            const std::list<std::string> &errorFileNameList);
  ~DicomData();
  DicomData(DicomData &&);

public:
  std::unique_ptr<marchingcubes::Grid3D> releaseGrid() {
    return std::move(mGrid);
  }
  std::unique_ptr<marchingcubes::Tensor3D> releaseValues() {
    return std::move(mValues);
  }
  const std::list<std::string> &getErrorFileNameList() const {
    return mErrorFileNameList;
  }

private:
  std::unique_ptr<marchingcubes::Grid3D> mGrid;
  std::unique_ptr<marchingcubes::Tensor3D> mValues;
  std::list<std::string> mErrorFileNameList;
};

/*!
 * \class DicomReader
 * \brief The DicomReader class read DICOM files and creates a DicomData object
 * from them.
 */
class DicomReader {

public:
  DicomData readFiles(const std::list<std::string> &fileNameList);

private:
  void readOneFile(const std::string &fileName);

private:
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  std::vector<double> values;
  std::list<std::string> errorFileNameList;
};
