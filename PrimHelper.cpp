/******************************************************************************
 * File: PrimHelper.cpp
 *
 * Eric Beach
 *
 * Implementation of Prim's algorithm to generate a MST for use in a maze.
 *
 * Credits:
 *  http://www.youtube.com/watch?v=BtGuZ-rrUeY
 *  http://www.youtube.com/watch?v=cplfcGZmX7I
 *  http://www.youtube.com/watch?v=YyLaRffCdk4
 *  http://en.wikipedia.org/wiki/Prim's_algorithm
 */

#include "PrimHelper.h"

/*
 * Constructor for PrimHelper, which receives the number of rows and the number
 *   of columns in the grid that is used to represent the maze.
 */
PrimHelper::PrimHelper(int nRows, int nCols) {
    clusters = new UnionFind(nRows * nCols, nCols);
    numRows = nRows;
    numCols = nCols;
}

/*
 * Add an edge that is present in the graph and its corresponding weight/cost.
 */
void PrimHelper::add(Edge& edge, double weight) {
    edges.put(edge, weight);
    clusters->makeSet(edge.start);
    clusters->makeSet(edge.end);
}

/*
 * This function represents the heart of Prim's algorithm.
 *
 * Take in a Set of all the locations (i.e., nodes) that have been visited
 *   (i.e., marked, included) as part of the MST.
 * For each of those nodes that have been visited (i.e., included in MST), find
 *   all the neighbor locations that could form a new edge in the graph and
 *   enqueue that possible edge with its weight.
 * Dequeue the minimum weight edge that connects the established graph
 *   to a node not presently in the established graph.
 * Update the established graph to include the new node and then return the
 *   selected edge.
 */
Edge PrimHelper::getNextEdge(Set<Loc>& visited) {
    // create a priority queue with the proper weight to store
    //   the possible edges that could be the next edge in the MST
    TrailblazerPQueue<Edge> possibleEdges;
    
    foreach (Loc curr in visited) {
        // add +-1 on each side of the current location and construct an edge
        // enqueue edges with weight from map
        for (int row = curr.row - 1; row < curr.row + 2; row++) {
            for (int col = curr.col - 1; col < curr.col + 2; col++) {
                // eliminate out of bounds edges
                if (row < 0 || row >= numRows ||
                    col < 0 || col >= numCols) continue;
                
                // eliminate the current point's edges (just a point)
                if (curr == makeLoc(row, col)) continue;
                
                // eliminate diagonal edges, which don't exist in graph
                if (row != curr.row && col != curr.col) continue;
                Loc neighborLoc = makeLoc(row, col);
                
                // elminate edges to locations already visited
                if (visited.contains(makeLoc(row, col))) continue;
            
                // construct the edge from curr-> row,col and
                //   from row,col -> curr
                // it is necessary to construct the edge both ways
                //   as we use the $edges Map with the edge as the key;
                //   this requires that we construct the edge both ways
                //   as the .containsKey function will miss an edge
                //   that is a->b if it is searching for b->a
                Edge neighborEdgeA = makeEdge(curr, neighborLoc);
                Edge neighborEdgeB = makeEdge(neighborLoc, curr);
            
                double neighborEdgeWeight;
                // find the right edge and enqueue it in the priority queue
                //   with the proper weight (i.e., cost)
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
        // if no edges left to examine from all the locations we've
        //   visited so far, throw an error
        // Style Note: it would be better to simply have an exit code
        //   or some other more graceful way to signal that we're done
        //   as an exception is quite severe
        if (possibleEdges.isEmpty()) throw ErrorException("No edges left");
        
        // pull out lowest weight neighbor edge from all the locations
        //   we've visited thus far
        Edge nextEdge = possibleEdges.dequeueMin();
        
        // find unconnected Loc (i.e., find Loc that is a part of the Edge
        //   but is not a part of the MST yet)
        if (visited.contains(nextEdge.start)) {
            visited += nextEdge.end;
        } else {
            visited += nextEdge.start;
        }
        
        // check for cycles (i.e., to create a MST, we cannot add an
        //   edge that creates a cycle)
        if (clusters->find(nextEdge.start) != clusters->find(nextEdge.end)) {
            // found a valid edge; update the set/cluster information
            //   to merge edges to be into same set so we can
            //   properly detect cycles in the future
            
            clusters->join(nextEdge.start, nextEdge.end);
            return nextEdge;
        }
    }
}
