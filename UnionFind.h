/******************************************************************************
 * File: UnionFind.h
 *
 * Eric Beach
 *
 * Assignment 7 -- Extension
 * Implementation of a UnionFind data structure
 *   (i.e., Disjoint-set data structure)
 * http://en.wikipedia.org/wiki/Disjoint-set_data_structure
 */

#ifndef __Trailblazer__UnionFind__
#define __Trailblazer__UnionFind__

#include <iostream>
#include "TrailblazerTypes.h"
#include "TrailblazerGraphics.h"

/*
 * Create a Disjoint-set data structure (i.e., Union/Find data structure).
 * This class is important for both Prim's algorithm as well as Kruskal's
 *   algorithm. Its main two functions are:
 *   (1) find - determine which set the the specific location is in.
 *   (2) join - join two sets together by making Set B a child of Set A.
 * The key to this class is as follows: nodes with the same
 *   parent belong to the same set
 * In other words: if two locations are passed into find and they
 *   both return the same root, then they are in the same set
 */
class UnionFind {
 public:
    // the maximum set number that this class should store
    // even if this class will only store two values, node 3 and node 56,
    //   this value needs to be 53
    // the number of columns in the grid is necessary information
    //   to translate between Loc objects and index numbers in the
    //   underlying array UnionFind uses
    UnionFind(const int maxSetNum, const int numCols);
    
    // clean up class by deallocating used memory
    ~UnionFind();
    
    // Return the root location for a given location (i.e., return the
    //   cluster that it is apart of)
    Loc find(const Loc& toFind);
    
    // Join two subsets into one
    void join(const Loc& a, const Loc& b);
    
    // make a set (i.e., add a new element as a singleton set)
    void makeSet(const Loc& input);

private:
    // array to store the parent nodes for each node
    int* nodeParents;
    
    // the maximum set number that this class should store
    // even if this class will only store two values, node 3 and node 56,
    //   this value needs to be 53
    int MAX_SET_NUM;
    
    // the number of columns in the maze; this is necessary information
    //   to translate between Loc objects and index numbers in nodeParents
    int NUM_COLS;
    
    // Take a location and find the index in the node array
    int locToNodeNum(const Loc& input);
    
    // take a node number (i.e., the index in nodeParents) and
    //   construct the corresponding Loc object
    Loc nodeNumToLoc(const int nodeNum);
    
    // find the node number that is the parent (or representative set number)
    //   for a given node number (i.e., determine which set a node number
    //   belongs to)
    int find(const int nodeNum);
};

#endif /* defined(__Trailblazer__UnionFind__) */
