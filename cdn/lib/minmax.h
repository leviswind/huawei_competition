#ifndef MINMAX_H
#define MINMAX_H
#include "global.h"
using namespace std;


vector<edgeCell> Init(const vector<edgeBasicCell>& edges, const vector<int>& sourceNodes, int s, int nnode);

minMaxResult minMax(vector<edgeBasicCell>& edges, const vector<int>& sourceNodes, int nnode, int s, int t, int f);
minMaxResult minMax(vector<edgeBasicCell>& edges, const vector<int>& sourceNodes, int nnode);

searchResult parseResidual(const minMaxResult& minmax_result, int nnode, int s, int t);
ourResult parseResidual(const minMaxResult &minmax_result);

searchResult MinMax_search(vector<edgeBasicCell> ori_edges, vector<int> source_nodes, int nnode);
searchResult MinMax_search(vector<edgeBasicCell> ori_edges, vector<int> source_nodes, int nnode, int s, int t, int f = INT_MAX);



#endif
