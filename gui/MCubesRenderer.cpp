/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Widget drawing 3D surfaces
   Documentation: http://www.opengl.org/sdk/docs/man/
   Tutorial: http://www.digitalfanatics.org/projects/qt_tutorial/chapter14.html
*/ /*
 =======================================*/

// Class definition
#include "MCubesRenderer.h"

#include "marching-cubes/Tensor3D.hpp"

// C / C++
#include <assert.h>
#include <float.h>
#include <math.h>

// Qt
#include <QMouseEvent>

#include "gui/MCubesTools.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Constructor
*/ /*
 =======================================*/
MCubesRenderer::MCubesRenderer(QWidget *parent, const QGLWidget *shareWidget,
                               Qt::WindowFlags flags)
    : QGLWidget(parent, shareWidget, flags) {
  mAzimuth = 20.0;
  mElevation = 30.0;

  mIsoXYZ = false;
  // MCubesSurface * testSurface = MCubesSurface::createSinus( 0.0, 1.0,
  // 0.0, 2.0, 0.0, 3.0, 100, 100 ) ; MCubesSurface * testSurface =
  // MCubesSurface::createSinus( 40.0, 60.0, -100.0, -20.0, 0.0, 3.0, 100, 100 )
  // ; MCubesSurface * testSurface = MCubesSurface::createSinus( 0.0, 1.0,
  // 0.0, 2.0, 0.0, 3.0, 5000, 200 ) ; MCubesSurface * testSurface =
  // MCubesSurface::createParaboloide( 0.0, 1.0, 0.0, 2.0, 0.0, 3.0, 100, 100 )
  // ;

  // addSurface( testSurface ) ;
}

MCubesRenderer::~MCubesRenderer() = default;

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Mouse move event
*/ /*
 =======================================*/
