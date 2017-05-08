#include "DJS.h"
#include <vector>
#include <iostream>
#include "util.h"
using namespace std;

DJS_result DJS(vector<vector<int>> matrix, int nnode, int root, int flag, int max_cost)
{
	vector<int> prev_(nnode, -1);
	vector<int> distance(nnode, flag);
	vector<bool> in(nnode, false);
	in[root] = true;
	distance[root] = 0;
	int startnode = root;
	for (int i = 0; i < nnode; i++)
	{
		distance[i] = matrix[startnode][i];
		if (i!=root && distance[i] != flag)
			prev_[i] = root;
	}
	
	for (int i = 0; i < nnode - 1; i++)
	{
		int min_distance = flag;
		for (int j = 0; j < nnode; j++)
		{
			if (!in[j] && distance[j] < min_distance)
			{
				startnode = j;
				min_distance = distance[j];
			}
		}
		if (distance[startnode] >= max_cost)
			break;
		in[startnode] = true;
		
		for (int j = 0; j < nnode; j++)
		{
			if (!in[j] && matrix[startnode][j] != flag)
			{
				if (distance[j] > distance[startnode] + matrix[startnode][j])
				{
					distance[j] = distance[startnode] + matrix[startnode][j];
					prev_[j] = startnode;
				}
			}
		}
	}
	return DJS_result{ prev_, distance };
}