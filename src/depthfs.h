#ifndef DEPTHFS_H
#define DEPTHFS_H

#include "grid.h"

#include <vector>

class DepthFS : public Grid
{
    Q_OBJECT

public:
    explicit DepthFS(QWidget *parent = nullptr);

protected:
    virtual std::vector<int> shortestPath(int start, int end) override;
};

#endif // DEPTHFS_H
