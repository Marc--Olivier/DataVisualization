/*=======================================*/
/**
   \author M.O. Andrez
   \date   06/07/2010
   \file   main.cpp
*/ /*!   
   Main class of the program
   DICOM files: 
      http://sourceforge.net/apps/mediawiki/gdcm/index.php?title=Sample_DataSet

*/ /*
 =======================================*/

// Qt
#include <QApplication>

// MCubes
#include "MCubesWindow.h"

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(gui_resources);
  QApplication application(argc, argv);
  MCubesWindow *window = new MCubesWindow(nullptr);
  window->show();
  int errorCode = application.exec();

  delete window;
  return errorCode;
}
