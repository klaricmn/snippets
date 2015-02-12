#include <iostream>

#include <cstring>

#include <Magick++.h>
#include <Magick++/Blob.h>

#include <boost/numeric/ublas/matrix.hpp>

int main(void)
{

  const int h = 256;
  const int w = 256;

  boost::numeric::ublas::matrix< unsigned char > band(h, w);
  for (int y=0; y<h; ++y)
    for(int x=0; x<w; ++x)
      {
	//band(y,x) = y;
	band(y,x) = static_cast<unsigned char>( (y*x) % 256 );
      }

  const unsigned char* __restrict bandPtr = (&(band.data()[0]));

  const int bandLen = sizeof(unsigned char) * h * w;

  char* buffer = new char [ bandLen * 3 ];
  for(int i=0; i<bandLen; ++i)
    {
      buffer[i*3 + 0] = buffer[i*3 + 1] = buffer[i*3 + 2] = bandPtr[i];
    }


  Magick::Blob inputBlob(buffer, 3*bandLen);

  Magick::Image image(inputBlob, Magick::Geometry(w,h), 8, "RGB");
  
  Magick::Blob outputBlob;
  image.write(&outputBlob, "JPEG");

  FILE* fp = fopen("out.jpg", "w");
  fwrite(outputBlob.data(), 1, outputBlob.length(), fp);
  fclose(fp);

  return 0;
}
