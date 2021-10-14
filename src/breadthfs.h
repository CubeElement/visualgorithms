#ifndef BREADTHFS_H
#define BREADTHFS_H

#include "grid.h"

#include <vector>

class BreadthFS : public Grid
{
    Q_OBJECT

public:
    explicit BreadthFS(QWidget *parent = nullptr);

protected:
    virtual std::vector<int> shortestPath(int start, int end) override;
};

#endif // BREADTHFS_H
