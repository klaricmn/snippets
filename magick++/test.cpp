#include <Magick++.h>

#include <vector>
#include <cstdio>

void test1()
{
  Magick::Image image(Magick::Geometry(100,100), "black");

  Magick::ColorRGB colorPixel;

  for(unsigned int y=0; y<100; ++y)
    for(unsigned int x=0; x<100; ++x)
      {
	double val = (x+y) / 200.0;

	if(val < 0.25)
	  val = 0;
	else if(val > 0.75)
	  val = 1;

	colorPixel.red(val);
	colorPixel.green(val);
	colorPixel.blue(val);
	
	image.pixelColor(x,y, colorPixel);
      }

  image.transparent("black");
  image.write("out1.png");
}

void test2()
{
  Magick::Image image(Magick::Geometry(100, 100), "black");
  Magick::ColorRGB colorPixel;
  
  
				       
}

int main(void)
{

  test1();

  test2();

  return 0;
}
