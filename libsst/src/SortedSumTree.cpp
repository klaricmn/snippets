#include "SortedSumTree.h"

#include "Statistics.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ext/numeric>
using namespace std;

sst::SortedSumTree::SortedSumTree(vector< vector< int> > data, bool debug) :
	_data(data) {

	// initialize the mapping with the right number of rows
	_mapOfNewColIdxsToOrigColIdxs = vector< vector <int> >(data.size());
	
	// reorganize each row so that it's sorted
	for (unsigned int i=0; i<_data.size(); ++i) {
		//cout << endl << "** Row " << i << endl;
		
		// initialize this row with the right number of elements
		_mapOfNewColIdxsToOrigColIdxs[i] = vector<int>(_data[i].size());

		// create a temporary array so that we can sort the data
		// and keep track of the original indexes 
		vector< pair<int, int> > tmpArr(_data[i].size());
		for(unsigned int j=0; j<_data[i].size(); ++j) 
			tmpArr[j] = pair<int, int>(_data[i][j], j);
	
		// sort the data
		sort(tmpArr.begin(), tmpArr.end(), greater< pair<int,int> >());

		// now store the mapping of the new row indexes back to the originals
		// and also put the sorted data into the data array
		for(unsigned int j=0; j<_data[i].size(); ++j) {
			_data[i][j] = tmpArr[j].first;
			_mapOfNewColIdxsToOrigColIdxs[i][j] = tmpArr[j].second;

			//cout << "\t New idx " << j << " was orig idx " << tmpArr[j].second << endl;
			
		}	
	}

	// reorganize the rows based on our magic score
	reorgRows();

	_rootNode = new Node(_data, 0);
	_rootNode->initLeftBuildout(debug);
}

sst::SortedSumTree::~SortedSumTree() {
	delete _rootNode;
}

bool sst::SortedSumTree::hasMore() const {
	if (_rootNode != NULL)
		return _rootNode->getNumViableChildren() > 0;
	else
		return false;
}

int sst::SortedSumTree::getTotalCount() const {
	if (_rootNode != NULL)
		return _rootNode->getTotalCount();
	else
		return -1;
}

pair<int,vector<int> > sst::SortedSumTree::getSumOfLargestSubtree(bool debug) const {

	pair<int, vector<int> > res;
	res = _rootNode->getSumOfLargestSubtree(debug);

	vector<int> removePath;
	removePath = res.second;

	_rootNode->removeAndBuildRight(removePath, debug);

	// this is a vector of original row indexes and original column indexes.  
	vector<pair<int, int> > tmp(res.second.size());
	
	for(unsigned int i=0; i<res.second.size(); ++i) {
		
		// the numbers in the path get tacked on in reverse order, 
		// so we need to reverse them when processing.  
		const int newRowIdx = (res.second.size()-i-1);
		//cout << " newRowIdx = " << newRowIdx << endl;
		
		const int newColIdx = res.second[newRowIdx];
		//cout << "\t newColIdx = " << newColIdx << endl;
		
		const int origRowIdx = _mapOfOrigAndNewRowIdxs.right.at(i);
		//cout << "\t origRowIdx = " << origRowIdx << endl;

		const int origColIdx = _mapOfNewColIdxsToOrigColIdxs[origRowIdx][newColIdx];
		//cout << "\t origColIdx = " << origColIdx << endl;
	
		tmp[i] = pair<int,int>(origRowIdx, origColIdx);
	}
	
	sort(tmp.begin(), tmp.end());
	
	for(unsigned int i=0; i<res.second.size(); ++i) {	
		res.second[i] = tmp[i].second;
	}
	
	return res;
}

void sst::SortedSumTree::print() const {
	if (_rootNode != NULL)
		_rootNode->print();
}

unsigned int sst::SortedSumTree::getNumInternalNodes() const {
	return _rootNode->getNumInternalNodes() - 1;
}

unsigned int sst::SortedSumTree::getNumLeaves() const {
	return _rootNode->getNumLeavesInSubtree();
}

/*
template< class MapType >
void print_map(const MapType & m)
{
    typedef typename MapType::const_iterator const_iterator;
    for( const_iterator iter = m.begin(), iend = m.end(); iter != iend; ++iter )
    {
        std::cout << iter->first << "-->" << iter->second << std::endl;
    }
}
*/

/* ****************************************************************************** */
/* ****************************************************************************** */
void sst::SortedSumTree::reorgUsingSpread() {

	vector< vector< int> > tmp;
	vector<int> origIndexList(_data.size());
	iota(origIndexList.begin(), origIndexList.end(), 0);
	
	while (_data.size() > 0) {

		vector<double> variances = sst::statistics::calcNormalizedVariances(_data);
		vector<double> coverages = sst::statistics::calcRangeCoverages(_data);
		vector<double> score = sst::statistics::sumOfLists(variances, coverages);

		int elemOfLargestScore = distance(score.begin(), max_element(score.begin(), score.end()));

		// store the mapping
		_mapOfOrigAndNewRowIdxs.insert( boost::bimap<int,int>::value_type( origIndexList[elemOfLargestScore], tmp.size() ) );

		// store the data into a temp array that has it in the new order
		tmp.push_back(_data[elemOfLargestScore]);
		
		// remove the data point from the original vector
		_data.erase(_data.begin() + elemOfLargestScore);
				
		// now delete the old index
		origIndexList.erase(origIndexList.begin() + elemOfLargestScore);
	}

	//print_map( _mapOfOrigAndNewRowIdxs.left  );
	
	_data = tmp;
}

/* ****************************************************************************** */
/* ****************************************************************************** */
void sst::SortedSumTree::reorgRows() {

	reorgUsingSpread();
}

/* ****************************************************************************** */
/* ****************************************************************************** */
string sst::SortedSumTree::toString(vector<int> x) {

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

/* ****************************************************************************** */
/* ****************************************************************************** */
string sst::SortedSumTree::toString(vector<vector<int> > data) {

	stringstream ss;
	ss << "(";

	for (unsigned int i=0; i<data.size(); ++i) {
		ss << toString(data[i]);

		if ( (i+1) != data.size() )
			ss << ",";
	}

	ss << ")";
	return ss.str();
}
