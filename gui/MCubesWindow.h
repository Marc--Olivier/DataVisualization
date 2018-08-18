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

// Qt
class QTextEdit;
class QSlider;
class QDoubleSpinBox;

// MCubes
class MCubesRenderer;
class MCubesAlgorithm;
class MCubesGrid;
class MCubesData;

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
  void setIsoValue(double isoValue);

  /*==================
      Marching cubes
  *==================*/
private:
  MCubesAlgorithm *mMarchingCubes; // Composite variable
private:
  std::unique_ptr<MCubesGrid> mCurrentGrid;
  std::unique_ptr<MCubesData> mCurrentValues;

private:
  void setMarchingCubesData();
};
