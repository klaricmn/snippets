#ifndef STATS_TEST_SUITE_H_
#define STATS_TEST_SUITE_H_

#include <cxxtest/TestSuite.h>

#include "Statistics.h"

#include <iostream>
#include <vector>
using namespace std;

class StatsTestSuite : public CxxTest::TestSuite {
public:

	void setUp() {
		;
	}

	void tearDown() {
		;
	}

	void testVariance() {
		vector<int> a;
		
		a.push_back(9);
		a.push_back(5);
		a.push_back(1);
		
		double var = sst::statistics::calcVariance(a);
		
		TS_ASSERT_DELTA(var, 16, 0.00001);
	}
	
	void testNormalizedVariances() {
		
		vector< vector<int> > a;
		a.push_back(vector<int>());
		a.push_back(vector<int>());
		a.push_back(vector<int>());

		a[0].push_back(9);
		a[0].push_back(5);
		a[0].push_back(1);

		a[1].push_back(8);
		a[1].push_back(4);
		a[1].push_back(3);

		a[2].push_back(7);
		a[2].push_back(6);
		a[2].push_back(5);
		
		vector<double> vars = sst::statistics::calcNormalizedVariances(a);
		
		TS_ASSERT_DELTA(vars[0], 1.0, 0.00001);
		TS_ASSERT_DELTA(vars[1], 0.4375, 0.00001);
		TS_ASSERT_DELTA(vars[2], 0.0625, 0.00001);
	}
	
	void testGettingDataRange() {

		vector< vector<int> > a;
		a.push_back(vector<int>());
		a.push_back(vector<int>());
		a.push_back(vector<int>());

		a[0].push_back(9);
		a[0].push_back(5);
		a[0].push_back(1);

		a[1].push_back(8);
		a[1].push_back(4);
		a[1].push_back(3);

		a[2].push_back(7);
		a[2].push_back(6);
		a[2].push_back(5);
		
		pair<int, int> range = sst::statistics::getDataRange(a);

		TS_ASSERT(range.first == 1);
		TS_ASSERT(range.second == 9);
	}
	
	void testCalcRangeCoverage() {

		vector< vector<int> > a;
		a.push_back(vector<int>());
		a.push_back(vector<int>());
		a.push_back(vector<int>());

		a[0].push_back(9);
		a[0].push_back(5);
		a[0].push_back(1);

		a[1].push_back(8);
		a[1].push_back(4);
		a[1].push_back(3);

		a[2].push_back(7);
		a[2].push_back(6);
		a[2].push_back(5);
		
		pair<int, int> range = sst::statistics::getDataRange(a);

		double p0 = sst::statistics::calcPercentOfRangeCovered(a[0], range);
		double p1 = sst::statistics::calcPercentOfRangeCovered(a[1], range);
		double p2 = sst::statistics::calcPercentOfRangeCovered(a[2], range);
		
		TS_ASSERT_DELTA(p0, 1.0, 0.00001);
		TS_ASSERT_DELTA(p1, 0.6666667, 0.00001);
		TS_ASSERT_DELTA(p2, 0.3333333, 0.00001);
	}
	
	void testCalcRangeCoverages() {

		vector< vector<int> > a;
		a.push_back(vector<int>());
		a.push_back(vector<int>());
		a.push_back(vector<int>());

		a[0].push_back(9);
		a[0].push_back(5);
		a[0].push_back(1);

		a[1].push_back(8);
		a[1].push_back(4);
		a[1].push_back(3);

		a[2].push_back(7);
		a[2].push_back(6);
		a[2].push_back(5);
		
		vector<double> coverages = sst::statistics::calcRangeCoverages(a);
		
		TS_ASSERT_DELTA(coverages[0], 1.0, 0.00001);
		TS_ASSERT_DELTA(coverages[1], 0.6666667, 0.00001);
		TS_ASSERT_DELTA(coverages[2], 0.3333333, 0.00001);
	}

	void testSumOfLists() {
		vector<double> a(4);
		a[0] = 8;
		a[1] = 5;
		a[2] = 3;
		a[3] = 1;
		
		vector<double> b(4);
		b[0] = 7;
		b[1] = 6;
		b[2] = 4;
		b[3] = 2;
		
		vector<double> c = sst::statistics::sumOfLists(a,b);
		
		TS_ASSERT_DELTA(c[0], 15, 0.1);
		TS_ASSERT_DELTA(c[1], 11, 0.1);
		TS_ASSERT_DELTA(c[2], 7, 0.1);
		TS_ASSERT_DELTA(c[3], 3, 0.1);

	}
};

#endif
