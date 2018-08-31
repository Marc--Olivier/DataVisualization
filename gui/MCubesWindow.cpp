/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Main window for the marching cubes rendering
*/ /*
 =======================================*/

// Class definition
#include "gui/MCubesWindow.h"

#include "marching-cubes/MarchingCubes.hpp"
#include "marching-cubes/Tensor3D.hpp"

// C / C++
#include <cassert>
#include <cmath>

// Qt
#include <QAction>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include <QTextEdit>
#include <QTime>
#include <QToolBar>
#include <QVBoxLayout>

// MCubes
#include "gui/DicomReader.h"
#include "gui/MCubesRenderer.h"
#include "gui/MCubesTools.h"

MCubesWindow::MCubesWindow(QWidget *parentWidget, Qt::WindowFlags flags)
    : QMainWindow(parentWidget, flags),
      mMarchingCubes(new marchingcubes::MarchingCubes{}) {

  setWindowTitle(QObject::tr("DICOM 3D renderer"));

  resize(500, 400);
  QWidget *theCentralWidget = new QWidget(this);
  // theCentralWidget->setSizePolicy( QSizePolicy::MinimumExpanding,
  // QSizePolicy::MinimumExpanding );
  setCentralWidget(theCentralWidget);

  delete theCentralWidget->layout();
  QVBoxLayout *centralLayout = new QVBoxLayout(theCentralWidget);

  // Renderer
  {
    mRenderer = new MCubesRenderer(theCentralWidget);
    mRenderer->setSizePolicy(QSizePolicy::MinimumExpanding,
                             QSizePolicy::MinimumExpanding);
    centralLayout->addWidget(mRenderer);
    mRenderer->setIsoXYZ(true);
  }

  // Log
  {
    mLogWidget = new QTextEdit(theCentralWidget);
    mLogWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Fixed);
    mLogWidget->setFixedHeight(60);
    centralLayout->addWidget(mLogWidget);
  }

  // Marching cubes
  {
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(toolBar);

    {
      QAction *openAction = new QAction(
          QIcon(":/gui_resources/images/open.png"), QObject::tr("Open"), this);
      openAction->setToolTip(QObject::tr("Open DICOM files"));
      toolBar->addAction(openAction);
      QObject::connect(openAction, SIGNAL(triggered()), this,
                       SLOT(slotOpenFile()));
    }
    {
      QAction *testAction =
          new QAction(QIcon(":/gui_resources/images/sphere.png"),
                      QObject::tr("Sphere"), this);
      testAction->setToolTip(
          QObject::tr("Create grid and compute f=x²+y²+z² values"));
      toolBar->addAction(testAction);
      QObject::connect(testAction, SIGNAL(triggered()), this,
                       SLOT(slotTestSphere()));
    }

    // Manage the value of the iso-surface
    {
      toolBar->insertSeparator(nullptr);
      mIsoSurfaceWidget = new QWidget(toolBar);
      delete mIsoSurfaceWidget->layout();
      QHBoxLayout *boxLayout = new QHBoxLayout(mIsoSurfaceWidget);
      mIsoSurfaceWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                                       QSizePolicy::MinimumExpanding);
      toolBar->addWidget(mIsoSurfaceWidget);

      // Label
      {
        QLabel *isoValueLable =
            new QLabel(QObject::tr("Iso-value: "), mIsoSurfaceWidget);
        isoValueLable->setSizePolicy(QSizePolicy::Minimum,
                                     QSizePolicy::MinimumExpanding);
        boxLayout->addWidget(isoValueLable);
      }

      // Slider
      {
        mIsoValueSlider = new QSlider(Qt::Horizontal, mIsoSurfaceWidget);
        // mIsoValueSlider->setTickPosition( QSlider::TicksBelow ) ;
        mIsoValueSlider->setSizePolicy(QSizePolicy::MinimumExpanding,
                                       QSizePolicy::MinimumExpanding);
        boxLayout->addWidget(mIsoValueSlider);
        QObject::connect(mIsoValueSlider, SIGNAL(valueChanged(int)), this,
                         SLOT(slotSliderValueChanged(int)));
      }

      // Spin box
      {
        mIsoValueSpinBox = new QDoubleSpinBox(mIsoSurfaceWidget);
        boxLayout->addWidget(mIsoValueSpinBox);
        QObject::connect(mIsoValueSpinBox, SIGNAL(valueChanged(double)), this,
                         SLOT(slotSpinBoxValueChanged(double)));
      }

      mIsoSurfaceWidget->setDisabled(true);
    }
  }
}

MCubesWindow::~MCubesWindow() = default;

void MCubesWindow::addLogMessage(const QString &message) {
  mLogWidget->append(message);
  mLogWidget->repaint();
}

void MCubesWindow::clearLogMessage() {
  mLogWidget->clear();
  mLogWidget->repaint();
}

void MCubesWindow::slotSliderValueChanged(int value) {
  assert(mCurrentTensor != NULL);
  MCubesRange sliderRange(mIsoValueSlider->minimum(),
                          mIsoValueSlider->maximum());
  MCubesRange isoValueRange(tensorMin, tensorMax);
  double isoValue = isoValueRange.getTransformedValue(sliderRange, value);

  setIsoValue(isoValue);
}

void MCubesWindow::slotSpinBoxValueChanged(double value) { setIsoValue(value); }

