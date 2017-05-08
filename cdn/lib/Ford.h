#ifndef FORD_H
#define FORD_H
#include "global.h"
#include "minmax.h"
FORD_result Ford(vector<vector<int>>, int nnode, int root, int flag);
FORD_result Ford(const vector<edgeCell>& edges, int nnode, int root, int flag);

#endif