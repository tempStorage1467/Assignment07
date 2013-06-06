/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
using namespace std;


/*
 Don't forget to adjust the parent pointers in Dijkstra's algorithm or A* search after calling decreaseKey. Otherwise, even though you'll dequeue the nodes in the proper order, your resulting path will be incorrect.
 */

/*
 Dijkstra's algorithm has found the shortest path from the start node to the end only when the
 end node has been dequeued from the priority queue (that is, when it colors the node green). It
 is possible to enqueue the end node into the priority queue but still not have a shortest path to it,
 since there might be a shorter path to the end node that has not been considered yet.
 */

/*
 Although A* search enqueues nodes into the priority queue with a priority based on both the
 node candidate distances and their heuristic values, it tracks their candidate distances independently of their heuristic costs. When storing the candidate distance to a node, do not add the
 heuristic value in. The heuristic is only used when setting the priorities in the priority queue
 */

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
             double costFn(Loc from, Loc to, Grid<double>& world)) {
	// TODO: Fill this in!
    
    // keep a counter for the number of visited cells
    // keep total distance in INTs to avoid annoying double computation errors
    
    // denote that all cells are currently grey
    // Note: do I even need to iterate over this, or is the default value Grey?
    // Note: can I just use this to store which nodes are in the priority queue?
    //   As in, can I write a wrapper such as bool inPriorityQueue(Loc) that
    //   under the hood checks the nodeColors and sees if it the node at
    //   Loc is yellow
    Grid<Color> nodeColors(world.numRows(), world.numCols());
    
    Grid<double> nodeCosts(world.numRows(), world.numCols());
    
	error("shortestPath is not implemented yet.");
    return Vector<Loc>();
}

Set<Edge> createMaze(int numRows, int numCols) {
	// TODO: Fill this in!
	error("createMaze is not implemented yet.");
    return Set<Edge>();
}
