//
//  PrimHelper.h
//  Trailblazer
//
//  Created by Eric Beach on 6/9/13.
//
//

#ifndef __Trailblazer__PrimHelper__
#define __Trailblazer__PrimHelper__

#include <iostream>

#include "UnionFind.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "map.h"

class PrimHelper {
public:
    PrimHelper(int numRows, int numCols);
    
    // add Locations at both ends of the edges into the underlying array
    void add(Edge& edge, double weight);
    
    /*
     * Construct four edges adjacent to a given location
     *    Need to have function that finds all edge cordinates that touch a location
     * Put all four edges into a priority queue
     * Pull out lowest edge and see if the two ends are a part of the same set
     */
    Edge getSmallestEligibleNeighborEdge(Loc);
    
private:
    UnionFind* clusters;
    Map<Edge, double> edges;
    int numRows;
    int numCols;
};

#endif /* defined(__Trailblazer__PrimHelper__) */
