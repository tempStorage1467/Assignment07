/******************************************************************************
 * File: PrimHelper.h
 *
 * Eric Beach
 *
 * Prototype of Prim's algorithm to generate a MST for use in a maze.
 *
 * Credits:
 *  http://www.youtube.com/watch?v=BtGuZ-rrUeY
 *  http://www.youtube.com/watch?v=cplfcGZmX7I
 *  http://www.youtube.com/watch?v=YyLaRffCdk4
 *  http://en.wikipedia.org/wiki/Prim's_algorithm
 */

#ifndef __Trailblazer__PrimHelper__
#define __Trailblazer__PrimHelper__

#include <iostream>
#include "map.h"
#include "set.h"
#include "UnionFind.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"

/*
 * Class to perform the bulk of the work for Prim's Algorithm, which is used
 *   as an alternative to Kruskal's Algorithm to create a MST which
 *   provides a maze.
 * PrimHelper receives the size of the grid in its constructor and later
 *   receives each location on that grid.
 * The main method in PrimHelper is getNextEdge() which receives all locations
 *   that have been visited (i.e., are a part of the MST) and returns a new
 *   edge to be a part of the MST.
 */
class PrimHelper {
public:
    // constructor; takes the number of rows and number of columns in the
    //   graph
    PrimHelper(int numRows, int numCols);
    
    // add Locations at both ends of the edges into the underlying array
    void add(Edge& edge, double weight);
    
    /*
     * Construct four edges adjacent to a given location
     *    Need to have function that finds all edge cordinates that touch a location
     * Put all four edges into a priority queue
     * Pull out lowest edge and see if the two ends are a part of the same set
     */
    Edge getNextEdge(Set<Loc>& visited);
    
private:
    // store the clusters that each location is a part of; this is
    //   necessary for ensuring that each new node in the MST does not create
    //   a cycle
    UnionFind* clusters;
    
    // map between each edge and its weight (i.e., cost)
    Map<Edge, double> edges;
    
    // number of rows in the grid
    int numRows;
    
    // number of columns in the grid
    int numCols;
};

#endif /* defined(__Trailblazer__PrimHelper__) */
