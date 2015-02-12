#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

#include "BruteForceSum.h"

vector<int> sst::sumCombine(vector< vector< int > > data) {
 

  //cout << "Depth = " << data.size() << endl;
  //print_rr(data);

  if(data.size() == 1) {
    return data[0];
  }
  else {

    vector<int> one,two;
    one = data.back(); data.pop_back();
    two = data.back(); data.pop_back();

    vector<int> combo;
    for(unsigned int i=0; i<one.size(); ++i)
      for(unsigned int j=0; j<two.size(); ++j)
	combo.push_back(one[i] + two[j]);

    data.push_back(combo);

    return sumCombine(data);

  }

}

vector<int> sst::bruteForceSum(vector< vector< int > > data) {
  
  vector<int> summed;
  summed = sumCombine(data);
  
  sort(summed.begin(), summed.end(), greater<int>());

  return summed;
}

void sst::print_r(const vector<int>& a) {
  for(unsigned int i=0; i<a.size(); ++i) {
    cout << a[i] << endl;
  }
}

void sst::print_rr(const vector< vector< int > >& a) {
  for(unsigned int i=0; i<a.size(); ++i) {
    for(unsigned int j=0; j<a[i].size(); ++j) {
      cout << a[i][j] << " ";
    }
    cout << endl;
  }
}
