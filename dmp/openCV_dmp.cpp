#include "openCV_dmp.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>

#include <opencv2/highgui/highgui.hpp>

void dumpDmpLevels(const std::vector<cv::Mat>& dmp, const std::vector<int>& ses, const std::string& sideTag, 
			const boost::filesystem::path& outputPath,
			const boost::filesystem::path& filePath)
{

	// ============================================
	// --------------------------------------------
	// Write each layer as a separate PNG 
	//	: OpenCV : OPEN
	// --------------------------------------------
	for (unsigned int b=0;b<dmp.size();++b)
	{
		boost::filesystem::path outputSS(outputPath);
		outputSS /= filePath.filename().string() + sideTag + "_DMP_" + boost::lexical_cast<std::string>(ses.at(b)) + ".png";
		// Use the line below for older versions of boost
		//outputSS /= filePath.filename() + sideTag + "_DMP_" + boost::lexical_cast<std::string>(ses.at(b)) + ".png";
		
		
		if (boost::filesystem::exists(outputSS))
		{
			if (boost::filesystem::remove(outputSS))
				;

		}

		
		cv::Mat level(dmp.at(b));
		const int mRows = level.rows;
		const int mCols = level.cols;
		cv::Mat outputMat(mRows, mCols, CV_8UC1);
		level.convertTo(outputMat, CV_8UC1);
			
		if (!cv::imwrite(outputSS.string(), level))
			std::cerr << "FAILED TO WRITE: " << outputSS.string() << std::endl;	
	}
}


