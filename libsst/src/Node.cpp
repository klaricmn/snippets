#include "Node.h"
#include "InternalNode.h"

#include <iostream>
#include <utility>
#include <cassert>
#include <limits>

using namespace std;

sst::Node::Node(const vector< vector< int> >& data, int level) :
	_data(data), _level(level) {

	;

}

sst::Node::~Node() {

	for (unsigned int i=0; i<_children.size(); ++i)
		delete _children[i];
}

void sst::Node::initLeftBuildout(bool debug) {

	// if the number of lists is greater than 0, then our tree has depth
	if (_data.size() > _level) {

		// insert the children pointers
		_children.clear();
		_childrenStatus.clear();
		_isResultCached.clear();
		_cachedResult.clear();
		_children.resize(_data[_level].size(), NULL);
		_childrenStatus.resize(_data[_level].size(), Unused);
		_isResultCached.resize(_data[_level].size(), false);
		_cachedResult.resize(_data[_level].size());

		// create the leftmost child
		_children[0] = new InternalNode(_data[_level][0], _data, _level+1);
		_childrenStatus[0] = Active;

		// now left build the leftmost child
		_children[0]->initLeftBuildout(debug);
	}
}

int sst::Node::getNumViableChildren() const {
	int count = 0;
	for (unsigned int i=0; i<_childrenStatus.size(); ++i)
		if ((_childrenStatus[i] == Unused) || (_childrenStatus[i] == Active))
			++count;
	return count;
}

int sst::Node::getTotalCount() const {
	int count = 1;
	for (unsigned int i=0; i<_data.size(); ++i)
		count *= _data[i].size();
	return count;
}

///
/// The return vector contains the path in reverse order.  
/// The indexes are for the bottom node to the top node.
///
pair<int, vector<int> > sst::Node::getSumOfLargestSubtree(bool debug) const {

	if (debug)
		cout << "Entered sst::Node::getSumOfLargestSubtree()" << endl;

	pair<int, vector<int> > best;
	best.first = std::numeric_limits<int>::min();

	for (unsigned int i=0; i<_children.size(); ++i) {
		if (Active == _childrenStatus[i]) {
			assert(_children[i] != NULL);

			// first pull out what we have in cache
			pair<int, vector<int> >& candidate = _cachedResult[i];

			// if we find out that we really don't have a value cached, 
			// then we need to calculate it
			if (false == _isResultCached[i]) {
				if (debug)
					cout << "CACHE_REPORT: sst::Node : Calculating a value for " << i << endl;
				candidate = _children[i]->getSumOfLargestSubtree(debug);
				_isResultCached[i] = true;
				_cachedResult[i] = candidate;
			} else {
				if (debug)
					cout << "CACHE_REPORT: sst::Node : Using a cached value for " << i << endl;
			}

			if (debug)
				cout << "BIG sum of [" << i << "] = " << candidate.first << endl;
			if (candidate.first >= best.first) {
				best = candidate;
				best.second.push_back(i);
			}
		}
	}

	if (debug) {
		cout << "largest = " << best.first << endl;
		cout << "indexOfLargest = " << best.second[best.second.size()-1] << endl;
	}

	if (debug)
		cout << "Leaving sst::Node::getSumOfLargestSubtree()" << endl;

	return best;
}

