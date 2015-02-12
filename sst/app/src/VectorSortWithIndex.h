#ifndef _VECTOR_SORT_WITH_INDEX_H_
#define _VECTOR_SORT_WITH_INDEX_H_

#include <vector>
#include <utility>
using namespace std;

template <class T> class VectorSortWithIndex {

public:
	VectorSortWithIndex(vector<T> vec) {

		// load the data
		for (unsigned int i=0; i<vec.size(); ++i)
			_data.push_back(pair<unsigned int, T>(i, vec[i]));

		// sort the data
		sort(_data.begin(), _data.end(), lessThanComp);
	}

	const T& getDataForKthSorted(unsigned int k) const {
		return _data[k].second;
	}

	unsigned int getOriginalIndexForKthSorted(unsigned int k) const {
		return _data[k].first;
	}
	
	vector<T> getData() const {
		vector<T> tmp;
		for(unsigned int i=0; i<_data.size(); ++i)
			tmp.push_back(_data[i].second);
		return tmp;
	}
	
	unsigned int size() const {
		return _data.size();
	}

private:
	vector<pair<unsigned int, T> > _data;

	static bool lessThanComp(const pair<unsigned int, T>& a, const pair<unsigned int, T>& b) {
		return a.second < b.second;
	}

};

#endif
