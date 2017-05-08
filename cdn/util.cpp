#include "util.h"
void printedges(vector<int> prev)
{
	for (int i = 0; i < prev.size(); i++)
	{
		int index = i;
		while (prev[index] != -1)
		{
			cout << index << "-->";
			index = prev[index];
		}
			
		if(index != i)
			cout << index<<endl;
	}
}

vector<vector<int>> getedges(vector<int> prev)
{
	vector<vector<int>> edges;
	for (int i = 0; i < prev.size(); i++)
	{
		vector<int> edge;
		int index = i;
		while (prev[index] != -1)
		{
			edge.push_back(index);
			index = prev[index];
		}
		if (index != i)
		{
			edge.push_back(index);
			edges.push_back(edge);
		}
	}
	return edges;
}

void printcost(vector<edgeCell> vec) {
	{
		cout << "------------------"<<endl;
		cout << "cost: ";
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec[i].cost << " ";
		}
		cout << endl;
		cout << "pos_capacity: ";
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec[i].pos_capacity << " ";
		}
		cout << endl;
		cout << "neg_capacity: ";
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec[i].neg_capacity << " ";
		}
		cout << endl;
	}
}