#ifndef NODE_TEST_SUITE_H_
#define NODE_TEST_SUITE_H_

#include <cxxtest/TestSuite.h>

#include "Node.h"
#include "SortedSumTree.h"
#include "BruteForceSum.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class NodeTestSuite : public CxxTest::TestSuite {
public:

	void setUp() {
		;
	}

	void tearDown() {
		;
	}

	void testCreation1() {
		/*
		 vector<int> emptyvec;
		 vector< vector<int> > emptyvecvec;
		 emptyvecvec.push_back(emptyvec);

		 zzz = new sst::Node(emptyvecvec);
		 TS_ASSERT(0 == zzz->getNumChildren());
		 delete zzz;
		 */
	}

	void testCreation2() {
		/*
		 vector<int> vec1;
		 vec1.push_back(5);
		 vec1.push_back(3);
		 vec1.push_back(2);

		 vector<int> vec2;
		 vec2.push_back(3);
		 vec2.push_back(3);
		 vec2.push_back(2);

		 vector<int> vec3;
		 vec3.push_back(1);
		 vec3.push_back(0);
		 vec3.push_back(0);

		 vector< vector<int> > heightThreeVec;
		 heightThreeVec.push_back(vec1);
		 heightThreeVec.push_back(vec2);
		 heightThreeVec.push_back(vec3);

		 zzz = new sst::Node(heightThreeVec);
		 zzz->initLeftBuildout(false);
		 TS_ASSERT(3 == zzz->getNumChildren());

		 pair<int, vector<int> > res;
		 vector<int> removePath;

		 int count = 0;

		 while (zzz->hasMore()) {
		 //zzz->print();
		 res = zzz->getSumOfLargestSubtree();
		 cout << "LARGEST : " << res.first << endl;
		 removePath = res.second;

		 cout << "Remove path is ";
		 for (unsigned int i=0; i<removePath.size(); ++i)
		 cout << removePath[i] << " ";
		 cout << endl;

		 zzz->removeAndBuildRight(removePath);

		 ++count;
		 }

		 TS_ASSERT(27 == count);

		 delete zzz;
		 */
	}

	void testCreation3() {

		//cout << "***************** START OF 3 **********" << endl;

		vector< vector<int> > data;
		data = readData("foo.txt");

		pair<int, vector<int> > res;

		sst::SortedSumTree* sst = new sst::SortedSumTree(data, true);

		TS_ASSERT(true == sst->hasMore());

		res = sst->getSumOfLargestSubtree(false);

		/*
		 for (unsigned int i=0; i<res.second.size(); ++i)
		 cout << i << " :\t " << res.second[i] << endl;
		 */
		//cout << "***************** END OF 3 **********" << endl;
	}

	void testCreation4() {

		//cout << "***************** START OF 4 **********" << endl;

		vector< vector<int> > data;
		data = readData("foo.txt");

		pair<int, vector<int> > res;

		int last = numeric_limits<int>::max();
		int lastBasedOnPath = numeric_limits<int>::max();

		sst::SortedSumTree* sst = new sst::SortedSumTree(data, true);
		TS_ASSERT(sst->getNumInternalNodes() == 3);
		TS_ASSERT(sst->getNumLeaves() == 1);

		while (sst->hasMore()) {

			//cout << "***" << endl;

			res = sst->getSumOfLargestSubtree(false);

			int tmp = 0;
			for (unsigned int i=0; i<res.second.size(); ++i) {
				tmp += data[i][res.second[i]];
				//cout << i << " :\t " << res.second[i] << endl;
			}

			// sanity check the returned sum
			TS_ASSERT(last >= res.first);
			last = res.first;

			// sanity check the sum based on path
			TS_ASSERT(lastBasedOnPath >= tmp);
			lastBasedOnPath = tmp;

			// sanity check that the two sums match
			TS_ASSERT(last == lastBasedOnPath);
		}

		delete sst;

		//cout << "***************** END OF 4 **********" << endl;
	}

	void testBruteForce() {

		vector< vector<int> > data;
		data = readData("foo.txt");

		vector<int> res;
		res = sst::bruteForceSum(data);

		//sst::print_r(res);
	}

private:
	sst::Node* zzz;

	static vector<vector<int> > readData(string filename) {

		vector<vector<int> > data;

		ifstream ifs(filename.c_str());
		string line;
		int lineCount = 0;
		int value;
		char delim;

		while (getline(ifs, line)) {
			//cout << line << endl;

			data.push_back(vector<int>());

			stringstream ss;
			ss << line;
			while (ss >> value) {
				//cout << "\t" << value << endl;
				data[lineCount].push_back(value);
			}

			++lineCount;
		}

		ifs.close();
		return data;
	}

};

#endif /* NODE_TEST_SUITE_H_ */
