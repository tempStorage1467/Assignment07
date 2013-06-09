//
//  UnionFind.cpp
//  Trailblazer
//
//  Created by Eric Beach on 6/8/13.
//
//

#include "UnionFind.h"

using namespace std;

UnionFind::UnionFind(const int maxSetNum, const int numCols) {
    MAX_SET_NUM = maxSetNum;
    NUM_COLS = numCols;
    nodeParents = new int[MAX_SET_NUM];
}

UnionFind::~UnionFind() {
    delete[] nodeParents;
}

int UnionFind::find(const int nodeNum) {
    // if the node toFind is the parent of itself, we have found the
    //   root location
    if (nodeParents[nodeNum] == nodeNum) {
        return nodeNum;
    }
    // the node toFind has a parent Location that is not itself,
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

Loc UnionFind::find(const Loc& toFind) {
    int nodeNum = locToNodeNum(toFind);
    if (nodeNum >= MAX_SET_NUM) error("Array out of bounds");
    int root = find(nodeNum);
    return nodeNumToLoc(root);
}

void UnionFind::join(const Loc& a, const Loc& b) {
    int aRootNodeNum = find(locToNodeNum(a));
    int bRootNodeNum = find(locToNodeNum(b));
    nodeParents[aRootNodeNum] = bRootNodeNum;
}

void UnionFind::makeSet(const Loc& input) {
    int nodeNum = locToNodeNum(input);
    if (nodeNum >= MAX_SET_NUM) error("Array out of bounds");
    nodeParents[nodeNum] = nodeNum;
}

int UnionFind::locToNodeNum(const Loc& input) {
    return input.col + (input.row * NUM_COLS);
}

Loc UnionFind::nodeNumToLoc(const int nodeNum) {
    int colNum = nodeNum % NUM_COLS;
    int rowNum = nodeNum / NUM_COLS;
    return makeLoc(rowNum, colNum);
}
