/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Generator of marching cubes configurations
   Use the basic configurations described in http://en.wikipedia.org/wiki/Marching_cubes
*/ /*
 =======================================*/

// C / C++
#include <assert.h>
#include <stdio.h>

// MCubes
#include "MCubesConfigurationArray.h"
#include "MCubesCube.h"
#include "MCubesTools.h"

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Class writing configurations 
*/ /*
 =======================================*/
class MCubesConfigurationGenerator {
public:
  MCubesConfigurationGenerator();
  ~MCubesConfigurationGenerator();

private:
  MCubesConfigurationArray *mBasicConfigurations;

public:
  int writeConfigurations();

private:
  FILE *mFile;
  mutable unsigned int mConfigurationIndex;

private:
  void writeConfigurations(unsigned int iLevel, MCubesCubeSigns &signs) const;
  unsigned int findRefConfiguration(const MCubesCubeSigns &signs,
                                    MCubesCubeIndexes &cubeIndexes) const;
  void writeConfiguration(const MCubesCubeSigns &signs,
                          unsigned int nbTriangles, unsigned int *triangles,
                          unsigned int referenceConfigurationIndex) const;
};

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Constructor
   Basic configurations described in http://en.wikipedia.org/wiki/Marching_cubes
*/ /*
 =======================================*/
MCubesConfigurationGenerator::MCubesConfigurationGenerator() {
  mBasicConfigurations = new MCubesConfigurationArray(15);

  ////  Basic configurations  ////
  unsigned int iConfiguration = 0;

  // 0
  {
    mBasicConfigurations->setSigns(iConfiguration, false, false, false, false,
                                   false, false, false, false);
    iConfiguration++;
  }

  // 1
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, false, false,
                                   false, false, false, false);
    const unsigned int nbTriangles = 1;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 8);
    iConfiguration++;
  }

  // 2
  {
    mBasicConfigurations->setSigns(iConfiguration, true, true, false, false,
                                   false, false, false, false);
    const unsigned int nbTriangles = 2;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 4, 8, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 4, 5, 9);
    iConfiguration++;
  }

  // 3
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, false, false,
                                   false, true, false, false);
    const unsigned int nbTriangles = 2;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 3, 6, 9);
    iConfiguration++;
  }

  // 4
  {
    mBasicConfigurations->setSigns(iConfiguration, false, true, true, true,
                                   false, false, false, false);
    const unsigned int nbTriangles = 3;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 4, 9, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 9, 10, 11);
    iConfiguration++;
  }

  // 5
  {
    mBasicConfigurations->setSigns(iConfiguration, true, true, true, true,
                                   false, false, false, false);
    const unsigned int nbTriangles = 2;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 8, 9, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 9, 10, 11);
    iConfiguration++;
  }

  // 6
  {
    mBasicConfigurations->setSigns(iConfiguration, false, true, true, true,
                                   true, false, false, false);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 3, 7, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 4, 9, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 9, 10, 11);
    iConfiguration++;
  }

  // 7
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, true, false,
                                   false, true, false, true);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 1, 5, 10);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 3, 6, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 7, 11);
    iConfiguration++;
  }

  // 8
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, true, true,
                                   false, false, false, true);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 7, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 2, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 2, 10);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 5, 10);
    iConfiguration++;
  }

  // 9
  // Warning, error in Wikipedia
  {
    mBasicConfigurations->setSigns(iConfiguration, false, true, true, true,
                                   false, false, false, true);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4,
                                      7); // Here is the error
                                          // In Wikipedia, 0,8,7
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 7, 10);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 7, 10);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 9, 10);
    iConfiguration++;
  }

  // 10
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, false, false,
                                   false, false, true, false);
    const unsigned int nbTriangles = 2;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 6, 10);
    iConfiguration++;
  }

  // 11
  {
    mBasicConfigurations->setSigns(iConfiguration, true, true, false, false,
                                   false, false, true, false);
    const unsigned int nbTriangles = 3;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 4, 5, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 5, 8, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 6, 10);
    iConfiguration++;
  }

  // 12
  {
    mBasicConfigurations->setSigns(iConfiguration, false, true, false, false,
                                   true, false, true, false);
    const unsigned int nbTriangles = 3;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 5, 9);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 3, 7, 8);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 6, 10);
    iConfiguration++;
  }

  // 13
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, true, false,
                                   true, false, true, false);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 4, 7);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 3, 7);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 1, 2, 5);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 5, 6);
    iConfiguration++;
  }

  // 14
  {
    mBasicConfigurations->setSigns(iConfiguration, true, false, true, true,
                                   false, false, true, false);
    const unsigned int nbTriangles = 4;
    mBasicConfigurations->setNbTriangles(iConfiguration, nbTriangles);
    unsigned int iTriangle = 0;
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 8, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 6, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 2, 6, 11);
    mBasicConfigurations->setTriangle(iConfiguration, iTriangle++, 0, 5, 6);
    iConfiguration++;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Destructor
