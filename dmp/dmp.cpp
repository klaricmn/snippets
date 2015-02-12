//
// This application works with 
// Boost 1.52.0
// OpenCV 2.4.5
// 
// Other versions may work
// 

#include "openCV_dmp.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <boost/lexical_cast.hpp>

int main(int argc, char** argv)
{

  if(argc < 4)
    {
      std::cerr << "USAGE: " << argv[0] << " <filename> <2 or more DMP SE sizes>" << std::endl;
      return 254;
    }

  // Always force the image to be loaded as a grayscale image.  
  cv::Mat inputImage;
  inputImage = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  std::vector<int> dmpLevels;
  for(int i=2; i<argc; ++i)
    dmpLevels.push_back(boost::lexical_cast<int>(argv[i]));

  // ==================================
  // Open DMP
  // ==================================
  std::vector<cv::Mat> dmp_openV;
  try
    {
      dmp_openV = opencvOpenDmp<unsigned char>(inputImage, dmpLevels);
    }
  catch (const std::exception& e)
    {
      std::cerr << "ERROR: Failed to generate opening" << std::endl;
      return 1;
    }
  
  // ==================================
  // Close DMP
  // ==================================
  std::vector<cv::Mat> dmp_closeV;
  try
    {
      dmp_closeV = opencvCloseDmp<unsigned char>(inputImage, dmpLevels);
    }
  catch (const std::exception& e)
    {
      std::cerr << "ERROR: Failed to generate closing" << std::endl;
      return 2;
    }


  // ==================================
  // code to use DMP would follow 
  // ==================================
  
  dumpDmpLevels( dmp_openV, dmpLevels, "OPEN", ".", "myouptut_");
  dumpDmpLevels( dmp_closeV, dmpLevels, "CLOSE", ".", "myouptut_");
  
  
  return 0;
}
