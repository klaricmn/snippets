#include "PrettyPrinting.h"
#include <iostream>
#include <sstream>

string procRow(vector<int> x) {

	stringstream ss;
	ss << "(";

	for (unsigned int i=0; i<x.size(); ++i) {
		ss << x[i];

		if ( (i+1) != x.size() )
			ss << ",";
	}

	ss << ")";
	return ss.str();
}

string toString(vector<vector<int> > data) {

	stringstream ss;
	ss << "(";

	for (unsigned int i=0; i<data.size(); ++i) {
		ss << procRow(data[i]);

		if ( (i+1) != data.size() )
			ss << ",";
	}

	ss << ")";
	return ss.str();
}

void printHistogram(const map<int, unsigned long>& hist) {

	for (map<int, unsigned long>::const_iterator iter=hist.begin(); iter!=hist.end(); ++iter) {
		cout << iter->first << "\t : " << iter->second << endl;
	}

}
