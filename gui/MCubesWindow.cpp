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
#include "MCubesWindow.h"

// C / C++
#include <assert.h>

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
#include "DicomReader.h"
#include "MCubesAlgorithm.h"
#include "MCubesGrid.h"
#include "MCubesRenderer.h"
#include "MCubesSurface.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   09/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Fonctor to construct values values f=x²+y²+z²
   on a grid (x,y,z)  
*/ /*
 =======================================*/
class MCubesFonctorSphere {
public:
  MCubesFonctorSphere() {
    mValues = new MCubesData();
    mPointGlobalIndex = 0;
  }

  ~MCubesFonctorSphere() { delete mValues; }

private:
  MCubesData *mValues;
  unsigned int mPointGlobalIndex;

public:
  MCubesData *detachData() {
    MCubesData *values = mValues;
    mValues = new MCubesData();
    return values;
  }

public:
  void setGrid(const MCubesGrid &grid) {
    mPointGlobalIndex = 0;
    mValues->resize(grid.getGridSize());
  }

  inline void execute(double xValue, double yValue, double zValue) {
    (*mValues)[mPointGlobalIndex++] =
        xValue * xValue + yValue * yValue + zValue * zValue;
  }
};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesWindow::MCubesWindow(QWidget *parentWidget, Qt::WindowFlags flags)
    : QMainWindow(parentWidget, flags) {
  setWindowTitle(QObject::tr("DICOM 3D renderer"));
  mMarchingCubes = new MCubesAlgorithm();

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

/*=======================================*/
/**
   \author M.O. Andrez
   \date   15/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Destructor
*/ /*
 =======================================*/
MCubesWindow::~MCubesWindow() {
  setMarchingCubesData();
  delete mMarchingCubes;
  mMarchingCubes = nullptr;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   13/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Add log messages
*/ /*
 =======================================*/
void MCubesWindow::addLogMessage(const QString &message) {
  mLogWidget->append(message);
  mLogWidget->repaint();
  // QCoreApplication::processEvents( ) ;
}

void MCubesWindow::clearLogMessage() {
  mLogWidget->clear();
  mLogWidget->repaint();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   15/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Set the wanted iso value 
*/ /*
 =======================================*/
void MCubesWindow::setIsoValue(double isoValue) {
  // Update the slider
  {
    mIsoValueSlider->blockSignals(true);
    MCubesRange sliderRange(mIsoValueSlider->minimum(),
                            mIsoValueSlider->maximum());
    MCubesRange isoValueRange(mCurrentValues->getMin(),
                              mCurrentValues->getMax());
    double value = sliderRange.getTransformedValue(isoValueRange, isoValue);
    mIsoValueSlider->setValue(
        static_cast<int>(value >= 0.0 ? value + 0.5 : value - 0.5));
    mIsoValueSlider->blockSignals(false);
  }

  // Update the spin box
  {
    mIsoValueSpinBox->blockSignals(true);
    mIsoValueSpinBox->setValue(isoValue);
    mIsoValueSpinBox->blockSignals(false);
  }

  // Update the iso-surface
  {
    while (mRenderer->getNbSurfaces() > 0)
      mRenderer->removeLastSurface();

    // Compute the iso surface f=1
    addLogMessage(QString("\nCompute the v=%1 iso-surface").arg(isoValue));

    QTime timer;
    timer.start();
    auto newSurface = mMarchingCubes->createIsoSurface(isoValue);

    int elapsedTime = timer.elapsed();
    addLogMessage(QString("Marching cubes executed in %1 ms").arg(elapsedTime));
    addLogMessage(QString("Surface created: %1 points and %2 surfaces")
                      .arg(newSurface->getNbData())
                      .arg(newSurface->getFaceList().size()));

    mRenderer->addSurface(std::move(newSurface));
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   15/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Slot called when the value of the slider has changed
*/ /*
 =======================================*/
void MCubesWindow::slotSliderValueChanged(int value) {
  assert(mCurrentValues != NULL);
  MCubesRange sliderRange(mIsoValueSlider->minimum(),
                          mIsoValueSlider->maximum());
  MCubesRange isoValueRange(mCurrentValues->getMin(), mCurrentValues->getMax());
  double isoValue = isoValueRange.getTransformedValue(sliderRange, value);

  setIsoValue(isoValue);
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   15/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Slot called when the value of the spin box has changed
*/ /*
 =======================================*/
void MCubesWindow::slotSpinBoxValueChanged(double value) { setIsoValue(value); }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   13/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Slot called to execute the marching cubes on a "sphere" grid
*/ /*
 =======================================*/
void MCubesWindow::slotTestSphere() {
  ////  Test  ////

  // Create grid with f=x²+y²+z² values
  unsigned int nbXPoints = 101;
  unsigned int nbYPoints = 101;
  unsigned int nbZPoints = 101;
  /*
  unsigned int nbXPoints = 1001 ;
  unsigned int nbYPoints = 501 ;
  unsigned int nbZPoints = 101 ;
  */
  // addLogMessage( QString( "\n=================\n" ) ) ;
  clearLogMessage();
  addLogMessage(QString("Create grid (%1,%2,%3)")
                    .arg(nbXPoints)
                    .arg(nbYPoints)
                    .arg(nbZPoints));
  mCurrentGrid = MCubesGrid::createGrid(-1.0, 1.0, -2.0, 2.0, -3.0, 3.0,
                                        nbXPoints, nbYPoints, nbZPoints);

  QTime timer;
  timer.start();

  /*
  auto elementsCount = mCurrentGrid->getGridSize();
  std::vector<double> values(elementsCount);
  for (size_t zIdx = 0; zIdx < mCurrentGrid->getData(); ++zIdx) {
  }
  */

  MCubesFonctorSphere sphereFonctor;
  mCurrentGrid->scanAllPoints(sphereFonctor);
  mCurrentValues.reset(sphereFonctor.detachData());
  mCurrentValues->computeProperties();
  addLogMessage(
      QString("Fill %1 points using the f=x²+y²+z² sphere function in %2 ms")
          .arg(nbXPoints * nbYPoints * nbZPoints)
          .arg(timer.elapsed()));

  setMarchingCubesData();
  setIsoValue(0.5 * (mCurrentValues->getMin() + mCurrentValues->getMax()));
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   14/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Slot called to open a dicom file 
   and execute the marching cube on it 
   
   Data encoding: http://medical.nema.org/dicom/2007/07_05pu.pdf
*/ /*
 =======================================*/
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

  /*
  QString fileName =
     QFileDialog::getOpenFileName( this, QObject::tr( "Open DICOM file" ),
                                   QString(), "DICOM (*.dcm);; All (*)" ) ;
  */

  QTime timer;
  timer.start();

  // Create the grid
  fileList.sort();
  std::list<std::string> fileNameList;
  foreach (const QString &fileName, fileList)
    // fileNameList.push_back( fileName.toLatin1().toData() ) ;
    fileNameList.push_back(fileName.toStdString());

  auto readDicom = DicomReader::readFiles(fileNameList);
  const auto &errorFileNameList = readDicom.getErrorFileNameList();
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

  mCurrentValues = readDicom.releaseValues();
  mCurrentGrid = readDicom.releaseGrid();
  setMarchingCubesData();
  // => The memory of the grid is now owned by the MCubesWindow

  // Set a "good" iso-value
  setIsoValue(0.5 * (mCurrentValues->getMin() + mCurrentValues->getMax()));
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   14/07/2010
   \file   MCubesWindow.cpp
*/ /*!   
   Set the marching cubes data
   The memory is then owned by the MCubesWindow
*/ /*
 =======================================*/
void MCubesWindow::setMarchingCubesData() {
  mMarchingCubes->setGridPoints(nullptr);
  mMarchingCubes->setValues(nullptr);

  mMarchingCubes->setGridPoints(mCurrentGrid.get());
  mMarchingCubes->setValues(mCurrentValues.get());

  if (mCurrentValues != nullptr && mCurrentGrid != nullptr) {
    mIsoSurfaceWidget->setEnabled(true);
    mIsoValueSpinBox->setMinimum(mCurrentValues->getMin());
    mIsoValueSpinBox->setMaximum(mCurrentValues->getMax());
    // mIsoValueSlider->

    addLogMessage(QObject::tr("Grid size = %1 x %2 x %3")
                      .arg(mCurrentGrid->getData(I_XAXIS).size())
                      .arg(mCurrentGrid->getData(I_YAXIS).size())
                      .arg(mCurrentGrid->getData(I_ZAXIS).size()));
  } else {
    mIsoSurfaceWidget->setEnabled(false);
  }
}
