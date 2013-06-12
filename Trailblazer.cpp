/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Eric Beach
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 *
 * Extensions:
 *   (1) Union-set data structure to store
 *   (2) Maze generation via Prim's algorithm
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "random.h"
#include "UnionFind.h"
#include "set.h"
#include "PrimHelper.h"

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
    /*
     * From an efficiency standpoint, I chose to use three Grids to represent
     *    the underlying data I needed for this assignment (parent nodes,
     *    node costs, and node colors). I did this because in using this data,
     *    I would always know the (row, col) coordinates and as such, I wanted
     *    a data structure that would allow for very fast x, y lookups. A Grid
     *    is such a structure. Further, it is always a good idea to use
     *    someone else's well written and optimized library code when you can
     *    instead of writing your own.
     * Three Grids: parentNode, nodeCosts, nodeColors
     */
    // store the parent cells/nodes for each specific location
    Grid<Loc> parentNode(world.numRows(), world.numCols());

    // stores the total cost in getting from the start node to any
    //   given cell; remember that this is a cumulative cost
    Grid<double> nodeCosts(world.numRows(), world.numCols());
    
    // priority queue to store locations to examine and their associated costs
    TrailblazerPQueue<Loc> locsToExamine;
        
    ////////// FOLLOWING PSEUDOCODE //////////
    // this grid is also used to store which cells are in the priority queue
    //   as a cell that is in the priority queue will be yellow while a cell
    //   that has been visited will be green
    Grid<Color> nodeColors(world.numRows(), world.numCols());
    
    // color the start node yellow; this also denotes that
    //   the node is in the PQueue but has not been dequeued yet
    /*
     * Initially, I had a wrapper function that both colored the grid and
     *   called colorCell. This ensured that we never accidently introduce a
     *   bug where the status of the cell is changed in one
     *   location (i.e., the Grid that tracks the cell status)
     *   but there is no attendant call to colorCell();
     *
     * That function had the following signature:
     *    void setNodeColor(Loc cell, Grid<Color>& nodeColors,
     *      Grid<double>& world, Color color);
     * I removed this function in order to stay more close to this assignment
     *   predefined method signatures.
     */
    nodeColors[start.row][start.col] = YELLOW;
    colorCell(world, start, YELLOW);
    
    // set startNode's candidate distance to 0.
    nodeCosts[start.row][start.col] = 0;

    // Enqueue startNode into the priority queue with priority 0
    //   or (h(start,end)).
    locsToExamine.enqueue(start, heuristic(start, end, world));
    
    // Continue iterating through nodes until we have found the end cell
    //   (i.e., curr == end)
    while (true) {
        // Dequeue the lowest-cost node curr from the priority queue.
        Loc curr = locsToExamine.dequeueMin();
        
        // Color curr green. (The candidate distance dist that is currently
        //   stored for node curr is the length of the shortest path from
        //   startNode to curr.)
        nodeColors[curr.row][curr.col] = GREEN;
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
                if (nodeColors[v.row][v.col] == GRAY) {
                    nodeColors[v.row][v.col] = YELLOW;
                    colorCell(world, v, YELLOW);
                    
                    nodeCosts[row][col] = vPathCost;
                    parentNode[row][col] = curr;
                    locsToExamine.enqueue(v, vPathCost + heuristic(v, end, world));
                }
                // Otherwise, if v is yellow and the candidate distance to v is greater than dist + L:
                //   (a) Set v's candidate distance to be dist + L.
                //   (b) Set v's parent to be curr.
                //   (c) Update v's priority in the priority queue to dist + L.
                else if (nodeColors[v.row][v.col] == YELLOW &&
                           nodeCosts[v.row][v.col] > vPathCost) {
                    nodeCosts[v.row][v.col] = vPathCost;
                    parentNode[v.row][v.col] = curr;
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
        curr = parentNode[curr.row][curr.col];
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

/* Function: createMazePrim
 *
 * Take a number of rows and a number of columns and construct a maze
 *   by eliminating some nodes via Prim's Algorithm
 */
Set<Edge> createMazePrim(int numRows, int numCols) {
    // STEP 1: Construct the edges and place them into a data structure
    //         that will hold information about edges for use with Prim's
    //         algorithm
    PrimHelper primHelper(numRows, numCols);
    
    //         Step 1A: Construct Horizontal Edges
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols - 1; col++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row, col + 1);
            Edge ab = makeEdge(a, b);
            
            // add the constructed edge to the repository of edges stored
            //   for use by Prim's algorithm
            primHelper.add(ab, randomReal(0, 100));
        }
    }
    
    //         Step 1B: Construct Vertical Edges
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows - 1; row++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row + 1, col);
            Edge ab = makeEdge(a, b);

            // add the constructed edge to the repository of edges stored
            //   for use by Prim's algorithm
            primHelper.add(ab, randomReal(0, 100));
        }
    }

    // STEP 2: Traverse through the graph picking the minimum adjacent node
    //   that does not create a cycle yet has one edge that we've already
    //   visited
    // STEP 2-A: Create a set of nodes (i.e., Loc's) that we've visited
    Set<Loc> visited;
    
    // STEP 2-B: Pick arbitrary node to seed the main process of Prim's algorithm
    //           This will be the first visited location
    Loc nextLoc = makeLoc(numRows / 2, numCols / 2);
    visited += nextLoc;
    
    // STEP 2-C: Create a set of result edges (i.e., the final edges)
    Set<Edge> results;
    
    // Step 2:D Choose the smallest elligable neighbor edge from all the
    //          locations we've visited thus far and then move
    //          to that location and repeat this step
    while (true) {
        try {
            Edge nextEdge = primHelper.getNextEdge(visited);
            results += nextEdge;
        } catch (ErrorException) {
            // nothing more to explore, so quit
            break;
        }
    }
    return results;
}

/* Function: createMaze
 *
 * Take a number of rows and a number of columns and construct a maze
 *   by eliminating some nodes via Kruskal's Algorithm
 */
Set<Edge> createMaze(int numRows, int numCols) {
    // The following line of code can be commented/uncommented to set
    //   whether maze generation happens via Kruskal's algorithm or Prim's algorithm.
    // Since the default assignment specifies Kruskal's algorithm, I have
    //   commented out the following line so Kruskal's algorithm is used.
    return createMazePrim(numRows, numCols);
    
    // STEP 1: Construct the edges and place them into a data structure
    Set<Edge> edges;
    //         Step 1A: Construct Horizontal Edges
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols - 1; col++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row, col + 1);
            Edge ab = makeEdge(a, b);
            
            edges.add(ab);
        }
    }
    
    //         Step 1B: Construct Vertical Edges
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows - 1; row++) {
            Loc a = makeLoc(row, col);
            Loc b = makeLoc(row + 1, col);
            Edge ab = makeEdge(a, b);
            
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
    //             to be returned to the calling context.
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
