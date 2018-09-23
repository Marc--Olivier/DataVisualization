/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.h
*/ /*!   
   Widget drawing 3D surfaces
   Documentation: http://www.opengl.org/sdk/docs/man/
   Tutorial: http://www.digitalfanatics.org/projects/qt_tutorial/chapter14.html
*/ /*
 =======================================*/
#pragma once

// Inherits
#include <QGLWidget>

// Qt
#include <QPoint>

// MCubes
class MCubesSurface;

class MCubesRenderer : public QGLWidget {
  Q_OBJECT

  /*=============
    Constructor
  *=============*/
public:
  MCubesRenderer(QWidget *parent = nullptr,
                 const QGLWidget *shareWidget = nullptr,
                 Qt::WindowFlags flags = nullptr);
  virtual ~MCubesRenderer();

  /*=============
     QWidget
  *=============*/
protected:
  virtual void mousePressEvent(QMouseEvent *evt);
  virtual void mouseMoveEvent(QMouseEvent *evt);

private:
  QPoint mMouseCurrentPos;

  /*=============
     Open GL
  *=============*/
protected:
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void paintGL();

private:
  float mAzimuth;
  float mElevation;
  bool mIsoXYZ; // Indicates if the ratio
                // Xrange/Yrange, XRange/ZRange and YRange/ZRange
                // are equal to 1.0 (no deformation)

public:
  void setIsoXYZ(bool isIsoXYZ);

  /*=============
     Surfaces
  *=============*/
private:
  std::list<std::unique_ptr<MCubesSurface>> mSurfaceList;

public:
  inline size_t getNbSurfaces() const { return mSurfaceList.size(); }
  void addSurface(std::unique_ptr<MCubesSurface> surface);
  void removeLastSurface();

private:
  void drawSurface(const MCubesSurface &surface);

private:
  std::pair<double, double> computeMinMax(size_t iAxis) const;
};
