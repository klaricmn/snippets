#include "ExperimentFunctions.h"

#include "VectorSortWithIndex.h"
#include "PrettyPrinting.h"
#include "Statistics.h"
#include "DataGeneration.h"

#include "sst/SortedSumTree.h"
#include "sst/BruteForceSum.h"
#include "sst/Timing.h"

#include <cmath>
#include <iostream>
#include <numeric>
#include <map>
#include <set>

/* ****************************************************************************** */
/* ****************************************************************************** */

void runSST(ofstream& os, vector< vector< int> > data, long granularity) {
	sst::Timing t;
	t.start();

	sst::SortedSumTree* sst = new sst::SortedSumTree(data, false);

	long iteration = 0;
	const long expected = sst->getTotalCount();
	// number of steps translates into this many iterations
	const long clock_every_n_iterations = (long) ceil(expected / (double) granularity);
	long clock_stop = 0;

	pair<int, vector<int> > res;
	while (sst->hasMore()) {

		res = sst->getSumOfLargestSubtree(false);

		++iteration;
		if ( (iteration % clock_every_n_iterations) == 0) {
			++clock_stop;

			os << (clock_stop / (double)granularity) << " " << t.getTotalElapsedTime() << endl;
		}
	}

	os << 1 << " " << t.getTotalElapsedTime() << endl;

	delete sst;

	t.stop();
}

/* ****************************************************************************** */
/* ****************************************************************************** */

unsigned long runSSTToGetTotalDepth(vector< vector< int> > data) {

	sst::SortedSumTree* sst = new sst::SortedSumTree(data, false);

	unsigned long total = 0;

	pair<int, vector<int> > res;
	while (sst->hasMore()) {

		const unsigned int curr = sst->getNumLeaves();
		total += curr;
		//cout << curr << "\t" << total << "\n";

		res = sst->getSumOfLargestSubtree(false);
	}
	/*
	 cout << total << " : ((" << data[0][0] << "," << data[0][1] << "," << data[0][2] << "),(" << data[1][0] << ","
	 << data[1][1] << "," << data[1][2] << "),(" << data[2][0] << "," << data[2][1] << "," << data[2][2] << "))"
	 << endl;
	 */
	delete sst;

	return total;
}

/* ****************************************************************************** */
/* ****************************************************************************** */
void runSSTDepthTest() {
	unsigned int r=3, c=3;

	vector< vector< vector<int> > > testData;
	testData = genData(r, c);

	vector<unsigned long> totalDepths;

	for (unsigned int i=0; i<testData.size(); ++i) {
		totalDepths.push_back(runSSTToGetTotalDepth(testData[i]));
	}

	//////////////////////////////////////////////////////////////		
	VectorSortWithIndex<unsigned long> zzz(totalDepths);

	for (unsigned int i=0; i<zzz.size(); ++i) {
		cout << zzz.getOriginalIndexForKthSorted(i) << "\t : " << zzz.getDataForKthSorted(i) << "\t : "
				<< toString(testData[zzz.getOriginalIndexForKthSorted(i)]) << endl;
	}

	totalDepths = zzz.getData();
	vector<int> yyy(totalDepths.size());
	adjacent_difference(totalDepths.begin(), totalDepths.end(), yyy.begin());

	for (unsigned int i=0; i<yyy.size(); ++i)
		cout << yyy[i] << endl;

	map<int, unsigned long> hist;
	hist = genHistogram(yyy);
	printHistogram(hist);
}

/* ****************************************************************************** */
/* ****************************************************************************** */
// check to see if there is a 1-to-1 match between a set in 'a' and a set in 'b'
bool compareListOfSets(const vector< vector<int> >& a, const vector< vector<int> >& b) {

	if (a.size() != b.size())
		return false;

	vector<int> usedFromB;
	for (unsigned int i=0; i<a.size(); ++i) {

		for (unsigned int j=0; j<b.size(); ++j) {
			if (usedFromB.end() == find(usedFromB.begin(), usedFromB.end(), j)) {
				// if we haven't used element j from b, then

				if (a[i] == b[j]) {
					// if element i in a matches element j in b, then
					// mark j as used.  
					usedFromB.push_back(j);
				}
			}
		}

	}

	if (usedFromB.size() != a.size())
		return false;

	return true;
}

/* ****************************************************************************** */
/* ****************************************************************************** */
void runSSTListReorgTest() {

	// generate the data
	unsigned int r=3, c=3;
	vector< vector< vector<int> > > testData;
	testData = genData(r, c);

	// generate a mapping of the data
	set<unsigned int> usedDataPoints;
	vector< vector< unsigned int> > mapping;
	for (unsigned int i=0; i<testData.size(); ++i) {
		if (usedDataPoints.end() == find(usedDataPoints.begin(), usedDataPoints.end(), i)) {

			mapping.push_back(vector<unsigned int>() );
			mapping[mapping.size()-1].push_back(i);

			// if data point i hasn't been used, then
			for (unsigned int j=i+1; j<testData.size(); ++j) {
				if (true == compareListOfSets(testData[i], testData[j])) {
					mapping[mapping.size()-1].push_back(j);
					usedDataPoints.insert(j);
				}
			}
		}
	}

	// now run through the mapping
	for (unsigned int i=0; i<mapping.size(); ++i) {
		cout << "**" << i << "**" << endl;
		for (unsigned int j=0; j<mapping[i].size(); ++j) {

			const vector<vector<int> >& currData = testData[mapping[i][j]];

			cout << toString(currData) << " " << runSSTToGetTotalDepth(currData) << " ";
			/*
			for (unsigned int k=0; k<currData.size(); ++k) {
				cout << calcVariance(currData[k]) << " " << calcSum(currData[k]) << " " << calcAdjDiff(currData[k]) << " ";
			}
			*/
			cout << calcSpreadOverlap(currData);
			cout << endl;
		}
	}

	// now run the experiments
	/*
	 vector<unsigned long> totalDepths;
	 for (unsigned int i=0; i<testData.size(); ++i) {
	 totalDepths.push_back(runSSTToGetTotalDepth(testData[i]));
	 }
	 */

}

/* ****************************************************************************** */
/* ****************************************************************************** */

void runBruteForce(ofstream& os, vector< vector< int> > data) {

	sst::Timing t;
	t.start();

	sst::bruteForceSum(data);

	os << "BRUTE_FORCE" << " " << t.getTotalElapsedTime() << endl;
	t.stop();
}
