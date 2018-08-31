#include "DicomReader.h"

#include <cassert>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <numeric>

#include "marching-cubes/Tensor3D.hpp"

DicomData::DicomData(std::unique_ptr<marchingcubes::Grid3D> grid,
                     std::unique_ptr<marchingcubes::Tensor3D> values,
                     const std::list<std::string> &errorFileNameList)
    : mGrid{std::move(grid)}, mValues{std::move(values)},
      mErrorFileNameList{errorFileNameList} {}

DicomData::~DicomData() = default;

DicomData::DicomData(DicomData &&) = default;

DicomData DicomReader::readFiles(const std::list<std::string> &fileNameList) {
  std::for_each(fileNameList.begin(), fileNameList.end(),
                [&](const std::string &fileName) { readOneFile(fileName); });
  auto dimX = x.size();
  auto dimY = y.size();
  auto dimZ = z.size();
  auto grid = std::make_unique<marchingcubes::Grid3D>(
      std::move(x), std::move(y), std::move(z));
  auto tensor3D = std::make_unique<marchingcubes::Tensor3D>(dimX, dimY, dimZ,
                                                            std::move(values));
  return DicomData{std::move(grid), std::move(tensor3D), errorFileNameList};
}

void DicomReader::readOneFile(const std::string &fileName) {
  // Open the file
  DicomImage image(fileName.data());

  if (image.getStatus() != EIS_Normal) {
    errorFileNameList.push_back(fileName);
    return;
  }

  // addLogMessage( QString( "Read file \"%1\"" ).arg( fileName ) ) ;
  auto width = image.getWidth();
  auto height = image.getHeight();
  auto nbFrames = image.getFrameCount();
  assert(width != 0 && height != 0 && nbFrames != 0);

  if (z.empty()) {
    x.resize(width);
    y.resize(height);
    std::iota(x.begin(), x.end(), 0);
    std::iota(y.begin(), y.end(), 0);
  } else {
    if (x.size() != width || y.size() == height) {
      errorFileNameList.push_back(fileName);
      return;
    }
  }

  auto prevFramesCount = z.size();
  z.resize(prevFramesCount + nbFrames);
  std::iota(z.begin() + static_cast<long>(prevFramesCount), z.end(),
            prevFramesCount);

  auto pointCountOnOneFrame = width * height;
  auto totalPointCount = pointCountOnOneFrame * z.size();
  values.resize(totalPointCount);

  int isMonochrome = image.isMonochrome();
#if 0
  {
    auto memorySize = image.getOutputDataSize();
    auto memoryIncrement = memorySize / pointCountOnOneFrame;
    if (!((isMonochrome && memoryIncrement != 1) ||
          (!isMonochrome && memoryIncrement == 2))) {
      throw std::runtime_error("Invalid image size");
    }

    assert((isMonochrome && memoryIncrement == 1) ||
           (!isMonochrome && memoryIncrement == 2));
  }
#endif

  auto index = prevFramesCount * pointCountOnOneFrame;
  for (unsigned int iFrame = 0; iFrame < nbFrames; iFrame++) {
    const void *frameBuffer = image.getOutputData(0, iFrame);

    if (isMonochrome) {
      const char *frameParser = static_cast<const char *>(frameBuffer);
      for (unsigned int iY = 0; iY < height; iY++) {
        for (unsigned int iX = 0; iX < width; iX++) {
          values[index++] = *frameParser;
          frameParser++;
        }
      }
    } else {
      const Uint16 *frameParser = static_cast<const Uint16 *>(frameBuffer);
      for (unsigned int iY = 0; iY < height; iY++) {
        for (unsigned int iX = 0; iX < width; iX++) {
          values[index++] = *frameParser;
          frameParser++;
        }
      }
    }
  }
  if (index != totalPointCount) {
    throw std::runtime_error("index " + std::to_string(index) +
                             " does not match totalPointCount " +
                             std::to_string(totalPointCount));
  }
}
