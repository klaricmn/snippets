#ifndef _PRETTY_PRINTING_H_
#define _PRETTY_PRINTING_H_

#include <vector>
#include <string>
#include <map>
using namespace std;

string procRow(vector<int> x);

string toString(vector<vector<int> > data);

void printHistogram(const map<int, unsigned long>& hist);

#endif
