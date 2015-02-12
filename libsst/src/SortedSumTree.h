#ifndef _SORTED_SUM_TREE_H_
#define _SORTED_SUM_TREE_H_

#include <vector>
#include <utility>
using namespace std;

#include <boost/bimap.hpp>

#include "Node.h"

namespace sst {

	///
	/// This class calculates the sums numbers from lists.  It selects one number
	/// from each list and sums them.  It will return the sums in order--from
	/// largest to smallest.  
	///
	class SortedSumTree {

public:

		///
		/// Constructor for the class.  
		///	\param data The lists of numbers
		/// \param debug Enables debug mode which prints verbose output.  
		///
		SortedSumTree(vector< vector< int> > data, bool debug=false);

		///
		/// Destructor for the class
		///
		~SortedSumTree();

		///
		/// This method returns the next largest sum as well as it's path 
		/// through the tree.  
		/// \param debug Enables debug mode which prints verbose output. 
		/// \return A pair containing the sum and the indices used to generate the sum.  
		///
		pair<int,vector<int> > getSumOfLargestSubtree(bool debug=false) const;

		///
		/// A method which can be used to determine if there are more sums
		/// left to generate.  Use this as a stopping criteria if you'd like 
		/// to loop through all sums.  
		/// \return Boolean value indicating if there are more sums to generate.
		///
		bool hasMore() const;

		///
		/// Get the total number of sums that are to be generated for this data.  
		/// \return Total number of sums.  
		///
		int getTotalCount() const;

		///
		/// Get the data that was stored in the object.  
		/// \return A reference to the data.  
		///
		inline const vector< vector< int> > & getData() const {
			return _data;
		}

		///
		/// Get the number of internal nodes that currently exist in the tree.
		/// \return Number of internal nodes
		///
		unsigned int getNumInternalNodes() const;

		///
		/// Get the number of leaves that currently exist in the tree.
		/// \return Number of leaves.  
		///
		unsigned int getNumLeaves() const;

		///
		/// Print the tree for debugging purposes.  
		///
		void print() const;

private:

		///
		/// Root node of the tree
		///
		Node* _rootNode;

		///
		/// A copy of the data being processed
		///
		vector< vector< int> > _data;

		///
		/// A bimap of original and new row indexes
		///
		boost::bimap<int,int> _mapOfOrigAndNewRowIdxs;

		///
		/// A map of new col indexes to the original col indexes.
		/// The rows here are indexed using the original row indexes.
		///
		vector< vector<int> > _mapOfNewColIdxsToOrigColIdxs;
		
		///
		/// A method to reorganize the data based on properties known to produce a good ordering.  
		/// This is a combination of the coverage of the population data and the variance.  
		///
		void reorgUsingSpread();

		///
		/// A method to reorganize the data of the object.  This method should be called prior to loading 
		/// the data and building the tree.  
		/// \sa reorgUsingSpread
		///
		void reorgRows();

		///
		/// Helper method used for printing.
		///
		static string toString(vector<int> x);

		///
		/// Helper method used for printing.  
		/// 
		static string toString(vector<vector<int> > data);
	};
}

#endif