void MCubesRenderer::mousePressEvent(QMouseEvent *evt) {
  QGLWidget::mousePressEvent(evt);
  mMouseCurrentPos = evt->pos();
  evt->accept();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Mouse move event
*/ /*
 =======================================*/
void MCubesRenderer::mouseMoveEvent(QMouseEvent *evt) {
  QGLWidget::mouseMoveEvent(evt);

  QPoint oldPos = mMouseCurrentPos;
  mMouseCurrentPos = evt->pos();
  QPointF deltaAngles = mMouseCurrentPos - oldPos;

  mAzimuth -= deltaAngles.x();
  // Constraint the azimuth between -180.0 and 180.0
  if (mAzimuth > 180.0F)
    mAzimuth -= 360.0F;
  if (mAzimuth < -180.0F)
    mAzimuth += 360.0F;

  mElevation += deltaAngles.y();
  // Constraint the elevation between -90.0 and 90.0
  if (mElevation > 90.0F)
    mElevation = 90.0F;
  if (mElevation < -90.0F)
    mElevation = -90.0F;

  updateGL();
  evt->accept();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   16/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Indicates if the ratio Xrange/Yrange, XRange/ZRange and YRange/ZRange
   are equal to 1.0 (no deformation)
*/ /*
 =======================================*/
void MCubesRenderer::setIsoXYZ(bool isIsoXYZ) { mIsoXYZ = isIsoXYZ; }

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Initialization of open GL properties 
   before the first draw
*/ /*
 =======================================*/
void MCubesRenderer::initializeGL() {
  glShadeModel(GL_SMOOTH); // Enables Smooth Shading
  const int greyColorValue = 0;
  qglClearColor(QColor(greyColorValue, greyColorValue, greyColorValue));

  glClearDepth(10.0);      // Depth Buffer Setup
  glEnable(GL_DEPTH_TEST); // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);  // The Type Of Depth Test To Do

  // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice
  // Perspective Calculations
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Called on resize
*/ /*
 =======================================*/
void MCubesRenderer::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
  glLoadIdentity();            // Reset The Projection Matrix

  glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
  glLoadIdentity();           // Reset The Modelview Matrix
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Method used to paint the contents of the scene onto the widget
*/ /*
 =======================================*/
void MCubesRenderer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  ////  Compute the min-max  ////

  double min[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
  double max[3] = {-DBL_MAX, -DBL_MAX, -DBL_MAX};
  double range[3];
  double maxRange = -DBL_MAX;

  for (unsigned int iAxis = I_XAXIS; iAxis <= I_ZAXIS; iAxis++) {
    std::tie(min[iAxis], max[iAxis]) = computeMinMax(iAxis);
    range[iAxis] = max[iAxis] - min[iAxis];
    if (range[iAxis] >= 0.0 && range[iAxis] > maxRange)
      maxRange = range[iAxis];
  }

  if (mIsoXYZ) {
    for (unsigned int iAxis = I_XAXIS; iAxis <= I_ZAXIS; iAxis++) {
      double deltaRange = maxRange - range[iAxis];
      if (deltaRange > 0.0) {
        range[iAxis] = maxRange;
        min[iAxis] -= deltaRange / 2.0;
        max[iAxis] += deltaRange / 2.0;
        // assert( range[iAxis] == max[iAxis] - min[iAxis] ) ;
      }
    }
  }

  ////  Trasnformation from data to openGL  ////
  // x_data -> x openGL
  // y_data -> z openGL
  // z_data -> y openGL
  {
    GLdouble dataToOpenGL[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    glMultMatrixd(dataToOpenGL);
  }

  //
  // Compute the rotation matrix using the euler angles
  // http://en.wikipedia.org/wiki/Euler_angles
  // In our case, alpha = azimuth, beta = elevation and gamma = 0.0
  //
  // Documentation about matrix multiplication on:
  // http://www.opengl.org/sdk/docs/man/xhtml/glMultMatrix.xml
  //
  {
    GLdouble rotationMatrix[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0};

    MCubesRange degreeAngleRange(-180.0, 180.0);
    MCubesRange radAngleRange(-PI, PI);
    double radAzimuth =
        radAngleRange.getTransformedValue(degreeAngleRange, mAzimuth);
    double radElevation =
        -radAngleRange.getTransformedValue(degreeAngleRange, mElevation);
    double radGamma = 0.0;

    double cosAzimuth = cos(radAzimuth);
    double sinAzimuth = sin(radAzimuth);
    double cosElevation = cos(radElevation);
    double sinElevation = sin(radElevation);
    double cosGamma = cos(radGamma);
    double sinGamma = sin(radGamma);

    rotationMatrix[0] =
        cosAzimuth * cosGamma - cosElevation * sinAzimuth * sinGamma;
    rotationMatrix[1] =
        -cosElevation * cosGamma * sinAzimuth - cosAzimuth * sinGamma;
    rotationMatrix[2] = sinAzimuth * sinElevation;

    rotationMatrix[4] =
        cosGamma * sinAzimuth + cosAzimuth * cosElevation * sinGamma;
    rotationMatrix[5] =
        cosAzimuth * cosElevation * cosGamma - sinAzimuth * sinGamma;
    rotationMatrix[6] = -cosAzimuth * sinElevation;

    rotationMatrix[8] = sinElevation * sinGamma;
    rotationMatrix[9] = cosGamma * sinElevation;
    rotationMatrix[10] = cosElevation;

    glMultMatrixd(rotationMatrix);
  }

  ////  Reduce the size of the Object  ////
  {
    GLdouble homothetieMatrix[16] = {1.0 / range[I_XAXIS],
                                     0.0,
                                     0.0,
                                     0.0,
                                     0.0,
                                     1.0 / range[I_YAXIS],
                                     0.0,
                                     0.0,
                                     0.0,
                                     0.0,
                                     1.0 / range[I_ZAXIS],
                                     0.0,
                                     0.0,
                                     0.0,
                                     0.0,
                                     1.0};
    glMultMatrixd(homothetieMatrix);
  }

  ////  Center the Object  ////
  {
    const double xCenter = (max[I_XAXIS] + min[I_XAXIS]) / 2.0;
    const double yCenter = (max[I_YAXIS] + min[I_YAXIS]) / 2.0;
    const double zCenter = (max[I_ZAXIS] + min[I_ZAXIS]) / 2.0;
    glTranslated(-xCenter, -yCenter, -zCenter);
  }

  ////  Draw axis  ////
  {
    glColor3f(1.0f, 0.0f, 1.0f);

    // X axis
    glBegin(GL_LINES);
    {
      glVertex3d(min[I_XAXIS], min[I_YAXIS], min[I_ZAXIS]);
      glVertex3d(max[I_XAXIS], min[I_YAXIS], min[I_ZAXIS]);
    }
    glEnd();
    renderText(max[I_XAXIS], min[I_YAXIS], min[I_ZAXIS], "X axis");

    // Y axis
    glBegin(GL_LINES);
    {
      glVertex3d(min[I_XAXIS], min[I_YAXIS], min[I_ZAXIS]);
      glVertex3d(min[I_XAXIS], max[I_YAXIS], min[I_ZAXIS]);
    }
    glEnd();
    renderText(min[I_XAXIS], max[I_YAXIS], min[I_ZAXIS], "Y axis");

    // Z axis
    glBegin(GL_LINES);
    {
      glVertex3d(min[I_XAXIS], min[I_YAXIS], min[I_ZAXIS]);
      glVertex3d(min[I_XAXIS], min[I_YAXIS], max[I_ZAXIS]);
    }
    glEnd();
    renderText(min[I_XAXIS], min[I_YAXIS], max[I_ZAXIS], "Z axis");
  }

  ////  Draw the surfaces  ////
  for (std::size_t i = 0; i < mSurfaceList.size(); ++i) {
    drawSurface(mSurfaceList[i], minMax[marchingcubes::Z][i]);
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Add a drawn surface
*/ /*
 =======================================*/
void MCubesRenderer::addSurface(Surface surface,
                                const marchingcubes::Grid3D &grid) {
  using namespace marchingcubes;
  minMax[X].push_back(
      std::make_pair(grid.values[X].front(), grid.values[X].back()));
  minMax[Y].push_back(
      std::make_pair(grid.values[Y].front(), grid.values[Y].back()));
  minMax[Z].push_back(
      std::make_pair(grid.values[Z].front(), grid.values[Z].back()));
  mSurfaceList.push_back(std::move(surface));
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   13/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Remove the last surface
*/ /*
 =======================================*/
void MCubesRenderer::removeSurface() {
  mSurfaceList.pop_back();
  minMax[marchingcubes::X].pop_back();
  minMax[marchingcubes::Y].pop_back();
  minMax[marchingcubes::Z].pop_back();
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Draw a surface
*/ /*
 =======================================*/
void MCubesRenderer::drawSurface(const Surface &surface,
                                 const std::pair<double, double> &zMinMax) {

  using namespace marchingcubes;

  MCubesRange zRange(zMinMax.first, zMinMax.second);
  MCubesRange zColorRange(0.0, 1.0);

  for (size_t iTriangle = 0; iTriangle < surface.size(); ++iTriangle) {
    const auto &triangle = surface[iTriangle];
    glBegin(GL_TRIANGLES);
    for (unsigned int iPoint = 0; iPoint < triangle.size(); iPoint++) {
      const auto &point = triangle[iPoint];
      double zColor = zColorRange.getTransformedValue(zRange, point[Z]);
      glColor3d(zColor, zColor, 1.0);
      glVertex3d(point[X], point[Y], point[Z]);
    }
    glEnd();
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   08/07/2010
   \file   MCubesRenderer.cpp
*/ /*!   
   Compute the min and max values for the given axis
*/ /*
 =======================================*/
std::pair<double, double> MCubesRenderer::computeMinMax(size_t iAxis) const {
  const auto &minMaxAxis = minMax.at(iAxis);
  if (minMaxAxis.empty()) {
    return std::make_pair(0.0, 0.0);
  }
  double minValue, maxValue;
  std::tie(minValue, maxValue) = minMaxAxis.front();

  std::for_each(minMaxAxis.cbegin(), minMaxAxis.cend(),
                [&minValue, &maxValue](const auto &surfaceMinMax) {
                  if (surfaceMinMax.first < minValue)
                    minValue = surfaceMinMax.first;

                  if (surfaceMinMax.second > maxValue)
                    maxValue = surfaceMinMax.second;
                });
  return std::make_pair(minValue, maxValue);
}
