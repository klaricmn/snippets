#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <map>
#include <vector>
using namespace std;

map<int, unsigned long> genHistogram(vector<int> d);

double calcVariance(vector<int> x);
double calcSum(vector<int> x);
double calcAdjDiff(vector<int> x);

int calcSpreadOverlap(const vector< vector< int > >& d);

#endif
