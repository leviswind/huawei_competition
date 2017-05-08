#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <iostream>
#include "minmax.h"
using namespace std;


void printcost(vector<edgeCell> vec);

template <typename T>
void printvector(vector<T> vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		cout << vec[i] << "  ";
	}
	cout << endl;
}

void printedges(vector<int> prev);
vector<vector<int>> getedges(vector<int> prev);


#endif

