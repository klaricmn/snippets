#ifndef _INTERNAL_NODE_H_
#define _INTERNAL_NODE_H_

#include <vector>
using namespace std;

#include "Node.h"

namespace sst {
	class InternalNode : public sst::Node {

public:
		InternalNode(int nodeValue, const vector< vector< int> >& data, int level);

		virtual ~InternalNode();

		inline int getNumChildren() const {
			return _children.size();
		}

		virtual pair<int, vector<int> > getSumOfLargestSubtree(bool debug=false) const;

		virtual void removeAndBuildRight(vector<int>& path, bool debug=false) {
			sst::Node::removeAndBuildRight(path, debug);
		}

		virtual void print(unsigned int indentation=0) const;

private:

		int _nodeValue;

	};
}
;

#endif
