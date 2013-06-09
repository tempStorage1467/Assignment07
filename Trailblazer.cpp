/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Eric Beach
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "random.h"
#include "UnionFind.h"
#include "set.h"

using namespace std;

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc>
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc from, Loc to, Grid<double>& world),
             double heuristic(Loc start, Loc end, Grid<double>& world)) {
    ////////// SETUP CODE //////////
    // store the parent cells for each specific location
    Grid<Loc> parentCell(world.numRows(), world.numCols());
        
    // stores the total cost in getting from the start node to any
    //   given cell; remember that this is a cumulative cost
    Grid<double> nodeCosts(world.numRows(), world.numCols());
    
    // priority queue to store locations to examine and their associated costs
    TrailblazerPQueue<Loc> locsToExamine;
        
    ////////// FOLLOWING PSEUDOCODE //////////
    // this grid is also used to store which cells are in the priority queue
    //   as a cell that is in the priority queue will be yellow while a cell
    //   that has been visited will be green
    Grid<Color> cellColors(world.numRows(), world.numCols());
    
    // color the start node yellow; marks node is in PQueue
    cellColors[start.row][start.col] = YELLOW;
    colorCell(world, start, YELLOW);
    
    // set startNode's candidate distance to 0 (or h(startNode, endNode)).
    nodeCosts[start.row][start.col] = heuristic(start, end, world);

    // Enqueue startNode into the priority queue with priority 0.
    locsToExamine.enqueue(start, 0);
    
    // While not all nodes have been visited
    while (true) {
        // Dequeue the lowest-cost node curr from the priority queue.
        Loc curr = locsToExamine.dequeueMin();
        
        // Color curr green. (The candidate distance dist that is currently
        //   stored for node curr is the length of the shortest path from
        //   startNode to curr.)
        cellColors[curr.row][curr.col] = GREEN;
        colorCell(world, curr, GREEN);
        
        // If curr is the destination node endNode, you have found the
        //   shortest path from startNode to endNode
        if (curr == end) break;
        
        // For each node v connected to curr by an edge of length L:
        for (int row = curr.row - 1; row < curr.row + 2; row++) {
            for (int col = curr.col - 1; col < curr.col + 2; col++) {
                //if (row == curr.row && col == curr.col) continue;
                if (row < 0 || row >= world.numRows() ||
                    col < 0 || col >= world.numCols()) continue;
                                
                // set v, the candidate cell, to be a location object
                Loc v = makeLoc(row, col);
                
                // cost to get to candidate cell, v, is the total cost to get
                //   to the current cell plus the incremental cost to get
                //   to the adjacent neighbor cell
                // = dist + L in pseudocode
                double vPathCost = nodeCosts[curr.row][curr.col] +
                                   costFn(curr, v, world);
                
                
                // If v is gray: (a) Color v yellow.
                //   (b) Set v's candidate distance to be dist + L.
                //   (c) Set v's parent to be curr.
                //   (d) Enqueue v into the priority queue with priority dist + L.
                if (cellColors[v.row][v.col] == GRAY) {
                    cellColors[v.row][v.col] = YELLOW;
                    colorCell(world, v, YELLOW);
                    
                    nodeCosts[row][col] = vPathCost;
                    parentCell[row][col] = curr;
                    locsToExamine.enqueue(v, vPathCost + heuristic(v, end, world));
                }
                // Otherwise, if v is yellow and the candidate distance to v is greater than dist + L:
                //   (a) Set v's candidate distance to be dist + L.
                //   (b) Set v's parent to be curr.
                //   (c) Update v's priority in the priority queue to dist + L.
                else if (cellColors[v.row][v.col] == YELLOW &&
                           nodeCosts[v.row][v.col] > vPathCost) {
                    nodeCosts[v.row][v.col] = vPathCost;
                    parentCell[v.row][v.col] = curr;
                    locsToExamine.decreaseKey(v, vPathCost + heuristic(v, end, world));
                }
            }
        }
    }
    
    // found end node; trace back parent cell for each cell in the path
    //   and add each path cell to the output vector
    // shortest (or cheapest) path
    Vector<Loc> tempReversePath;
    Loc curr = end;
    while (curr != start) {
        tempReversePath += curr;
        curr = parentCell[curr.row][curr.col];
    }
    tempReversePath+= start;
    
    // the vector that is returned with the path should have the
    //   start location at element 0, not at element size() - 1
    Vector<Loc> finalPath;
    for (int i = tempReversePath.size() - 1; i >=0; i--) {
        finalPath += tempReversePath[i];
    }
    return finalPath;
}

Set<Edge> createMaze(int numRows, int numCols) {
//    numCols = 5;
//    numRows = 4;
    
    // STEP 1: Construct the edges and place them into a data structure
    Set<Edge> edges;
    //         Step 1A: Construct Horizontal Edges
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols - 1; col++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row, col + 1);
            Edge ab = makeEdge(a, b);
            
           // cout << "(" << row << ", " << col << ") to (" << row << ", " << (col + 1) << ")" << endl;
            
            edges.add(ab);
        }
    }
    
    //         Step 1B: Construct Vertical Edges
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows - 1; row++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row + 1, col);
            Edge ab = makeEdge(a, b);
            
           // cout << "(" << row << ", " << col << ") to (" << (row + 1) << ", " << col << ")" << endl;
            
            edges.add(ab);
        }
    }
    
    // STEP 2: Place all edges into priority queue with random weight/length/cost
    TrailblazerPQueue<Edge> pQueue;
    
    // Step 2-B: Create a UnionFind, used to determine if an edge is from
    //   the same cluster
    UnionFind clusters(numRows * numCols, numCols);
    
    // Place the edges into the PriorityQueue and the UnionFind
    foreach (Edge next in edges) {
        clusters.makeSet(next.start);
        clusters.makeSet(next.end);
        pQueue.enqueue(next, randomReal(0, 100));
    }
    
    // Pre Step 3: Setup a data structure to store the result of the edges
    Set<Edge> result;
    // STEP 3: While the priority queue is not empty:
    while (!pQueue.isEmpty()) {
        //         Step 3A: Dequeue an edge from the priority queue.
        Edge next = pQueue.dequeueMin();
        
        //         Step 3B: If the endpoints of the edge aren't already connected
        //                  to one another, add in that edge.
        if (clusters.find(next.start) != clusters.find(next.end)) {
            // add the next edge to the final set of edges
            result += next;
            
            // join the two locations into the same cluster
            clusters.join(next.start, next.end);
        }
        //         Step 3C: Otherwise, skip the edge.
    }
    return result;
}
