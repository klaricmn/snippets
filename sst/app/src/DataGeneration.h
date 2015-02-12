#ifndef _DATA_GENERATION_H_
#define _DATA_GENERATION_H_

#include <vector>
using namespace std;

vector< vector< int> > generateFakeData(int r, int c, int ub);

vector< vector< vector<int> > > genData3x2();
vector< vector< vector<int> > > genData3x3();
vector< vector< vector<int> > > genData4x4(); 
vector< vector< vector<int> > > genData(unsigned int r, unsigned int c);

#endif
