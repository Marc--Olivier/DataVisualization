/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesWindow.h
*/ /*!   
   Main window for the marching cubes rendering
*/ /*
 =======================================*/
#pragma once

#include <QMainWindow>
#include <memory>

class QTextEdit;
class QSlider;
class QDoubleSpinBox;

class MCubesRenderer;
class MCubesAlgorithm;
class MCubesGrid;
class MCubesData;

namespace marchingcubes {
class Grid3D;
class MarchingCubes;
class Tensor3D;
} // namespace marchingcubes

/*!
 * \brief The MCubesWindow class is the main window of the application.
 */
class MCubesWindow : public QMainWindow {
  Q_OBJECT

  /*==================
      Constructors
  *==================*/
public:
  MCubesWindow(QWidget *parentWidget = nullptr,
               Qt::WindowFlags flags = nullptr);
  ~MCubesWindow();

  /*==================
        Widgets
  *==================*/
private:
  MCubesRenderer *mRenderer;
  QTextEdit *mLogWidget;
  QWidget *mIsoSurfaceWidget;
  QSlider *mIsoValueSlider;
  QDoubleSpinBox *mIsoValueSpinBox;

protected:
  void addLogMessage(const QString &message);
  void clearLogMessage();

protected slots:
  void slotTestSphere();
  void slotOpenFile();
  void slotSliderValueChanged(int value);
  void slotSpinBoxValueChanged(double value);

private:
  void setTensor(std::unique_ptr<marchingcubes::Grid3D> mCurrentGrid,
                 std::unique_ptr<marchingcubes::Tensor3D> mCurrentTensor);
  void setIsoValue(double isoValue);

private:
  const std::unique_ptr<marchingcubes::MarchingCubes> mMarchingCubes;

private:
  std::unique_ptr<marchingcubes::Grid3D> mCurrentGrid;
  std::unique_ptr<marchingcubes::Tensor3D> mCurrentTensor;
  double tensorMin;
  double tensorMax;
};
