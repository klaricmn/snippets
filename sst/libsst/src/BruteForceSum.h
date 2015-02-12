#ifndef _BRUTE_FORCE_SUM_H_
#define _BRUTE_FORCE_SUM_H_

#include <vector>
using namespace std;

namespace sst {

	vector<int> sumCombine(vector< vector< int> > data);

	vector<int> bruteForceSum(vector< vector< int> > data);

	void print_r(const vector<int>& a);

	void print_rr(const vector< vector< int> >& a);
}
;

#endif
