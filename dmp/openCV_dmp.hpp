#ifndef _OPENCV_DMP_HPP_
#define _OPENCV_DMP_HPP_

#include <vector>
#include <opencv2/core/core.hpp>
#include <boost/filesystem.hpp>

#include <limits>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <stdexcept>

#include <opencv2/imgproc/imgproc.hpp>

//for DEBUG
//#include <opencv2/highgui/highgui.hpp>

///
/// Generate a opening DMP profile
/// \param inputImage The image that will be filtered for light objects
/// \param ses The set of strictly increasing geodesic structuring elements
///
template <typename OperatingPixelType>
std::vector<cv::Mat> opencvOpenDmp(const cv::Mat& inputImage, const std::vector<int>& ses);

///
/// Dilation by Reconstruction using the Downhill Filter algorithm (Robinson,Whelan)
///
/// \param mask The image/surface that bounds the reconstruction of the marker
/// \param mark The image/surface that initializes the reconstruction
/// \returns The dilation reconstruction
template <typename OperatingPixelType>
cv::Mat dilationByReconstructionDownhill(const cv::Mat& mask, const cv::Mat& mark);


///
/// Generate a closing DMP profile
/// \param inputImage The image that will be filtered for dark objects
/// \param ses The set of strictly increasing geodesic structuring elements
///
template <typename OperatingPixelType>
std::vector<cv::Mat> opencvCloseDmp(const cv::Mat& inputImage, const std::vector<int>& ses);

///
/// Erosion by Reconstruction using an Uphill Filter algorithm (insp. by Robinson,Whelan)
///
/// \param mask The image/surface that bounds the reconstruction of the marker
/// \param mark The image/surface that initializes the reconstruction
/// \returns The erosion reconstruction
template <typename OperatingPixelType>
cv::Mat erosionByReconstructionUphill(const cv::Mat& mask, const cv::Mat& mark);

/// Dump the pyramid levels
/// \param dmp the pyramid cells to dump
/// \param sideTag the Open or Close identifier for the levels
/// \param outputPath the output directory
/// \param filePath the filename being processed
void dumpDmpLevels(const std::vector<cv::Mat>& dmp, const std::vector<int>& ses, const std::string& sideTag, 
			const boost::filesystem::path& outputPath,
			const boost::filesystem::path& filePath);


// ============================
// ============================
// == BEGIN Template Functions
// ============================
// ============================

template <typename OperatingPixelType>
std::vector<cv::Mat> opencvOpenDmp(const cv::Mat& inputImage, const std::vector<int>& ses)
{
	//cgi::log::MetaLogStream& log(cgi::log::MetaLogStream::instance());
	// ++ Open, then dilate by reconstruction

	//if (false == std::numeric_limits<T>::is_integer)
	//	CGI_THROW(std::logic_error,"Invalid template type");

	// Build Structuring Elements
	std::vector<cv::Mat> structuringElements;
	for (std::vector<int>::const_iterator level = ses.begin();level!=ses.end();++level)
	{
		const int lev = *level;
		// Circular structuring element
		const int edgeSize = lev+1+lev; // turn radius into edge size
		structuringElements.push_back(
					cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(edgeSize,edgeSize))
					);
	}

	// Build Morphological Operation Stack (open)
	std::vector<cv::Mat> mos;
	for (unsigned int level = 0;level<structuringElements.size();++level)
	{
		cv::Mat filteredImage(inputImage.size().height, inputImage.size().width, cv::DataType<OperatingPixelType>::type);
		cv::morphologyEx(inputImage, filteredImage, cv::MORPH_OPEN, structuringElements.at(level));
		mos.push_back(filteredImage);
	}

	const int n_lvls = mos.size();
