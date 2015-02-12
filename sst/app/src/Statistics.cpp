#include "Statistics.h"

#include <numeric>
#include <cmath>
#include <iostream>
using namespace std;

map<int, unsigned long> genHistogram(vector<int> d) {

	map<int, unsigned long> hist;

	for (unsigned int i=0; i<d.size(); ++i)
		++hist[d[i]];

	return hist;
}

/**********************************************/
/**********************************************/
double calcVariance(vector<int> x) {

	const int sum = accumulate(x.begin(), x.end(), 0);
	const double mean = sum / (double) x.size();

	double var = 0;
	for (unsigned int i=0; i<x.size(); ++i)
		var += (x[i] - mean) * (x[i] - mean);

	return (1.0/x.size()) * var;
}

/**********************************************/
/**********************************************/
double calcSum(vector<int> x) {
	return accumulate(x.begin(), x.end(), 0);
}

/**********************************************/
/**********************************************/
double calcAdjDiff(vector<int> x) {

	vector<int> y(x.size());
	adjacent_difference(x.begin(), x.end(), y.begin());

	return -1 * accumulate(y.begin()+1, y.end(), 0);
}

/**********************************************/
/**********************************************/
int calcSpreadOverlap(const vector< vector< int> >& d) {

	if (d.size() == 0)
		return 0;

	int sumOfMaxes = 0, sumOfMins = 0;

	// get the min and max path down the tree below the top row
	for (unsigned int i=1; i<d.size(); ++i) {
		sumOfMaxes += d[i][0];
		sumOfMins += d[i][d[i].size()-1];
	}
	//cout << "sumOfMaxes = " << sumOfMaxes << endl;
	//cout << "sumOfMins = " << sumOfMins << endl;
	
	int sumOfOverlap = 0;

	for (unsigned int i=1; i<d[0].size(); ++i) {
		const int tmp1 = (d[0][i] + sumOfMaxes) - (d[0][i-1] + sumOfMins);
		//cout << " tmp1 = " << tmp1 << endl;
		sumOfOverlap += tmp1>0?tmp1:0;
	}

	return sumOfOverlap;
}
