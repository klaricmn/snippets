#include <iostream>
#include <math.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_wavelet2d.h>

#include <cgi/mosaic/Mosaic.hpp>

void normalizeBySum(double* data, int n)
{
  int sum = 0;
  for(int i=0; i<n; ++i)
    sum += data[i];

  for(int i=0; i<n; ++i)
    data[i] /= sum;
}
   
int
main (int argc, char **argv)
{
  
  const int n = 1024;

  if(argc < 2)
    {
      std::cout << "ERROR" << std::endl;
      return 1;
    }

  
  cgi::Mosaic m;
  if(cgi::NoError != m.open(argv[1]))
    {
      std::cout << "Failed to open" << std::endl;
      return 2;
    }

  m.setTileSize(cgi::core::Size2d<int>(n,n));
  
  double* data = new double [n * n];
  if(cgi::NoError != m.getTile<double>(0, 0, data))
    {
      std::cout << "getTile failed" << std::endl;
      return 3;
    }

  gsl_wavelet *w;
  gsl_wavelet_workspace *work;
     
  w = gsl_wavelet_alloc (gsl_wavelet_daubechies, 4);
  work = gsl_wavelet_workspace_alloc (n);
  
  if(GSL_SUCCESS != gsl_wavelet2d_transform_forward (w, data, n, n, n, work))
    {
      std::cout << "Transform failed" << std::endl;
      return 4;
    }

  /////////////////////////////////////////
  cgi::Mosaic moDwt;
  moDwt.create("dwt.tif", "GTiff", cgi::core::Size2d<int>(n,n), 1, cgi::Depth64F);
  moDwt.setTileSize(cgi::core::Size2d<int>(n,n));
  moDwt.putTile<double>(data, 0, 0);
  moDwt.close();

  /////////////////////////////////////////
  // normalization
  normalizeBySum(data, n);

  /////////////////////////////////////////
  // do the inverse and reconstruct the image

  if(GSL_SUCCESS != gsl_wavelet2d_transform_inverse (w, data, n, n, n, work))
    {
      std::cout << "Transform failed" << std::endl;
      return 5;
    }


  /////////////////////////////////////////
  // convert the reconstructed image to unsigned short
  unsigned short* recData = new unsigned short [n * n];
  int i;
  for(i=0; i<(n*n); ++i)
    {
      recData[i] = static_cast<unsigned short>(std::max(std::min(data[i], 65535.0), 0.0));
    }

  cgi::Mosaic moRec;
  moRec.create("rec.tif", "GTiff", cgi::core::Size2d<int>(n,n), 1, cgi::Depth16U);
  moRec.setTileSize(cgi::core::Size2d<int>(n,n));
  moRec.putTile<unsigned short>(recData, 0, 0);
  moRec.close();
  
  /////////////////////////////////////////
  gsl_wavelet_free (w);
  gsl_wavelet_workspace_free (work);

  delete [] recData;
  delete [] data;

  return 0;
}
