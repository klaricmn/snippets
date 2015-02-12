// g++ cdf.cpp  -lboost_unit_test_framework

#include <cmath>
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/accumulators/numeric/functional/vector.hpp>
#include <boost/accumulators/numeric/functional/complex.hpp>
#include <boost/accumulators/numeric/functional/valarray.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/p_square_cumul_dist.hpp>

#include <vector>
#include <utility>

using namespace boost;
using namespace boost::accumulators;
//using namespace unit_test;

typedef iterator_range<std::vector<std::pair<double, double> >::iterator > histogram_type;

std::vector<std::pair<double, int> mapdata(histogram_type& h)
{
  const double mapToBlackPct = 0.2;
  const int otherSteps = 15;
  const double stepSizePct = (1-mapToBlackPct) / otherSteps;
  std::cout << "stepSizePct = " << stepSizePct << std::endl;

  std::vector<std::pair<double, int> mapping;

  for (std::size_t i = 0; i < h.size(); ++i)
    {
      int step = -1;
      if(h[i].second < mapToBlackPct)
	step = 0;
      else
	step = std::min(otherSteps, ceil((h[i].second - mapToBlackPct) / stepSizePct));
      std::cout << "[" << h[i].first << "," << h[i].second << "]"  << " ==> " << step << std::endl;

      // NOTE: Using -1 here so that the searches are easier later
      mapping.push_back(std::make_pair(h[i].first, step-1));
    }

  return mapping;
}

int main(void)
{

  // tolerance in %
  double epsilon = 3;

  typedef accumulator_set<double, stats<tag::p_square_cumulative_distribution> > accumulator_t;

  accumulator_t acc(tag::p_square_cumulative_distribution::num_cells = 100);

  // two random number generators
  boost::lagged_fibonacci607 rng;
  boost::normal_distribution<> mean_sigma(0,1);
  boost::variate_generator<boost::lagged_fibonacci607&, boost::normal_distribution<> > normal(rng, mean_sigma);

  for (std::size_t i=0; i<100000; ++i)
    {
      acc(normal());
    }

  histogram_type histogram = p_square_cumulative_distribution(acc);
  //histogram_type histogram;

  for (std::size_t i = 0; i < histogram.size(); ++i)
    {
      std::cout << "[" << histogram[i].first << "," << histogram[i].second << "]" << std::endl;

      /*
      // problem with small results: epsilon is relative (in percent), not absolute!
      if ( histogram[i].second > 0.001 )
        BOOST_CHECK_CLOSE( 0.5 * (1.0 + erf( histogram[i].first / sqrt(2.0) )), histogram[i].second, epsilon );
      */
    }

  std::cout << "*************************" << std::endl;
  std::vector<std::pair<double, int> mapping (mapdata(histogram));

  std::vector<double> vals;
  vals.push_back(-0.75);
  vals.push_back(-0.25);
  vals.push_back(0);
  vals.push_back(0.5);
  vals.push_back(2);
  //vals.push_back(4.2);

  for(std::vector<double>::const_iterator iter; iter!=vals.end(); ++iter)
    {
       
    }

  return 0;
}
