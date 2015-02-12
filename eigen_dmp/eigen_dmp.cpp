#include <iostream>
#include <Eigen/Dense>

#include <cgi/logger/logger.hpp>
#include <cgi/mosaic/Mosaic.hpp>
#include <cgi/mosaic/MosaicUtility.hpp>
#include <boost/filesystem/convenience.hpp>

static const int nBands = 8;

namespace Eigen {
  typedef Eigen::Matrix<float, nBands, nBands> Matrix8f;
  typedef Eigen::Matrix<float, nBands, 1> Vector8f;
}

Eigen::Vector8f v2v(const std::vector<unsigned short>& sVec)
{
  Eigen::Vector8f eVec;
  for(unsigned int i=0; i<sVec.size(); ++i)
    eVec(i,0) = sVec[i];
  return eVec;
}

std::vector<unsigned short> v2v(const Eigen::VectorXf& eVec)
{
  std::vector<unsigned short> sVec(eVec.rows());
  for(unsigned int i=0; i<sVec.size(); ++i)
    sVec[0] = eVec(i,0);
  return sVec;
}

cgi::Tile<unsigned char> threshold(const cgi::Tile<float>& inTile, const Eigen::VectorXf& thresh)
{
  const cgi::core::Size2d<int>& size(inTile.getSize());
  const int height = size.getHeight();
  const int width = size.getWidth();
  const int depth = inTile.getBandCount();

  cgi::Tile<unsigned char> outTile(size, depth, 0);

  for(int r=0; r<height; ++r)
    {
      for(int c=0; c<width; ++c)
	{
	  for(int b=0; b<depth; ++b)
	    {
	      if(inTile[b](r,c) > thresh(b,0))
		outTile[b](r,c) = 255;
	    }
	}
    }

  return outTile;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  if(argc != 4)
    {
      std::cerr << "USAGE: " << argv[0] << " <input filename> <output filename> <number of output levels>" << std::endl;
      return 1;
    }

  cgi::log::MetaLogStream::instance().setTargets(cgi::log::STDOUT);
  cgi::log::MetaLogStream::instance().setPriorityThreshold(cgi::log::Priority::DEBUG);
  cgi::log::MetaLogStream& log (cgi::log::MetaLogStream::instance());
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  const std::string outputFile(argv[2]);
  CGI_THROW_IF(true == boost::filesystem::exists(outputFile), std::runtime_error);

  const int l = boost::lexical_cast<int>(argv[3]);

  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  cgi::Mosaic m;
  CGI_THROW_IF(cgi::NoError != m.open(argv[1]), std::runtime_error);

  CGI_THROW_IF(nBands != m.getBandCount(), std::runtime_error);

  m.setTileSize(cgi::core::Size2d< int >(1000,1000));
 
  const cgi::core::Size2d< int > &size = m.getTileSize();
  const int numRows = size.getHeight();
  const int numCols = size.getWidth();

  //const int everyNthTile = m.getColumnCount()/2 - 1;
  const int everyNthTile = 100;

  // initialization
  Eigen::Vector8f mean = Eigen::Vector8f::Zero();
  int validPixelCount = 0;

  log << cgi::log::Priority::INFO << "eigen_dmp" << "Calculating mean for..." << cgi::log::flush;

  // Calculate the mean vector across all tiles
  for(int i=0; i<m.getTileCount(); ++i)
    {
      if(0 != (i % everyNthTile))
	continue;

      log << cgi::log::Priority::INFO << "eigen_dmp" << "Tile " << i << " of " << m.getTileCount() << cgi::log::flush;

      const cgi::Tile<unsigned short> t(m.getTile<unsigned short>(i));
      const boost::numeric::ublas::matrix< bool > mask (t.getValidMask(cgi::valid_mask::ALL));

      for(int r=0; r<numRows; ++r)
	{
	  for(int c=0; c<numCols; ++c)
	    {
	      if(mask(r,c))
		{
		  const Eigen::Vector8f eVec(v2v(t(r,c)));
		  mean += eVec;
		  ++validPixelCount;
		}
	    }
	}
      
    }

  mean /= validPixelCount;
  std::cout << "mean = " << std::endl << mean << std::endl;

  ////////////////////////////////////////////////////////

  //
  // Calculate the covariance matrix
  //
  
  Eigen::Matrix8f cov = Eigen::Matrix8f::Zero();

  log << cgi::log::Priority::INFO << "eigen_dmp" << "Calculating covariance for..." << cgi::log::flush;

  for(int i=0; i<m.getTileCount(); ++i)
    {
      if(0 != (i % everyNthTile))
	continue;

      log << cgi::log::Priority::INFO << "eigen_dmp" << "Tile " << i << " of " << m.getTileCount() << cgi::log::flush;

      const cgi::Tile<unsigned short> t(m.getTile<unsigned short>(i));
      const boost::numeric::ublas::matrix< bool > mask (t.getValidMask(cgi::valid_mask::ALL));
  
      for(int r=0; r<numRows; ++r)
	{
	  for(int c=0; c<numCols; ++c)
	    {
	      if(mask(r,c))
		{

		  const Eigen::Vector8f eVec(v2v(t(r,c)));
		  cov += (eVec - mean) * (eVec - mean).transpose();
		}
	    }
	}
    }

  cov /= validPixelCount;
  //cov -= mean * mean.transpose();
  std::cout << "cov = " << std::endl << cov << std::endl;

  ////////////////////////////////////////////////////////

  Eigen::SelfAdjointEigenSolver<Eigen::Matrix8f> eigensolver(cov);
  
  if (eigensolver.info() != Eigen::Success) 
    abort();


  std::cout << "The eigenvalues of A are:" << std::endl << eigensolver.eigenvalues() << std::endl;
  std::cout << "Here's a matrix whose columns are eigenvectors of A " << std::endl
	    << "corresponding to these eigenvalues:" << std::endl
	    << eigensolver.eigenvectors() << std::endl;
  

  ////////////////////////////////////////////////////////

  Eigen::Vector8f evals = eigensolver.eigenvalues();
  evals /= evals(evals.rows()-1, 0);
  std::cout << "Cummulative energy content for each eigenvector: " << std::endl
	    << evals << std::endl;

  ////////////////////////////////////////////////////////

  //const Eigen::Vector8f z = eigensolver.eigenvectors().col(nBands-1);
  Eigen::MatrixXf zz(nBands,l);
  zz = eigensolver.eigenvectors().block(0,nBands-l, nBands,l);

  ////////////////////////////////////////////////////////

  Eigen::VectorXf meanOfEigenImage = zz.transpose() * mean;
  std::cout << "meanOfEigneImage = " << std::endl
	    << meanOfEigenImage << std::endl;

  ////////////////////////////////////////////////////////

  const cgi::Tile<unsigned short> t(m.getTile<unsigned short>(m.getTileIndex1D(m.getRowCount()/2, m.getColumnCount()/2)));
  cgi::Tile<float> tt(t.getSize(), l);
  
  for(int r=0; r<numRows; ++r)
    {
      for(int c=0; c<numCols; ++c)
        {
	  const Eigen::Vector8f eVec(v2v(t(r,c)));

	  //const float pp = z.transpose() * (eVec - mean);
	  const Eigen::VectorXf ppv (zz.transpose() * (eVec - mean));

	  for(int b=0; b < ppv.rows(); ++b)
	    tt[b](r,c) = ppv(b,0);
        }
    }
  
  cgi::MosaicUtility::writeImage<unsigned short>(t, "dmp.tif");
  cgi::MosaicUtility::writeImage<float>(tt, "out.tif");

  cgi::MosaicUtility::writeImage<unsigned char>(threshold(tt, 3*meanOfEigenImage), "thresh.tif");

  ////////////////////////////////////////////////////////

  return 0;

  /*
  Eigen::Matrix8f A;
  
  //srand(time(NULL));

  const int n = nBands;
  for(int i=0; i<n; ++i)
    for(int j=i; j<n; ++j)
      A(i,j) = A(j,i) = (-1 + (2 * (rand()%2))) * (rand() / static_cast<float>(RAND_MAX));

  cout << "Here is the matrix A:\n" << A << endl;
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix8f> eigensolver(A);
  if (eigensolver.info() != Eigen::Success) abort();
  cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
  cout << "Here's a matrix whose columns are eigenvectors of A \n"
       << "corresponding to these eigenvalues:\n"
       << eigensolver.eigenvectors() << endl;

  */
}