void sst::Node::removeAndBuildRight(vector<int>& path, bool debug) {

	if (debug)
		cout << "Entering sst::Node::removeAndBuildRight()" << endl;

	const int currPathIdx = path.size()-1;
	const int currChildIdx = path[currPathIdx];

	if (debug) {
		cout << "currPathIdx = " << currPathIdx << endl;
		cout << "currChildIdx = " << currChildIdx << endl;
	}

	// remove the last element in the path
	path.pop_back();

	// check to see if we're in a leaf.  (In this case both the path size
	// will be 1 and there will be no children. This is a bit pedantic, but
	// better to be safe than sorry.)  
	if ((currPathIdx == 0) && (_childrenStatus[currChildIdx] == Active) && (_children[currChildIdx]->getNumChildren()
			== 0)) {

		if (debug) {
			cout << "NOTE: Entering base case" << endl;
			cout << "Deleting child " << currChildIdx << endl;
		}

		// delete the node
		delete _children[currChildIdx];
		_children[currChildIdx] = NULL;
		_childrenStatus[currChildIdx] = Used;

		// check to see if we need to add another node
		if (((currChildIdx+1) < _children.size()) && (_childrenStatus[currChildIdx+1] == Unused)) {

			if (debug)
				cout << "Adding another node" << endl;

			if (debug)
				cout << "Adding a node with value " << _data[_level][currChildIdx+1] << endl;

			// add a new node (right)
			_children[currChildIdx+1] = new InternalNode(_data[_level][currChildIdx+1], _data, _level+1);
			_childrenStatus[currChildIdx+1] = Active;

			// build out this new node (left)
			if (debug)
				cout << "Calling initLeftBuildout() on index " << (currChildIdx+1) << endl;
			_children[currChildIdx+1]->initLeftBuildout(debug);

			if (debug) {
				cout << "NOTE: Leaving base case" << endl;
			}
		}
	} else {

		if (debug)
			cout << "NOTE: Entering recursive case" << endl;

		// do the recursion, to go deeper in the path first
		_children[currChildIdx]->removeAndBuildRight(path, debug);

		// this value should no longer be cached
		_isResultCached[currChildIdx] = false;
		_cachedResult[currChildIdx] = pair<int, vector<int> >();

		// now, check to see if this child is still viable. 
		// if not, then remove it.
		if (debug)
			cout << "Number of viable children for " << currChildIdx << " = "
					<< _children[currChildIdx]->getNumViableChildren() << endl;
		if (0 == _children[currChildIdx]->getNumViableChildren()) {
			if (debug) {
				cout << "Deleting from child index " << currChildIdx << endl;
				_children[currChildIdx]->print();
			}
			delete _children[currChildIdx];
			_children[currChildIdx] = NULL;
			_childrenStatus[currChildIdx] = Used;
		}

		// check to see if we need to add another node
		if (debug)
			cout << "(currChildIdx+1) < _children.size() : (" << (currChildIdx+1) << " < " << _children.size() << ")"
					<< endl;
		if (((currChildIdx+1) < _children.size()) && (_childrenStatus[currChildIdx+1] == Unused) && (NULL
				== _children[currChildIdx+1])) {

			if (debug)
				cout << "Adding node at " << (currChildIdx+1) << " with a value of " << _data[_level][currChildIdx+1]
						<< endl;

			// add a new node (right)
			_children[currChildIdx+1] = new InternalNode(_data[_level][currChildIdx+1], _data, _level+1);
			_childrenStatus[currChildIdx+1] = Active;

			// build out this new node (left)
			if (debug) {
				cout << "Calling initLeftBuildout() on index " << (currChildIdx+1) << endl;
				cout << "Pre:" << endl;
				_children[currChildIdx+1]->print();
			}
			_children[currChildIdx+1]->initLeftBuildout(debug);
			if (debug) {
				cout << "Post:" << endl;
				_children[currChildIdx+1]->print();
			}

		}

		if (debug)
			cout << "NOTE: Returning from recursive case" << endl;
	}
	if (debug)
		cout << "Leaving sst::Node::removeAndBuildRight()" << endl;

}

void sst::Node::print(unsigned int indentation) const {
	cout << endl;
	for (unsigned int i=0; i<_children.size(); ++i)
		if (NULL != _children[i])
			_children[i]->print();
	cout << endl;
}

unsigned int sst::Node::getNumInternalNodes() const {

	unsigned int nodesFromKids = 0;
	bool hasKids = false;

	for (unsigned int i=0; i<_children.size(); ++i)
		if (_children[i] != NULL) {

			hasKids = true;
			nodesFromKids += _children[i]->getNumInternalNodes();
		}

	if (false == hasKids)
		return 1;
	else
		return 1 + nodesFromKids;
}

unsigned int sst::Node::getNumLeavesInSubtree() const {

	bool hasKids = false;

	for (unsigned int i=0; i<_children.size(); ++i) 
		if (_children[i] != NULL) 
			hasKids = true;
		
	
	if(hasKids == true) {

		unsigned int totalLeaves = 0;
		
		for (unsigned int i=0; i<_children.size(); ++i)
			if (_children[i] != NULL) 
				totalLeaves += _children[i]->getNumLeavesInSubtree();
		return totalLeaves;
		
	}
	else 
		return 1;
}
