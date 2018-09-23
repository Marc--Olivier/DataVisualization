/*=======================================*/
/**
   \author M.O. Andrez
   \date   14/07/2010
   \file   MCubesGridDicom.cpp
*/ /*!   
   Grid of points constructed from a DICOM file
*/ /*
 =======================================*/

// Class definition
#include "DicomReader.h"

// C / C++
#include <assert.h>

// DICOM
#include "dcmtk/dcmimgle/dcmimage.h"

// MCubes
#include "MCubesData.h"
#include "MCubesGrid.h"

ReadDicom::ReadDicom() : mGrid(new MCubesGrid()), mValues(new MCubesData()) {}
ReadDicom::~ReadDicom() = default;
ReadDicom::ReadDicom(ReadDicom &&) = default;

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
ReadDicom DicomReader::readFiles(const std::list<std::string> &fileNameList) {
  ReadDicom readDicom;
  std::for_each(fileNameList.begin(), fileNameList.end(),
                [&readDicom](const std::string &fileName) {
                  readOneFile(fileName, readDicom);
                });
  return readDicom;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   16/07/2010
   \file   MCubesGrid.cpp
*/ /*!   
   Read one DICOM file
   Documentation of DicomImage: http://support.dcmtk.org/docs/classDicomImage.html 
*/ /*
 =======================================*/
// void MCubesGridDicom::readOneFile( const char * fileName  )
void DicomReader::readOneFile(const std::string &fileName,
                              ReadDicom &readDicom) {
  // Open the file
  DicomImage image(fileName.data());

  if (image.getStatus() != EIS_Normal) {
    readDicom.mErrorFileNameList.push_back(fileName);
    return;
  }

  // addLogMessage( QString( "Read file \"%1\"" ).arg( fileName ) ) ;
  auto width = image.getWidth();
  auto height = image.getHeight();
  auto nbFrames = image.getFrameCount();
  assert(width != 0 && height != 0 && nbFrames != 0);

  MCubesData &xData = readDicom.mGrid->getData(I_XAXIS);
  MCubesData &yData = readDicom.mGrid->getData(I_YAXIS);
  MCubesData &zData = readDicom.mGrid->getData(I_ZAXIS);
  auto iZStart = zData.size();

  if (iZStart == 0) {
    xData.resize(width);
    yData.resize(height);

    // Set the x, y and z values
    for (size_t iX = 0; iX < width; iX++)
      xData[iX] = iX;

    for (size_t iY = 0; iY < height; iY++)
      yData[iY] = iY;
  }

  else {
    bool isWidthValid = (width == xData.size());
    bool isHeightValid = (height == yData.size());
    if (!isWidthValid || !isHeightValid) {
      readDicom.mErrorFileNameList.push_back(fileName);
      return;
    }
  }

  auto nbTotalFrames = iZStart + nbFrames;
  zData.resize(nbTotalFrames);

  for (auto iZ = iZStart; iZ < nbTotalFrames; iZ++)
    zData[iZ] = iZ;

  ////  Get the images values

  auto nbPointsOneSlice = width * height;
  auto nbTotalPoints = nbPointsOneSlice * nbTotalFrames;
  auto &values = *readDicom.mValues;
  values.resize(nbTotalPoints);

  int isMonochrome = image.isMonochrome();
#ifdef _DEBUG
  {
    unsigned int memorySize = image.getOutputDataSize();
    unsigned int memoryIncrement = memorySize / nbPointsOneSlice;
    assert((isMonochrome && memoryIncrement == 1) ||
           (!isMonochrome && memoryIncrement == 2));
  }
#endif

  auto iGlobalIndex = iZStart * nbPointsOneSlice;
  for (unsigned int iFrame = 0; iFrame < nbFrames; iFrame++) {
    const void *frameBuffer = image.getOutputData(0, iFrame);

    if (isMonochrome) {
      const char *frameParser = static_cast<const char *>(frameBuffer);
      for (unsigned int iY = 0; iY < height; iY++) {
        for (unsigned int iX = 0; iX < width; iX++) {
          values[iGlobalIndex++] = *frameParser;
          frameParser++;
        }
      }
    } else {
      const Uint16 *frameParser = static_cast<const Uint16 *>(frameBuffer);
      for (unsigned int iY = 0; iY < height; iY++) {
        for (unsigned int iX = 0; iX < width; iX++) {
          values[iGlobalIndex++] = *frameParser;
          frameParser++;
        }
      }
    }
  }
  assert(iGlobalIndex == nbTotalPoints);

  xData.computeProperties();
  yData.computeProperties();
  zData.computeProperties();
  values.computeProperties();
}
