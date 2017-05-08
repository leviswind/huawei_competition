#ifndef BFS_DOUBLE_H
#define BFS_DOUBLE_H
#include <vector>
using namespace std;
struct DJS_result {
	vector<int> bfs_prev;
	vector<int> cost;
};

DJS_result DJS(vector<vector<int>> matrix, int nnode, int root, int flag, int max_cost);


#endif