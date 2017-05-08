#ifndef BFS_H
#define BFS_H
#include "io.h"
#include "global.h"
#include <queue>

using namespace std;

BFS_result BFS(vector<vector<BFS_cell>> next, int nnode);
#endif
