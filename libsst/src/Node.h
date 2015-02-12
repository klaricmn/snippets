#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
using namespace std;

namespace sst {

	typedef enum {
		Unused,
		Active,
		Used
	} NodeStatus;

	class Node {

public:
		Node(const vector< vector< int> >& data, int level);

		virtual ~Node();

		virtual void initLeftBuildout(bool debug=false);

		inline int getNumChildren() const {
			return _children.size();
		}

		int getNumViableChildren() const;

		inline bool hasMore() const {
			return getNumViableChildren() > 0;
		}
		
		int getTotalCount() const;
		
		virtual pair<int,vector<int> > getSumOfLargestSubtree(bool debug=false) const;

		virtual void removeAndBuildRight(vector<int>& path, bool debug=false);

		virtual void print(unsigned int indentation=0) const;

		unsigned int getNumInternalNodes() const;
		
		unsigned int getNumLeavesInSubtree() const;
		
protected:
		vector<Node*> _children;
		vector<NodeStatus> _childrenStatus;
		mutable vector<bool> _isResultCached;
		mutable vector< pair<int, vector<int> > > _cachedResult;
		
		const vector< vector< int> >& _data;
		int _level;
	};
}
;

#endif
