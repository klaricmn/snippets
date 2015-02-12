#include "InternalNode.h"

#include <iostream>
#include <cassert>
#include <limits>
using namespace std;

sst::InternalNode::InternalNode(int nodeValue, const vector< vector< int> >& data, int level) :
	_nodeValue(nodeValue), Node(data, level) {
	;
}

sst::InternalNode::~InternalNode() {
	;
}

pair<int, vector<int> > sst::InternalNode::getSumOfLargestSubtree(bool debug) const {

	if (debug)
		cout << "Entered sst::InternalNode::getSumOfLargestSubtree()" << endl;

	// init the objects that we'll use to track the best solution and the current candidate
	pair<int, vector<int> > best, candidate;
	best.first = numeric_limits<int>::min();

	if (_children.size() == 0) {
		// if we've hit a leaf, then simply return the node value
		if (debug)
			cout << "Base case for sum. Returning " << _nodeValue << endl;
		best.first = _nodeValue;
		best.second.clear();
	} else {

		// othewise, we have to find the child with the best sum		
		for (unsigned int i=0; i<_children.size(); ++i) {
			if (NULL != _children[i]) {

				// first pull out what we have in cache
				pair<int, vector<int> >& candidate = _cachedResult[i];

				// if we find out that we really don't have a value cached, 
				// then we need to calculate it
				if (false == _isResultCached[i]) {
					if (debug)
						cout << "CACHE_REPORT: sst::InternalNode : Calculating a value for " << i << endl;
					candidate = _children[i]->getSumOfLargestSubtree(debug);
					_isResultCached[i] = true;
					_cachedResult[i] = candidate;
				} else {
					if (debug)
						cout << "CACHE_REPORT: sst::InternalNode : Using a cached value for " << i << endl;
				}

				if (debug)
					cout << "sum of [" << i << "] = " << candidate.first << endl;
				if (candidate.first >= best.first) {
					best = candidate;
					best.second.push_back(i);
				}
			}
		}

		// now that we've found the best child, also add in the score for this node
		best.first += _nodeValue;

		if (debug) {
			cout << "largest = " << best.first << endl;
			cout << "indexOfLargest = " << best.second[best.second.size()-1] << endl;
		}

		assert(best.second[best.second.size()-1] >= 0);

	}
	if (debug)
		cout << "Leaving sst::InternalNode::getSumOfLargestSubtree()" << endl;

	return best;
}

void sst::InternalNode::print(unsigned int indentation) const {
	for (unsigned int i=0; i<indentation; ++i)
		cout << " ";
	cout << _nodeValue << endl;

	for (unsigned int i=0; i<_children.size(); ++i) {
		if (NULL != _children[i])
			_children[i]->print(indentation+1);
	}

}
