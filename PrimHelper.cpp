//
//  PrimHelper.cpp
//  Trailblazer
//
//  Created by Eric Beach on 6/9/13.
//
//

#include "PrimHelper.h"

PrimHelper::PrimHelper(int nRows, int nCols) {
    clusters = new UnionFind(nRows * nCols, nCols);
    numRows = nRows;
    numCols = nCols;
}

void PrimHelper::add(Edge& edge, double weight) {
    edges.put(edge, weight);
    clusters->makeSet(edge.start);
    clusters->makeSet(edge.end);
}

// need to input a vector of Loc's to consider, not just one
// keep a list of Visited Loc's... then compute all Loc's connected to our
//   visited Loc's... choose the lowest weight
Edge PrimHelper::getSmallestEligibleNeighborEdge(Loc curr) {
    TrailblazerPQueue<Edge> possibleEdges;
    
    // add +-1 on each side of the current location and construct an edge
    // enqueue edges with weight from map
    for (int row = curr.row - 1; row < curr.row + 2; row++) {
        for (int col = curr.col - 1; col < curr.col + 2; col++) {
            // eliminate out of bounds edges
            if (row < 0 || row >= numRows ||
                col < 0 || col >= numCols) continue;
            // eliminate point edges
            if (curr == makeLoc(row, col)) continue;
            // eliminate diagonal edges
            if (row != curr.row && col != curr.col) continue;
            Loc neighborLoc = makeLoc(row, col);
            Edge neighborEdgeA = makeEdge(curr, neighborLoc);
            Edge neighborEdgeB = makeEdge(neighborLoc, curr);
            
            double neighborEdgeWeight;
            if (edges.containsKey(neighborEdgeA)) {
                neighborEdgeWeight = edges.get(neighborEdgeA);
                possibleEdges.enqueue(neighborEdgeA, neighborEdgeWeight);
            } else {
                neighborEdgeWeight = edges.get(neighborEdgeB);
                possibleEdges.enqueue(neighborEdgeB, neighborEdgeWeight);
            }
        }
    }

    if (possibleEdges.size() < 4) cout << "huge error: "
        << curr.row << ", " << curr.col << endl;
    
    while (true) {
        // if no edges left, throw an error
        if (possibleEdges.isEmpty()) {
            cout << "No Edges Left" << endl;
            throw ErrorException("No edges left");
        }
        
        // pull out lowest weight neighbor edge from current location
        Edge nextEdge = possibleEdges.dequeueMin();
        
        // find location at opposite end of edge from current lcoation
        Loc oppositeEndOfEdge;
        if (nextEdge.start == curr) {
            oppositeEndOfEdge = nextEdge.end;
        } else {
            oppositeEndOfEdge = nextEdge.start;
        }
        
        // check for cycles
        if (clusters->find(curr) != clusters->find(oppositeEndOfEdge)) {
            // found a valid edge; update the set/cluster information
            // merge edges to be into same set
            clusters->join(curr, oppositeEndOfEdge);
            return nextEdge;
        }
    }
}
