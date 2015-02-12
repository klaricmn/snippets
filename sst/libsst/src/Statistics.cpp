#include "Statistics.h"

#include <numeric>
#include <cassert>
#include <iostream>
#include <limits>
#include <algorithm>

/**********************************************/
/**********************************************/
double sst::statistics::calcVariance(vector<int> x) {

	if (x.size() == 0)
		return 0;

	const int sum = accumulate(x.begin(), x.end(), 0);
	const double mean = sum / (double) x.size();

	double var = 0;
	for (unsigned int i=0; i<x.size(); ++i)
		var += (x[i] - mean) * (x[i] - mean);

	return (1.0/(x.size()-1)) * var;
}

/**********************************************/
/**********************************************/
vector<double> sst::statistics::calcNormalizedVariances(const vector< vector< int> >& d) {

	vector<double> variances(d.size());

	for (unsigned int i=0; i<d.size(); ++i) {
		variances[i] = calcVariance(d[i]);
	}

	const double maxVariance = *std::max_element(variances.begin(), variances.end());

	if (maxVariance == 0)
		return variances;

	for (unsigned int i=0; i<d.size(); ++i) {
		variances[i] /= maxVariance;
	}

	return variances;
}

/**********************************************/
/**********************************************/
pair<int, int> sst::statistics::getDataRange(const vector< vector< int> >& d) {

	pair<int, int> range;
	range.first = std::numeric_limits<int>::max();
	range.second = std::numeric_limits<int>::min();

	for (unsigned int i=0; i<d.size(); ++i) {

		vector<int>::const_iterator minElem = std::min_element(d[i].begin(), d[i].end());
		vector<int>::const_iterator maxElem = std::max_element(d[i].begin(), d[i].end());

		if (range.first > *minElem)
			range.first = *minElem;
		if (range.second < *maxElem)
			range.second = *maxElem;

	}

	return range;
}

/**********************************************/
/**********************************************/
double sst::statistics::calcPercentOfRangeCovered(const vector< int>& d, const pair<int, int>& range) {

	// in this function, the range given will always be wider (or equal to)
	// the data provided in d.  

	const int min = *min_element(d.begin(), d.end());
	const int max = *max_element(d.begin(), d.end());
	const int widthOfD = max - min + 1;

	const int widthOfRange = range.second - range.first + 1;

	return widthOfD / (double) widthOfRange;
}

/**********************************************/
/**********************************************/
vector<double> sst::statistics::calcRangeCoverages(const vector< vector< int> >& d) {

	vector<double> coverages(d.size());

	pair<int, int> range = getDataRange(d);

	for (unsigned int i=0; i<d.size(); ++i) {
		coverages[i] = calcPercentOfRangeCovered(d[i], range);
	}

	return coverages;
}

/**********************************************/
/**********************************************/
vector<double> sst::statistics::sumOfLists(const vector<double>& x, const vector<double> &y) {

	assert(x.size() == y.size());

	vector<double> z(x.size());
	for (unsigned int i=0; i<x.size(); ++i) {
		z[i] = x[i] + y[i];
	}

	return z;
}
