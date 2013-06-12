/******************************************************************************
 * File: UnionFind.cpp
 *
 * Eric Beach
 *
 * Assignment 7 -- Extension
 * Implementation of a UnionFind data structure
 *   (i.e., Disjoint-set data structure)
 * http://en.wikipedia.org/wiki/Disjoint-set_data_structure
 */

#include "UnionFind.h"

using namespace std;

/*
 * A UnionFind (aka Disjoint-set) data structure to store sets, find
 *   which set an element belongs to, and join two sets.
 * 
 */
UnionFind::UnionFind(const int maxSetNum, const int numCols) {
    MAX_SET_NUM = maxSetNum;
    NUM_COLS = numCols;
    nodeParents = new int[MAX_SET_NUM];
}

/*
 * Destructor to cleanup the allocated memory.
 */
UnionFind::~UnionFind() {
    delete[] nodeParents;
}

/*
 * Find the root node number for a particular node.
 * This is used to determine whether two nodes are in the same set (i.e.,
 *   if two nodes have the same root, they are in the same set)
 */
Loc UnionFind::find(const Loc& toFind) {
    int nodeNum = locToNodeNum(toFind);
    if (nodeNum >= MAX_SET_NUM) error("Array out of bounds");
    int root = find(nodeNum);
    return nodeNumToLoc(root);
}

/*
 * Join node b to node a such that node a will now become the parent of node b.
 */
void UnionFind::join(const Loc& a, const Loc& b) {
    int aRootNodeNum = find(locToNodeNum(a));
    int bRootNodeNum = find(locToNodeNum(b));
    nodeParents[bRootNodeNum] = aRootNodeNum;
}

/*
 * Add a new Location object into the data structure as a singleton
 *   Location (i.e., as its own single element set).
 */
void UnionFind::makeSet(const Loc& input) {
    int nodeNum = locToNodeNum(input);
    if (nodeNum >= MAX_SET_NUM) error("Array out of bounds");
    nodeParents[nodeNum] = nodeNum;
}

////////// PRIVATE METHODS //////////
/*
 * Find the root node number of a particular node.
 * This is used to determine whether two nodes are in the same set (i.e.,
 *   if two nodes have the same root, they are in the same set)
 * $nodeNum is the key value in $nodeParents for a given particular node.
 */
int UnionFind::find(const int nodeNum) {
    // if the node toFind is the parent of itself, we have found the
    //   root location (i.e., we have found the parent node number)
    if (nodeParents[nodeNum] == nodeNum) {
        return nodeNum;
    }
    // the node toFind has a parent Location that is not itself, so
    //   traverse back to find the Location representative of the set
    //   that toFind is a part of
    else {
        // find the immediate parent of nodeNum and then recursively
        //   find its parent
        int parent = find(nodeParents[nodeNum]);
        
        // store the result of the resursive call, which will contain
        //   the root for nodeNum that represents the set that
        //   nodeNum is a part of
        nodeParents[nodeNum] = parent;
        return parent;
    }
}

/*
 * Convert a Location object to the node number, which can be used
 *   in performing lookups in the nodeParents array.
 */
int UnionFind::locToNodeNum(const Loc& input) {
    return input.col + (input.row * NUM_COLS);
}

/*
 * Convert a node number, which is the key of the node stored in the nodeParents
 *   object, which can be used in performing lookups in the nodeParents array.
 */
Loc UnionFind::nodeNumToLoc(const int nodeNum) {
    int colNum = nodeNum % NUM_COLS;
    int rowNum = nodeNum / NUM_COLS;
    return makeLoc(rowNum, colNum);
}