*/ /*
 =======================================*/
MCubesConfigurationGenerator::~MCubesConfigurationGenerator() {
  delete mBasicConfigurations;
  return;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Write the configurations in a file
*/ /*
 =======================================*/
int MCubesConfigurationGenerator::writeConfigurations() {
  ////  Open the file  ///
  mFile = fopen("MCubesAllConfigurations_init.cpp", "w");
  if (mFile == NULL)
    return -1;

  ////  Write the header  ///
  fprintf(mFile, "/*=======================================*/\n");
  fprintf(mFile, "/**\n");
  fprintf(mFile, "   \\author M.O. Andrez \n");
  fprintf(mFile, "   \\file   MCubesAllConfigurations_init.cpp \n");
  fprintf(mFile, "*//*!\n");
  fprintf(mFile, "   File generated by mcubes_configuration_generator, \n");
  fprintf(mFile, "   containing the initialization of the 255 marching cubes "
                 "configurations\n");
  fprintf(mFile, "*//*\n");
  fprintf(mFile, "=======================================*/\n");

  fprintf(mFile, "\n//Class definition \n");
  fprintf(mFile, "#include \"MCubesAllConfigurations.h\" \n\n");
  fprintf(mFile,
          "void MCubesAllConfigurations::initAllConfigurations( ) \n{\n");

  ////  Write the configurations  ///

  mConfigurationIndex = 0;

  /*
  const unsigned int nbBasicConfigurations =
  mBasicConfigurations->getNbConfigurations( ) ; for ( unsigned int
  iConfiguration = 0 ; iConfiguration < nbBasicConfigurations ; iConfiguration
  ++ )
  {
     writeConfiguration( mBasicConfigurations->getSigns( iConfiguration ),
                         mBasicConfigurations->getNbTriangles( iConfiguration ),
                         mBasicConfigurations->getTriangles( iConfiguration ),
                         iConfiguration ) ;
     mConfigurationIndex ++ ;
  }
  */

  MCubesCubeSigns signs;
  writeConfigurations(0, signs);

  ////  Close the file  ///
  fprintf(mFile, "\n}\n");
  fclose(mFile);
  return 0;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Recursive method used to write all the configurations into the file
*/ /*
 =======================================*/
void MCubesConfigurationGenerator::writeConfigurations(
    unsigned int iLevel, MCubesCubeSigns &signs) const {
  if (iLevel == 8) // Write the current configuration
  {
    // Find the equivalent basic configuration
    // by applying symmetries on reference configurations

    MCubesCubeIndexes cubeIndexes;
    unsigned int foundConfiguration =
        MCubesConfigurationGenerator::findRefConfiguration(signs, cubeIndexes);
// assert( foundConfiguration !=
// MCubesConfigurationArray::UNDEFINED_CONFIGURATION ) ;
#ifdef _DEBUG
    if (foundConfiguration ==
        MCubesConfigurationArray::UNDEFINED_CONFIGURATION) {
      foundConfiguration = MCubesConfigurationGenerator::findRefConfiguration(
          signs, cubeIndexes);
    }
#endif

    const unsigned int nbTriangles =
        mBasicConfigurations->getNbTriangles(foundConfiguration);
    const unsigned int *refTriangles =
        mBasicConfigurations->getTriangles(foundConfiguration);
    unsigned int *triangles = new unsigned int[3 * nbTriangles];
    for (unsigned int iEdge = 0; iEdge < 3 * nbTriangles; iEdge++)
      triangles[iEdge] = cubeIndexes.getEdgeNewIndex(refTriangles[iEdge]);

    writeConfiguration(signs, nbTriangles, triangles, foundConfiguration);
    delete[] triangles;
  }

  else // Construct the configurations for the current index
  {
    assert(signs[iLevel] == false);
    writeConfigurations(iLevel + 1, signs);
    signs[iLevel] = true;
    writeConfigurations(iLevel + 1, signs);
    signs[iLevel] = false;
  }
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Find a reference configuration
*/ /*
 =======================================*/
unsigned int MCubesConfigurationGenerator::findRefConfiguration(
    const MCubesCubeSigns &signs, MCubesCubeIndexes &cubeIndexes) const {
  MCubesCubeSigns auxSigns = signs;
  auxSigns.setMinusMajority();
  const unsigned int nbPlus = auxSigns.getNbPlusSigns();

  const unsigned int nbBasicConfigurations =
      mBasicConfigurations->getNbConfigurations();
  size_t foundConfiguration = MCubesConfigurationArray::UNDEFINED_CONFIGURATION;

  for (unsigned int iConfiguration = 0; iConfiguration < nbBasicConfigurations;
       iConfiguration++) {
    MCubesCubeSigns refConfSigns =
        mBasicConfigurations->getSigns(iConfiguration);
    unsigned int nbRefPlus = refConfSigns.getNbPlusSigns();
    if (nbPlus == nbRefPlus) {
      cubeIndexes.initDefault();

      //// Test all configurations
      //
      //           D2                  D2
      //      A7________A6         a7________a6
      //   D7 /|       /|D6     d7 /|       /|d6
      //     / |  D3  / |         / |  d3  / |
      //   A4________A5 |D10    a4________a5 |d10
      //    |  |     |  |        |  |     |  |
      //    |  |D11  |D9|        |  |d11  |d9|
      // D8 |  |  D1 |  |     d8 |  |  d1 |  |
      //    | A3_____|__A2       | a3_____|__a2
      //    | /      | /         | /      | /
      //    |/ D4    |/ D5       |/ d4    |/ d5
      //   A0________A1         a0________a1
      //        D0                   d0
      //
      // A configuration a0,a1,...,a7 is a permutation P of A0,A1,...,A7
      // checking the condition:
      //    for each i in 0..7, there is one and only one j in 0..7
      //          so that "a_i=P(A_i)=A_j"
      //    for each i in 0..11, there is one and only one j in 0..11
      //          so that "d_i=Pd(D_i)=D_j"
      //
      // As a consequence, if A_i1, A_i2, A_i3, A_i4 are on the same cube face
      // then a_j1=P(A_i1), a_j2=P(A_i2), a_j3=P(A_i3), a_j4=P(A_i4) are on the
      // same cube face The points a0, a1, a3, a4 are defining a configuration:
      //   - a2 is the unique point on the same face as a0, a1, a3
      //   - a5 is the unique point on the same face as a0, a1, a4
      //   - a7 is the unique point on the same face as a0, a3, a4
      //   - a6 is the unique point on the same face as a4, a5, a7
      //
      // Testing all configurations mean testing all permutations
      // giving different a0, a1, a3, a4
      // (remembering that d0, d4, d5, d8 are edges from D0,...,D11 )
      // Only three edges are starting from a given a0 = A_i0:
      // D_j1, D_j2 and D_j3. The other extremities of these edges
      // are a1, a3 and a4, but we do not know if a1 is at the end
      // of edge D_j1, D_j2 or D_j3.
      // => there are 6=3*2*1 permutations without repeting a number
      // possible for a given a0
      // a0 can be one of the eight vertex of the cube
      // => 6*48 possibilities
      //
      // Algorithm:
      // ----------
      // For a0 in A0..A7
      //    For a1, a3, a4 extremities of edges starting with a0
      //       Test configuration
      //

      // Make change a0
      for (unsigned int iXYSymmetry = 0; iXYSymmetry < 2; iXYSymmetry++) {
        for (unsigned int iXZSymmetry = 0; iXZSymmetry < 2; iXZSymmetry++) {
          for (unsigned int iYZSymmetry = 0; iYZSymmetry < 2; iYZSymmetry++) {
            // Make change a1, a3, a4

            for (unsigned int iPa4 = 0; iPa4 < 3; iPa4++) {
              for (unsigned int iPa2 = 0; iPa2 < 2; iPa2++) {
                if (refConfSigns == auxSigns) {
                  foundConfiguration = iConfiguration;
                  cubeIndexes.inversePermutation();
                  return foundConfiguration;
                }
                MCubesCube::apply0246Symmetry(refConfSigns);
                MCubesCube::apply0246Symmetry(cubeIndexes);
              }

              if (iPa4 == 0) {
                // interchange a3 and a4
                MCubesCube::apply0167Symmetry(refConfSigns);
                MCubesCube::apply0167Symmetry(cubeIndexes);
              } else if (iPa4 == 1) {
                // Restore a3 and a4
                MCubesCube::apply0167Symmetry(refConfSigns);
                MCubesCube::apply0167Symmetry(cubeIndexes);
                // interchange a1 and a4
                MCubesCube::apply0356Symmetry(refConfSigns);
                MCubesCube::apply0356Symmetry(cubeIndexes);
              } else {
                // Restore a1 and a4
                MCubesCube::apply0356Symmetry(refConfSigns);
                MCubesCube::apply0356Symmetry(cubeIndexes);
              }
            }

            MCubesCube::applyYZSymmetry(refConfSigns);
            MCubesCube::applyYZSymmetry(cubeIndexes);
          }

          MCubesCube::applyXZSymmetry(refConfSigns);
          MCubesCube::applyXZSymmetry(cubeIndexes);
        }

        MCubesCube::applyXYSymmetry(refConfSigns);
        MCubesCube::applyXYSymmetry(cubeIndexes);
      }
    }
  }

  return foundConfiguration;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Write one configuration 
*/ /*
 =======================================*/
static inline const char bool2sign(bool sign) { return sign ? '+' : '-'; };
static inline const char *bool2string(bool sign) {
  return sign ? "true" : "false";
};

void MCubesConfigurationGenerator::writeConfiguration(
    const MCubesCubeSigns &signs, unsigned int nbTriangles,
    unsigned int *triangles, unsigned int referenceConfigurationIndex) const {
  const MCubesCubeSigns &refSigns =
      mBasicConfigurations->getSigns(referenceConfigurationIndex);

  ////  Write comments to have the reference configuration  ////

  fprintf(mFile, "// \n");
  fprintf(mFile, "// Configuration %d \n", mConfigurationIndex);
  fprintf(mFile, "// \n");
  fprintf(mFile, "//       initial conf       reference basic conf #%d \n//\n",
          referenceConfigurationIndex);
  fprintf(mFile, "//           D2                 D2                   \n");
  fprintf(mFile, "//       %c________%c           %c________%c              \n",
          bool2sign(signs[7]), bool2sign(signs[6]), bool2sign(refSigns[7]),
          bool2sign(refSigns[6]));
  fprintf(mFile, "//   D7 /|       /|D6     D7 /|       /|D6     \n");
  fprintf(mFile, "//     / |  D3  / |         / |  D3  / |       \n");
  fprintf(mFile, "//    %c________%c  |D10     %c________%c  |D10    \n",
          bool2sign(signs[4]), bool2sign(signs[5]), bool2sign(refSigns[4]),
          bool2sign(refSigns[5]));
  fprintf(mFile, "//    |  |     |  |        |  |     |  |       \n");
  fprintf(mFile, "//    |  |D11  |D9|        |  |D11  |D9|       \n");
  fprintf(mFile, "// D8 |  |  D1 |  |     D8 |  |  D1 |  |       \n");
  fprintf(mFile, "//    |  %c_____|__%c        |  %c_____|__%c       \n",
          bool2sign(signs[3]), bool2sign(signs[2]), bool2sign(refSigns[3]),
          bool2sign(refSigns[2]));
  fprintf(mFile, "//    | /      | /         | /      | /        \n");
  fprintf(mFile, "//    |/ D4    |/ D5       |/ D4    |/ D5      \n");
  fprintf(mFile, "//    %c________%c           %c________%c      \n",
          bool2sign(signs[0]), bool2sign(signs[1]), bool2sign(refSigns[0]),
          bool2sign(refSigns[1]));
  fprintf(mFile, "//        D0                    D0             \n//\n\n");

  ////  Write the configuration  ////

  // Write the signs
  fprintf(mFile, "   setSigns( %d, %s, %s, %s, %s, \n", mConfigurationIndex,
          bool2string(signs[0]), bool2string(signs[1]), bool2string(signs[2]),
          bool2string(signs[3]));
  fprintf(mFile, "                 %s, %s, %s, %s);\n", bool2string(signs[4]),
          bool2string(signs[5]), bool2string(signs[6]), bool2string(signs[7]));

  fprintf(mFile, "   setNbTriangles( %d, %d ); \n", mConfigurationIndex,
          nbTriangles);

  for (unsigned int iTriangle = 0; iTriangle < nbTriangles; iTriangle++) {
    unsigned int *triangle = triangles + 3 * iTriangle;
    fprintf(mFile, "   setTriangle( %d, %d, %d, %d, %d ); \n",
            mConfigurationIndex, iTriangle, triangle[0], triangle[1],
            triangle[2]);
  }

  fprintf(mFile, "\n");
  mConfigurationIndex++;
}

/*=======================================*/
/**
   \author M.O. Andrez
   \date   10/07/2010
   \file   MCubesConfigurationGenerator.cpp
*/ /*!   
   Main method of the program
*/ /*
 =======================================*/
int main(int /*argc*/, char * /*argv*/ [])
// #include <QApplication>
// int main(int argc, char * argv[])
{
  // QApplication app( argc, argv ) ;
  MCubesConfigurationGenerator configurationGenerator;
  int errorCode = configurationGenerator.writeConfigurations();
  // app.exec( ) ;
  return errorCode;
}
