#include "depthfs.h"
#include "breadthfs.h"

#include <QApplication>
#include <iostream>
#include <queue>

DepthFS::DepthFS(QWidget *parent)
    : Grid(parent) {}
std::vector<int> DepthFS::shortestPath(int start, int end)
{
    std::vector <int> previous(GRID_SIZE*GRID_SIZE);
    std::queue <int> queue;
    std::vector <int> visited = this->blockedCells;
    queue.push(start);
    visited.push_back(start);

    int current = start;
    bool foundEnd = false;

    killLoopFlag = false;

    while ( !queue.empty() && !foundEnd && !killLoopFlag )
    {
        current = queue.front();
        this->setCellColor(this->grid_elements[current], "current");
        delay(DELAY_MS);

        queue.pop();
        for ( int neighbor : neighborsList(current, GRID_SIZE) )
        {
            if ( killLoopFlag == true ) { break; }
            QApplication::processEvents();
            if ( std::count(visited.begin(), visited.end(), neighbor) == 0)
            {
                queue.push(neighbor);
                visited.push_back(neighbor);
                this->setCellColor(this->grid_elements[neighbor], "searched");
                delay(DELAY_MS);
                previous[neighbor] = current;
            }
            if ( neighbor == end ) { foundEnd = true; break; }
        }
    }
    drawShortestPath(previous, end, start);
    return previous;
}