//	log << cgi::log::Priority::INFO << "opencvOpenDmp" << "MOS Produced, size:" << n_lvls << cgi::log::flush;

	// Set the Tile band count to the SEs array size
	std::vector<cv::Mat> profile;
	profile.reserve(n_lvls);

	// ////////////////////////////////////////////////////////////////////////
	// USE: dilationByReconstructionDownhill(const cv::Mat& mask,const cv::Mat& mark)


	cv::Mat tempRec;			// Temporary Open Record
	cv::Mat lastRec;			// previous level Open Record

	// Open = (erode,dilate)
	//typename MorphologyOperator<DataType>::type morphology(_ses[0]);
	//typename ReconstructionOperator<DataType>::type reconstructor;

	//dilationByReconstructionDownhill( inputImage, mos.at(0) ).copyTo(tempRec);
	tempRec = dilationByReconstructionDownhill<OperatingPixelType>( inputImage, mos.at(0) ).clone();

	// get the absolute difference of the two images
	cv::Mat diffImage(inputImage.size().height, inputImage.size().width, cv::DataType<OperatingPixelType>::type);
	absdiff(inputImage, tempRec, diffImage);

	profile.push_back(diffImage.clone());
	tempRec.copyTo(lastRec);

	// Subsequent Levels
	for (int i = 1;i < n_lvls;++i)
	{
		//  reconstruction(mask     ,  mark)
		//dilationByReconstructionDownhill( inputImage, mos.at(i) ).copyTo(tempRec);
		tempRec = dilationByReconstructionDownhill<OperatingPixelType>( inputImage, mos.at(i) ).clone();
		
		// BEGIN DEBUG 
		/*
		std::stringstream ss;
		ss << "reconstruction_from_opening_" << i << ".png";
		if(!cv::imwrite(ss.str(), tempRec))
		  {
		    throw std::runtime_error("Failed to write");
		  }
		*/
		// END DEBUG
		
		// get the absolute difference of the two images
		absdiff(lastRec, tempRec, diffImage);
		profile.push_back(diffImage.clone());

		tempRec.copyTo(lastRec);

	}


	return profile;
}

