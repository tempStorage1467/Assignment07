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

/*
 * This function is also a wrapper for colorCell(); This ensures that we
 *   never accidently introduce a bug where the status of the cell is
 *   changed in one location (i.e., the Grid that tracks the cell status)
 *   but there is no attendant call to colorCell();
 */
void setNodeColor(Loc cell, Grid<Color>& nodeColors,
                   Grid<double>& world, Color color) {
    if (cell.col > nodeColors.nCols || cell.row > nodeColors.nRows) {
        error("The cell is out of bounds");
    }
    
    switch (color) {
        case GRAY:
            nodeColors[cell.row][cell.col] = GRAY;
            colorCell(world, cell, GRAY);
        case YELLOW:
            nodeColors[cell.row][cell.col] = YELLOW;
            colorCell(world, cell, YELLOW);
        case GREEN:
            nodeColors[cell.row][cell.col] = GREEN;
            colorCell(world, cell, GREEN);
    }
}


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
    
    // remember that once we've dequeued something, it is the cheapest path period
    
    ////////// SETUP CODE //////////
    // keep a counter for the number of visited cells; Dijkstra's algorithm
    //  continues as long as we have unvisited cells
    int cellsVisited = 0;
    
    // store the parent cells for each specific location
    Grid<Loc> parentCell(world.numRows(), world.numCols());
        
    // stores the total cost in getting from the start node to any
    //   given cell; remember that this is a cumulative cost
    Grid<double> nodeCosts(world.numRows(), world.numCols());
    
    // priority queue to store locations to examine and their associated costs
    TrailblazerPQueue<Loc> locsToExamine;
    
    // shortest (or cheapest) path
    Vector<Loc> finalPathToEndCell;
    
    ////////// FOLLOWING PSEUDOCODE //////////
    // denote that all cells are currently grey; this behavior occurs by default
    //   as Gray is the default enum value
    // this grid is also used to store which cells are in the priority queue
    //   as a cell that is in the priority queue will be yellow while a cell
    //   that has been visited will be green
    Grid<Color> cellColors(world.numRows(), world.numCols());
    
    // color the start node yellow; marks node is in PQueue
    setNodeColor(start, cellColors, world, YELLOW);
    
    // set startNode's candidate distance to 0
    nodeCosts[start.row][start.col] = 0;

    // Enqueue startNode into the priority queue with priority 0.
    locsToExamine.enqueue(start, 0);
    
    // While not all nodes have been visited
    while (cellsVisited < world.numCols() * world.numRows()) {
        // Dequeue the lowest-cost node curr from the priority queue.
        Loc curr = locsToExamine.dequeueMin();
        cellsVisited++;
        
        // Color curr green. (The candidate distance dist that is currently
        //   stored for node curr is the length of the shortest path from
        //   startNode to curr.)
        setNodeColor(curr, cellColors, world, GREEN);
        
        // If curr is the destination node endNode, you have found the
        //   shortest path from startNode to endNode
        if (curr == end) break;

        cout << "Cur: " << curr.row << ", " << curr.col << endl;
        
        // For each node v connected to curr by an edge of length L:
        for (int row = curr.row - 1; row < curr.row + 2; row++) {
            for (int col = curr.col - 1; col < curr.col + 2; col++) {
                if (row == curr.row && col == curr.col) continue;
                if (row < 0 || row >= world.numRows() ||
                    col < 0 || col >= world.numCols()) continue;
                
                cout << "Seeing a new cell" << endl;
                cout << row << ", " << col << endl;
                
                // set v, the candidate cell, to be a location object
                Loc v = makeLoc(row, col);
                
                // cost to get to candidate cell, v, is the total cost to get
                //   to the current cell plus the incremental cost to get
                //   to the adjacent neighbor cell
                // = dist + L in pseudocode
                double vPathCost = nodeCosts[curr.row][curr.col] + costFn(curr, v, world);
                
                if (cellColors[v.row][v.col] == GRAY) {
                    setNodeColor(v, cellColors, world, YELLOW);
                    nodeCosts[row][col] = vPathCost;
                    parentCell[row][col] = curr;
                    locsToExamine.enqueue(v, vPathCost);
                } else if (cellColors[v.row][v.col] == YELLOW &&
                           nodeCosts[v.row][v.col] > vPathCost) {
                    nodeCosts[v.row][v.col] = vPathCost;
                    parentCell[v.row][v.col] = curr;
                    locsToExamine.decreaseKey(v, vPathCost);
                }
            }
        }
    }
    
    // found end node; trace back parent cell for each cell in the path
    //   and add each path cell to the output vector
    Loc curr = end;
    while (curr != start) {
        finalPathToEndCell += curr;
        curr = parentCell[curr.row][curr.col];
    }
    finalPathToEndCell+= start;
    
    return finalPathToEndCell;
}

Set<Edge> createMaze(int numRows, int numCols) {
	// TODO: Fill this in!
	error("createMaze is not implemented yet.");
    return Set<Edge>();
}
