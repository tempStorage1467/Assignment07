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
Edge PrimHelper::getNextEdge(Set<Loc>& visited) {
    TrailblazerPQueue<Edge> possibleEdges;
    
    foreach (Loc curr in visited) {
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
                
                // elminate edges already entirely visited
                if (visited.contains(makeLoc(row, col))) continue;
            
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
    }
    
    while (true) {
        // if no edges left, throw an error
        if (possibleEdges.isEmpty()) throw ErrorException("No edges left");
        
        // pull out lowest weight neighbor edge from current location
        Edge nextEdge = possibleEdges.dequeueMin();
        
        // find unconnected Loc (i.e., find Loc that is a part of the Edge
        //   but is not a part of the MST yet)
        if (visited.contains(nextEdge.start)) {
            visited += nextEdge.end;
        } else {
            visited += nextEdge.start;
        }
        
        // check for cycles
        if (clusters->find(nextEdge.start) != clusters->find(nextEdge.end)) {
            // found a valid edge; update the set/cluster information
            // merge edges to be into same set
            
            clusters->join(nextEdge.start, nextEdge.end);
            return nextEdge;
        }
    }
}
