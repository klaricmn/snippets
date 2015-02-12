#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <vector>
#include <utility>
using namespace std;

namespace sst {
	namespace statistics {

		///
		/// Calculate the variance for a sample of numbers
		/// \param A sample of numbers
		/// \return The sample variance
		///
		double calcVariance(vector<int> x);
		
		///
		/// Calculate the normalized variances for multiple samples of numbers.  
		/// The variance is calculated for each sample.  Then, each variance is 
		/// divided by the largest in order to normalize them to the range [0,1].  
		/// \param d The vector of samples of numbers
		/// \return A vector of variances
		///
		vector<double> calcNormalizedVariances(const vector< vector< int> >& d);

		///
		/// Get the range of data present in the samples.  (i.e. Find the min and the max.)
		/// \param d The vector of samples of numbers
		/// \return A pair containing the min and max values.  
		///
		pair<int, int> getDataRange(const vector< vector< int> >& d);

		///
		/// Calculate the percent of the range covered by the data points in the sample.  
		/// \param d Sample data
		/// \param range Range of values
		/// \return Percent of range covered by (the min and max values of) the sample
		///
		double calcPercentOfRangeCovered(const vector< int>& d, const pair<int, int>& range);

		///
		/// Generate a vector of percentages of the fraction of data covered by each sample (row).  
		/// The range is determined by the min and max values stored in any sample in d.  
		/// \param d Sample data
		/// \return A vector of the coverage percentages.  
		///
		vector<double> calcRangeCoverages(const vector< vector< int> >& d);

		///
		/// Perform an element by element addition on two vectors.  
		/// \param x Vector 1
		/// \param y Vector 2
		/// \return The result of the addition.  
		///
		vector<double> sumOfLists(const vector<double>& x, const vector<double> &y);
	}
}

#endif
