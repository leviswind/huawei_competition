#include "BFS.h"

BFS_result BFS(vector<vector<BFS_cell>> next, int nnode) {
	int s = nnode + 1;
	int t = nnode;
	queue<int> q;
	vector<bool> visited(nnode + 2, false);
	vector<int> distance(nnode + 2, INT_MAX);
	vector<int> index(nnode + 2, -1);
	distance[s] = 0;
	visited[s] = true;
	int v;
	vector<int> prev(nnode + 2, -1);
	q.push(s);
	while (!q.empty()) {
		v = q.front();
		for (int i = 0; i < next[v].size(); i++) {
			int k = next[v][i].next;
			if (!visited[k]) {
				q.push(k);
				prev[k] = v;
				visited[k] = true;
				distance[k] = distance[v] + next[v][i].cost;
				index[k] = next[v][i].index;
			}
		}
		if (visited[t])
			break;
		q.pop();
	}
	return BFS_result{ visited, prev, distance, index };
}