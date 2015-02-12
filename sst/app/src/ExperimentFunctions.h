#ifndef _EXPERIMENT_FUNCTIONS_H_
#define _EXPERIMENT_FUNCTIONS_H_

#include <fstream>
#include <vector>
using namespace std;

void runSST(ofstream& os, vector< vector< int> > data, long granularity);
unsigned long runSSTToGetTotalDepth(vector< vector< int> > data);
void runSSTDepthTest();
void runSSTListReorgTest();
void runBruteForce(ofstream& os, vector< vector< int> > data);

#endif
