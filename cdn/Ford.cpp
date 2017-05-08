#include "Ford.h"
#include "util.h"
#include <iostream>
FORD_result Ford(vector<vector<int>> matrix, int nnode, int root, int flag)
{
	vector<edgeCell> edges;
	for (int i = 0; i < nnode; i++)
	{
		for (int j = 0; j < nnode; j++)
		{
			if (matrix[i][j] != flag && matrix[i][j] != 0)
				edges.push_back(edgeCell{ i, j, matrix[i][j],INT_MAX });
		}
	}
	return Ford(edges, nnode, root, flag);
}

FORD_result Ford(const vector<edgeCell> &edges, int nnode, int root, int flag)
{
	vector<int> distance(nnode, flag);
	vector<int> prev(nnode, -1);
	vector<int> index(nnode, -1);
	distance[root] = 0;
	int i = 0;
	for (i = 0; i < nnode - 1; i++)
	{
		bool updated = false;
		for (int j = 0; j < edges.size(); j++)
		{
			edgeCell edge = edges[j];
			int capacity = edge.cost >= 0 ? edge.pos_capacity : edge.neg_capacity;
			if (capacity > 0 && distance[edge.start] != flag && (distance[edge.end] > (distance[edge.start] + edge.cost)))
			{
				prev[edge.end] = edge.start;
				index[edge.end] = j;
				distance[edge.end] = distance[edge.start] + edge.cost;
				updated = true;
				/*
				cout << "edge.start: " << edge.start << "edge.end: " << edge.end << " " << capacity << endl;
				printvector(prev);
				printvector(distance);*/
			}
		}
		if (!updated)
			break;
		
	}
	//cout << "循环次数:" << i << " " << "边数:" << edges.size() << endl;
	return FORD_result{ prev, distance, index};
}