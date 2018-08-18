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

class MCubesData;
class MCubesGrid;

class ReadDicom {
  friend class DicomReader;

public:
  ReadDicom();
  ~ReadDicom();
  ReadDicom(ReadDicom &&);

public:
  std::unique_ptr<MCubesGrid> releaseGrid() { return std::move(mGrid); }
  std::unique_ptr<MCubesData> releaseValues() { return std::move(mValues); }
  const std::list<std::string> &getErrorFileNameList() const {
    return mErrorFileNameList;
  }

private:
  std::unique_ptr<MCubesGrid> mGrid;
  std::unique_ptr<MCubesData> mValues;
  std::list<std::string> mErrorFileNameList;
};

class DicomReader {

public:
  static ReadDicom readFiles(const std::list<std::string> &fileNameList);

private:
  static void readOneFile(const std::string &fileName, ReadDicom &readDicom);
};
