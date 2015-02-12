#include "SST_Experiment.h"

#include "DataGeneration.h"
#include "ExperimentFunctions.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
using namespace std;

/* ****************************************************************************** */
/* ****************************************************************************** */

///
///
/// 
vector<vector<int> > readData(string filename) {

	vector<vector<int> > data;

	ifstream ifs(filename.c_str());
	assert(ifs.is_open() == true);
	string line;
	int lineCount = 0;
	int value;

	while (getline(ifs, line)) {
		cout << line << endl;

		data.push_back(vector<int>());

		stringstream ss;
		ss << line;
		while (ss >> value) {
			cout << "\t" << value << endl;
			data[lineCount].push_back(value);
		}

		++lineCount;
	}

	ifs.close();
	return data;
}

/* ****************************************************************************** */
/* ****************************************************************************** */

int main(int argc, char** argv) {

	if (argc != 3) {
		cerr << "USAGE: " << argv[0] << " <data size> <timing granularity>" << endl;
		return 1;
	}

	//////////////////////////////////////////////////////////////	

	const int dataSize = atoi(argv[1]);
	if (dataSize <= 0) {
		cerr << "ERROR: data size was not positive" << endl;
		return 2;
	}

	// number of steps to take
	const long granularity = atol(argv[2]);
	if (granularity <= 0) {
		cerr << "ERROR: granularity was not positive" << endl;
		return 3;
	}

	//////////////////////////////////////////////////////////////	

	srand(13);

	//////////////////////////////////////////////////////////////	

	stringstream ss;
	ss << "sst." << dataSize << ".txt";

	ofstream os(ss.str().c_str());
	assert(os.is_open() == true);

	//////////////////////////////////////////////////////////////	

	vector< vector<int> > data;
	data = generateFakeData(dataSize, dataSize, 1000);
	//sst::print_rr(data);

	//////////////////////////////////////////////////////////////	

	//runSST(os, data, granularity);
	//runBruteForce(os, data);

	//runSSTDepthTest();
	runSSTListReorgTest();

	os.close();
	return 0;
}
