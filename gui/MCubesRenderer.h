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

#include "marching-cubes/MarchingCubes.hpp"

#include <QGLWidget>
#include <QPoint>

class MCubesSurface;

/*!
 * \brief The MCubesRenderer class is a widget that renders 3D surfaces.
 */
class MCubesRenderer : public QGLWidget {

  using Surface = std::vector<marchingcubes::Triangle3D>;

public:
  MCubesRenderer(QWidget *parent = nullptr,
                 const QGLWidget *shareWidget = nullptr,
                 Qt::WindowFlags flags = nullptr);
  virtual ~MCubesRenderer();

protected:
  virtual void mousePressEvent(QMouseEvent *evt);
  virtual void mouseMoveEvent(QMouseEvent *evt);

private:
  QPoint mMouseCurrentPos;

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

private:
  std::vector<Surface> mSurfaceList;
  std::array<std::vector<std::pair<double, double>>, marchingcubes::DIM_COUNT>
      minMax;

public:
  inline size_t surfaceCount() const { return mSurfaceList.size(); }
  void addSurface(Surface surface, const marchingcubes::Grid3D &grid);
  void removeSurface();

private:
  void drawSurface(const Surface &surface,
                   const std::pair<double, double> &zMinMax);

private:
  std::pair<double, double> computeMinMax(size_t iAxis) const;
};