void MCubesWindow::slotTestSphere() {

  using namespace marchingcubes;

  // Create grid with f=x²+y²+z² values
  unsigned int nbXPoints = 101;
  unsigned int nbYPoints = 101;
  unsigned int nbZPoints = 101;

  clearLogMessage();
  QTime timer;
  timer.start();

  auto grid = std::make_unique<marchingcubes::Grid3D>(
      equidistantPoints(-1.0, 1.0, nbXPoints),
      equidistantPoints(-2.0, 2.0, nbYPoints),
      equidistantPoints(-3.0, 3.0, nbZPoints));
  addLogMessage(QString("Created grid (%1,%2,%3)")
                    .arg(nbXPoints)
                    .arg(nbYPoints)
                    .arg(nbZPoints));

  auto tensor = std::make_unique<marchingcubes::Tensor3D>(
      marchingcubes::createSphere(*grid));
  addLogMessage(
      QString("Filled %1 points using the f=x²+y²+z² sphere function in %2 ms")
          .arg(nbXPoints * nbYPoints * nbZPoints)
          .arg(timer.elapsed()));

  setTensor(std::move(grid), std::move(tensor));
}

void MCubesWindow::slotOpenFile() {

  // addLogMessage( QString( "\n=================\n" ) ) ;
  clearLogMessage();
  QStringList fileList =
      QFileDialog::getOpenFileNames(this, QObject::tr("Select DICOM files"),
                                    QString(), "DICOM (*.dcm);; All (*)");

  if (fileList.count() == 0) {
    addLogMessage(QObject::tr("No file selected"));
    return;
  }

  QTime timer;
  timer.start();

  fileList.sort();
  std::list<std::string> fileNameList;
  foreach (const QString &fileName, fileList)
    fileNameList.push_back(fileName.toStdString());

  auto dicomData = DicomReader{}.readFiles(fileNameList);
  const auto &errorFileNameList = dicomData.getErrorFileNameList();
  QStringList notReadFiles;
  for (std::list<std::string>::const_iterator itFile =
           errorFileNameList.begin();
       itFile != errorFileNameList.end(); itFile++)
    notReadFiles.append(QString::fromStdString(*itFile));

  if (notReadFiles.size() > 0) {
    addLogMessage(
        QObject::tr("Cannot read \"%1\"").arg(notReadFiles.join("\", \"")));
  }

  QStringList readFileList = fileList;
  foreach (const QString &fileName, notReadFiles)
    readFileList.removeOne(fileName);

  if (readFileList.count() == 0) {
    addLogMessage(QObject::tr("No file read"));
    return;
  } else {
    addLogMessage(QObject::tr("\"%1\" read in %2 ms")
                      .arg(readFileList.join("\", \""))
                      .arg(timer.elapsed()));
  }

  setTensor(dicomData.releaseGrid(), dicomData.releaseValues());
}

void MCubesWindow::setTensor(std::unique_ptr<marchingcubes::Grid3D> grid,
                             std::unique_ptr<marchingcubes::Tensor3D> tensor) {
  assert(grid != nullptr);
  assert(tensor != nullptr);
  mIsoSurfaceWidget->setEnabled(true);

  mCurrentGrid = std::move(grid);
  mCurrentTensor = std::move(tensor);

  std::tie(tensorMin, tensorMax) = mCurrentTensor->minMax();

  addLogMessage(QObject::tr("Grid size = %1 x %2 x %3")
                    .arg(mCurrentGrid->values[I_XAXIS].size())
                    .arg(mCurrentGrid->values[I_YAXIS].size())
                    .arg(mCurrentGrid->values[I_ZAXIS].size()));
  addLogMessage(
      QString("tensorMin=%1, tensorMax=%2").arg(tensorMin).arg(tensorMax));

  auto lowerValue = static_cast<int>(std::floor(tensorMin));
  auto upperValue = static_cast<int>(std::ceil(tensorMax));
  {
    QSignalBlocker blocker(mIsoValueSpinBox);
    mIsoValueSpinBox->setRange(lowerValue, upperValue);
  }
  {
    QSignalBlocker blocker(mIsoValueSlider);
    mIsoValueSlider->setMinimum(lowerValue);
    mIsoValueSlider->setMaximum(upperValue);
  }
  setIsoValue(0.5 * (tensorMin + tensorMax));
}

void MCubesWindow::setIsoValue(double isoValue) {
  {
    MCubesRange sliderRange(mIsoValueSlider->minimum(),
                            mIsoValueSlider->maximum());
    QSignalBlocker blocker(mIsoValueSlider);
    MCubesRange isoValueRange(tensorMin, tensorMax);
    double value = sliderRange.getTransformedValue(isoValueRange, isoValue);
    mIsoValueSlider->setValue(
        static_cast<int>(value >= 0.0 ? value + 0.5 : value - 0.5));
  }
  {
    QSignalBlocker blocker(mIsoValueSpinBox);
    mIsoValueSpinBox->setValue(isoValue);
  }

  QTime timer;
  timer.start();
  auto newSurface =
      mMarchingCubes->isoSurface(*mCurrentGrid, *mCurrentTensor, isoValue);

  int elapsedTime = timer.elapsed();
  addLogMessage(QString("Marching cubes executed in %1 ms").arg(elapsedTime));
  addLogMessage(QString("Surface created: %1 points and %2 surfaces")
                    .arg(newSurface.size() * 3)
                    .arg(newSurface.size()));

  while (mRenderer->surfaceCount() > 0) {
    mRenderer->removeSurface();
  }
  mRenderer->addSurface(std::move(newSurface), *mCurrentGrid);
  mRenderer->updateGL();
}