template <typename OperatingPixelType>
cv::Mat dilationByReconstructionDownhill(const cv::Mat& mask, const cv::Mat& mark)
{
	//typedef unsigned short OperatingPixelType;
	//cgi::log::MetaLogStream& log(cgi::log::MetaLogStream::instance());

	if (false == std::numeric_limits<OperatingPixelType>::is_integer)
		//CGI_THROW(std::logic_error,"Invalid template / operating-pixel type");
		throw new std::logic_error("Invalid template / operating-pixel type");

	const cv::Size size = mask.size();

	//if (size.width != mark.size().width || size.height != mark.size().height)
	if (size != mark.size())
	{
		//log << cgi::log::Priority::ERROR << "dilationByReconstructionUphill"
		//<< "Size mismatch: ("
		//<< size.width << "," << size.height << ") != ("
		//<< mark.size().width << "," << mark.size().height << ")" << cgi::log::flush;
	}

	// --------------------------------
	// Place each p : I(p) > 0 into List[I(p)]
	// --------------------------------
	// Copy the marker into the reconstruction
	//	we can then grow it in place
	// value i has a list of pixels at position (y,x)
	cv::Mat reconstruction(mark.size().height, mark.size().width, cv::DataType<OperatingPixelType>::type);
	mark.copyTo(reconstruction);

	// for template, use typename
	//	typedef typename std::list<std::pair<int,int> > LocationListType;
	//	typedef typename std::map< OperatingPixelType, LocationListType > MapOfLocationListType;
	typedef typename std::list<std::pair<int,int> > LocationListType;
	typedef typename std::map< OperatingPixelType, LocationListType > MapOfLocationListType;

	// This is essentially an indexed image version of the input `mark' raster
	MapOfLocationListType valueLocationListMap;

	// Build an in-memory indexed image
	// but while we are at it, verify that it is properly bounded by `mask' raster
	for (int i = 0; i < size.height; ++i)
		for (int j = 0; j < size.width; ++j)
		{
			const OperatingPixelType pixelValue =  reconstruction.at<OperatingPixelType>(i, j);
			if (pixelValue > mask.at<OperatingPixelType>(i, j))
			{
				//std::cout << "Storing ("<< i << "," << j <<") into valueLocationListMap["<< static_cast<int>(pixelValue) << "]" << std::endl;
				std::stringstream msg;
				msg << "(pixelValue > mask[0](i, j)), for raster location = ("<<i<<","<<j<<")";
				//CGI_THROW (std::logic_error, msg.str()); // the marker must always be LTE the mask, by definition
				throw new std::logic_error( msg.str());
			}

			valueLocationListMap[pixelValue].push_back( std::make_pair<int,int>(i,j) );
		}

	// No valid pixels or values above floor?  Return the input as output
	if (valueLocationListMap.size() == 0)
		return reconstruction;

	// --------------------------------
	// The farthest downhill we will go
	//	is to the current min(mark[0])
	//	therefore, we do not need to
	//	process the current min, we
	//	already copied them to output
	const OperatingPixelType minValue =  valueLocationListMap.begin()->first;
	valueLocationListMap[minValue].clear();

	std::set<std::pair<int,int> > finalizedPixels;

	// --------------------------------
	// Do a backward iteration (downhill)
	// through the
	//	valueLocationListMap[values]
	// --------------------------------
	// use typename for template version
	//for (typename MapOfLocationListType::reverse_iterator valueList = valueLocationListMap.rbegin();
	//			valueList != valueLocationListMap.rend(); ++valueList)
	for (typename MapOfLocationListType::reverse_iterator valueList = valueLocationListMap.rbegin();
					valueList != valueLocationListMap.rend(); ++valueList)
	{
		// The gray value
		const OperatingPixelType currentValue = valueList->first;
		// The list of (y,x) tuples
		LocationListType locations = valueList->second;

		// for each location indexed in the value list
		while (!locations.empty())
		{

			// Debug thing
//			if (locations.size() > (size.width * size.height))
//			{
//				CGI_THROW(std::logic_error, "GrayscaleDilationByReconstructionDownhill has gone out of control, this should never happen!");
//			}

			// pull/pop the first position from the list, mark it as finalized
			std::pair<int,int> frontPosition = locations.front();
			finalizedPixels.insert(frontPosition);
			locations.pop_front();

			const int y = frontPosition.first;
			const int x = frontPosition.second;

			const int pre_x = x - 1;
			const int post_x = x + 1;
			const int pre_y = y - 1;
			const int post_y = y + 1;

			// For each neighbor
			// - a -
			// b p d
			// - c -
			// a = (pre_y,x), b = (y,pre_x), c = (post_y,x), d = (y,post_x)

			// Neighbor Pixel 'a'
			const std::pair<int,int> a = std::make_pair<int,int>(pre_y,x);
			const OperatingPixelType mask_a = mask.at<OperatingPixelType>(pre_y,x);
			// if neighbor index is within bounds and not finalized
			if ((finalizedPixels.find(a) == finalizedPixels.end()) && (pre_y >= 0) && (mask_a > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(pre_y,x);
				OperatingPixelType constraintValue = std::min<OperatingPixelType>(currentValue,mask_a);

//				std::cout << "Neighbor(a) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue < constraintValue)
				{
					reconstruction.at<OperatingPixelType>(pre_y,x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< pre_y << "," << x <<") to current list" << std::endl;

						locations.push_back( a );
					}
					else
					{
//						std::cout << "Moving ("<< pre_y << "," << x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( a );
						valueLocationListMap[constraintValue].push_back( a );
					}
				}
			}

			// Neighbor Pixel 'b'
			const std::pair<int,int> b = std::make_pair<int,int>(y,pre_x);
			const OperatingPixelType mask_b = mask.at<OperatingPixelType>(y,pre_x);
			if ((finalizedPixels.find(b) == finalizedPixels.end()) && (pre_x >= 0) && (mask_b > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(y,pre_x);
				OperatingPixelType constraintValue = std::min<OperatingPixelType>(currentValue,mask_b);

//				std::cout << "Neighbor(b) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue < constraintValue)
				{
					reconstruction.at<OperatingPixelType>(y,pre_x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< y << "," << pre_x <<") to current list" << std::endl;

						locations.push_back( b );
					}
					else
					{
//						std::cout << "Moving ("<< y << "," << pre_x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( b );
						valueLocationListMap[constraintValue].push_back( b );
					}
				}
			}

			// Neighbor Pixel 'c'
			const std::pair<int,int> c = std::make_pair<int,int>(post_y,x);
			const OperatingPixelType mask_c = mask.at<OperatingPixelType>(post_y,x);
			if ((finalizedPixels.find(c) == finalizedPixels.end()) && (post_y < size.height) && (mask_c > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(post_y,x);
				OperatingPixelType constraintValue = std::min<OperatingPixelType>(currentValue,mask_c);

//				std::cout << "Neighbor(c) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue < constraintValue)
				{
					reconstruction.at<OperatingPixelType>(post_y,x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< post_y << "," << x <<") to current list" << std::endl;

						locations.push_back( c );
					}
					else
					{
//						std::cout << "Moving ("<< post_y << "," << x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( c );
						valueLocationListMap[constraintValue].push_back( c );
					}
				}
			}

			// Neighbor Pixel 'd'
			const std::pair<int,int> d = std::make_pair<int,int>(y,post_x);
			const OperatingPixelType mask_d = mask.at<OperatingPixelType>(y,post_x);
			if ((finalizedPixels.find(d) == finalizedPixels.end()) && (post_x < size.width) && (mask_d > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(y,post_x);
				OperatingPixelType constraintValue = std::min<OperatingPixelType>(currentValue,mask_d);

//				std::cout << "Neighbor(d) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue < constraintValue)
				{
					reconstruction.at<OperatingPixelType>(y,post_x) = constraintValue;

					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< y << "," << post_x <<") to current list" << std::endl;

						locations.push_back( d );
					}
					else
					{
//						std::cout << "Moving ("<< y << "," << post_x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( d );
						valueLocationListMap[constraintValue].push_back( d );
					}
				}
			}



		} // end, a value's position list is emptied.

	} // end for each value in the marker



	return reconstruction;
};


template <typename OperatingPixelType>
std::vector<cv::Mat> opencvCloseDmp(const cv::Mat& inputImage, const std::vector<int>& ses)
{
	//cgi::log::MetaLogStream& log(cgi::log::MetaLogStream::instance());
	// ++ Close, then erode by reconstruction

	//if (false == std::numeric_limits<T>::is_integer)
	//	CGI_THROW(std::logic_error,"Invalid template type");
	
	
	// Build Structuring Elements
	std::vector<cv::Mat> structuringElements;
	for (std::vector<int>::const_iterator level = ses.begin();level!=ses.end();++level)
	{
		const int lev = *level;
		// Circular structuring element
		const int edgeSize = lev+1+lev; // turn radius into edge size
		structuringElements.push_back(
					cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(edgeSize,edgeSize))
					);
	}

	// Build Morphological Operation Stack (close)
	std::vector<cv::Mat> mos;
	for (unsigned int level = 0;level<structuringElements.size();++level)
	{
		cv::Mat filteredImage(inputImage.size().height, inputImage.size().width, cv::DataType<OperatingPixelType>::type);
		cv::morphologyEx(inputImage, filteredImage, cv::MORPH_CLOSE, structuringElements.at(level));
		mos.push_back(filteredImage);
	}

	const int n_lvls = mos.size();
//	log << cgi::log::Priority::INFO << "opencvCloseDmp" << "MOS Produced, size:" << n_lvls << cgi::log::flush;

	// Set the Tile band count to the SEs array size
	std::vector<cv::Mat> profile;
	profile.reserve(n_lvls);

	// ////////////////////////////////////////////////////////////////////////
	// USE: erosionByReconstructionUphill(const cv::Mat& mask,const cv::Mat& mark)


	cv::Mat tempRec;			// Temporary Close Record
	cv::Mat lastRec;			// previous level Close Record

	// Close = (dilate,erode)
	//typename MorphologyOperator<DataType>::type morphology(_ses[0]);
	//typename ReconstructionOperator<DataType>::type reconstructor;

	//erosionByReconstructionUphill<OperatingPixelType>( inputImage, mos.at(0) ).copyTo(tempRec);
	tempRec = erosionByReconstructionUphill<OperatingPixelType>( inputImage, mos.at(0) ).clone();
	

	// get the absolute difference of the two images
	cv::Mat diffImage(inputImage.size().height, inputImage.size().width, cv::DataType<OperatingPixelType>::type);
	absdiff(inputImage, tempRec, diffImage);

	profile.push_back(diffImage.clone());
	tempRec.copyTo(lastRec);

	// Subsequent Levels
	for (int i = 1;i < n_lvls;++i)
	{
		//  reconstruction(mask     ,  mark)
	        // NOTE: Potentially bad line
	        //erosionByReconstructionUphill<OperatingPixelType>( inputImage, mos.at(i) ).copyTo(tempRec);
		tempRec = erosionByReconstructionUphill<OperatingPixelType>( inputImage, mos.at(i) ).clone();
		
		// BEGIN DEBUG 
		/*
		std::stringstream ss;
		ss << "reconstruction_from_closing_" << i << ".png";
		if(!cv::imwrite(ss.str(), tempRec))
		  {
		    throw std::runtime_error("Failed to write");
		  }
		*/
		// END DEBUG
		
		// get the absolute difference of the two images
		absdiff(lastRec, tempRec, diffImage);
		profile.push_back(diffImage.clone());

		tempRec.copyTo(lastRec);

	}


	return profile;

};

template <typename OperatingPixelType>
cv::Mat erosionByReconstructionUphill(const cv::Mat& mask, const cv::Mat& mark)
{
//	cgi::log::MetaLogStream& log(cgi::log::MetaLogStream::instance());

	if (false == std::numeric_limits<OperatingPixelType>::is_integer)
		//CGI_THROW(std::logic_error,"Invalid template / operating-pixel type");
		throw new std::logic_error("Invalid template / operating-pixel type");

	const cv::Size size = mask.size();

	//if (size.width != mark.size().width || size.height != mark.size().height)
	if (size != mark.size())
	{
		//log << cgi::log::Priority::ERROR << "erosionByReconstructionDownhill"
		//<< "Size mismatch: ("
		//<< size.width << "," << size.height << ") != ("
		//<< mark.size().width << "," << mark.size().height << ")" << cgi::log::flush;
	}

	// --------------------------------
	// Place each p : I(p) > 0 into List[I(p)]
	// --------------------------------
	// Copy the marker into the reconstruction
	//	we can then grow it in place
	// value i has a list of pixels at position (y,x)
	cv::Mat reconstruction(mark.size().height, mark.size().width, cv::DataType<OperatingPixelType>::type);
	mark.copyTo(reconstruction);

	// for template, use typename
	//	typedef typename std::list<std::pair<int,int> > LocationListType;
	//	typedef typename std::map< OperatingPixelType, LocationListType > MapOfLocationListType;
	typedef std::list<std::pair<int,int> > LocationListType;
	typedef std::map< OperatingPixelType, LocationListType > MapOfLocationListType;

	// This is essentially an indexed image version of the input `mark' raster
	MapOfLocationListType valueLocationListMap;

	// Build an in-memory indexed image
	// but while we are at it, verify that it is properly bounded by `mask' raster
	for (int i = 0; i < size.height; ++i)
		for (int j = 0; j < size.width; ++j)
		{
			const OperatingPixelType pixelValue =  reconstruction.at<OperatingPixelType>(i, j);
			if (pixelValue < mask.at<OperatingPixelType>(i, j))
			{
				//std::cout << "Storing ("<< i << "," << j <<") into valueLocationListMap["<< static_cast<int>(pixelValue) << "]" << std::endl;
				std::stringstream msg;
				msg << "(pixelValue < mask[0](i, j)), for raster location = ("<<i<<","<<j<<")";
				//CGI_THROW (std::logic_error, msg.str()); // the marker must always be GTE the mask, by definition
				throw new std::logic_error(msg.str());
			}

			valueLocationListMap[pixelValue].push_back( std::make_pair<int,int>(i,j) );
		}

	// No valid pixels or values below floor?  Return the input as output
	if (valueLocationListMap.size() == 0)
		return reconstruction;

	// --------------------------------
	// The farthest uphill we will go
	//	is to the current max(mark[0])
	//	therefore, we do not need to
	//	process the current max, we
	//	already copied them to output
	const OperatingPixelType maxValue =  valueLocationListMap.rbegin()->first;
	valueLocationListMap[maxValue].clear();

	std::set<std::pair<int,int> > finalizedPixels;

	// --------------------------------
	// Do a forward iteration (uphill)
	// through the
	//	valueLocationListMap[values]
	// --------------------------------
	// use typename for template version
	//for (typename MapOfLocationListType::iterator valueList = valueLocationListMap.begin();
	//			valueList != valueLocationListMap.end(); ++valueList)
	for (typename MapOfLocationListType::iterator valueList = valueLocationListMap.begin();
					valueList != valueLocationListMap.end(); ++valueList)
	{
		// The gray value
		const OperatingPixelType currentValue = valueList->first;
		// The list of (y,x) tuples
		LocationListType locations = valueList->second;

		// for each location indexed in the value list
		while (!locations.empty())
		{

			// Debug thing
//			if (locations.size() > (size.width * size.height))
//			{
//				CGI_THROW(std::logic_error, "GrayscaleDilationByReconstructionDownhill has gone out of control, this should never happen!");
//			}

			// pull/pop the first position from the list, mark it as finalized
			std::pair<int,int> frontPosition = locations.front();
			finalizedPixels.insert(frontPosition);
			locations.pop_front();

			const int y = frontPosition.first;
			const int x = frontPosition.second;

			const int pre_x = x - 1;
			const int post_x = x + 1;
			const int pre_y = y - 1;
			const int post_y = y + 1;

			// For each neighbor
			// - a -
			// b p d
			// - c -
			// a = (pre_y,x), b = (y,pre_x), c = (post_y,x), d = (y,post_x)

			// Neighbor Pixel 'a'
			const std::pair<int,int> a = std::make_pair<int,int>(pre_y,x);
			const OperatingPixelType mask_a = mask.at<OperatingPixelType>(pre_y,x);
			// if neighbor index is within bounds and not finalized
			if ((finalizedPixels.find(a) == finalizedPixels.end()) && (pre_y >= 0) && (mask_a > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(pre_y,x);
				OperatingPixelType constraintValue = std::max<OperatingPixelType>(currentValue,mask_a);

//				std::cout << "Neighbor(a) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue > constraintValue)
				{
					reconstruction.at<OperatingPixelType>(pre_y,x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< pre_y << "," << x <<") to current list" << std::endl;

						locations.push_back( a );
					}
					else
					{
//						std::cout << "Moving ("<< pre_y << "," << x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( a );
						valueLocationListMap[constraintValue].push_back( a );
					}
				}
			}

			// Neighbor Pixel 'b'
			const std::pair<int,int> b = std::make_pair<int,int>(y,pre_x);
			const OperatingPixelType mask_b = mask.at<OperatingPixelType>(y,pre_x);
			if ((finalizedPixels.find(b) == finalizedPixels.end()) && (pre_x >= 0) && (mask_b > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(y,pre_x);
				OperatingPixelType constraintValue = std::max<OperatingPixelType>(currentValue,mask_b);

//				std::cout << "Neighbor(b) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue > constraintValue)
				{
					reconstruction.at<OperatingPixelType>(y,pre_x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< y << "," << pre_x <<") to current list" << std::endl;

						locations.push_back( b );
					}
					else
					{
//						std::cout << "Moving ("<< y << "," << pre_x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( b );
						valueLocationListMap[constraintValue].push_back( b );
					}
				}
			}

			// Neighbor Pixel 'c'
			const std::pair<int,int> c = std::make_pair<int,int>(post_y,x);
			const OperatingPixelType mask_c = mask.at<OperatingPixelType>(post_y,x);
			if ((finalizedPixels.find(c) == finalizedPixels.end()) && (post_y < size.height) && (mask_c > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(post_y,x);
				OperatingPixelType constraintValue = std::max<OperatingPixelType>(currentValue,mask_c);

//				std::cout << "Neighbor(c) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue > constraintValue)
				{
					reconstruction.at<OperatingPixelType>(post_y,x) = constraintValue;
					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< post_y << "," << x <<") to current list" << std::endl;

						locations.push_back( c );
					}
					else
					{
//						std::cout << "Moving ("<< post_y << "," << x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( c );
						valueLocationListMap[constraintValue].push_back( c );
					}
				}
			}

			// Neighbor Pixel 'd'
			const std::pair<int,int> d = std::make_pair<int,int>(y,post_x);
			const OperatingPixelType mask_d = mask.at<OperatingPixelType>(y,post_x);
			if ((finalizedPixels.find(d) == finalizedPixels.end()) && (post_x < size.width) && (mask_d > 0))
			{
				OperatingPixelType neighborValue = reconstruction.at<OperatingPixelType>(y,post_x);
				OperatingPixelType constraintValue = std::max<OperatingPixelType>(currentValue,mask_d);

//				std::cout << "Neighbor(d) = ("<< static_cast<int>(neighborValue) << "), constraint=(" << static_cast<int>(constraintValue) <<")" << std::endl;

				if (neighborValue > constraintValue)
				{
					reconstruction.at<OperatingPixelType>(y,post_x) = constraintValue;

					if ( constraintValue == currentValue )
					{
//						std::cout << "Adding ("<< y << "," << post_x <<") to current list" << std::endl;

						locations.push_back( d );
					}
					else
					{
//						std::cout << "Moving ("<< y << "," << post_x <<") from "
//						<< static_cast<int>(neighborValue) << " to " << static_cast<int>(constraintValue) << std::endl;

						valueLocationListMap[neighborValue].remove( d );
						valueLocationListMap[constraintValue].push_back( d );
					}
				}
			}



		} // end, a value's position list is emptied.

	} // end for each value in the marker



	return reconstruction;


};



#endif //_OPENCV_DMP_HPP_

