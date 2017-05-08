#include "minmax.h"
#include "Ford.h"
#include <map>
#include <queue>
#include <cmath>
#include "util.h"
#include "BFS.h"
#include <ctime>
#include <set>
using namespace std;

vector<edgeCell> Init(const vector<edgeBasicCell>& edges, const vector<int> &sourceNodes, int s, int nnode) {
	vector<edgeCell> residual_edges;
	for (int i = 0; i < edges.size(); i += 2)
	{
		edgeCell forward, backward;
		forward.start = edges[i].start; forward.end = edges[i].end;
		backward.start = edges[i + 1].start; backward.end = edges[i + 1].end;
		forward.cost = edges[i].cost; backward.cost = edges[i + 1].cost;
		forward.pos_capacity = edges[i].capacity; forward.neg_capacity = 0;
		backward.pos_capacity = edges[i + 1].capacity; backward.neg_capacity = 0;
		residual_edges.push_back(forward);
		residual_edges.push_back(backward);
	}

	for (int i = 0; i < sourceNodes.size(); i++) {
		edgeCell forward, backward;
		forward.start = s; forward.end = sourceNodes[i];
		backward.start = sourceNodes[i]; backward.end = s;
		forward.cost = 0; backward.cost = 0;
		forward.pos_capacity = param.max_cap; backward.pos_capacity = 0;
		forward.neg_capacity = 0; backward.neg_capacity = 0;
		residual_edges.push_back(forward);
		residual_edges.push_back(backward);
	}
	return residual_edges;
}

vector<vector<BFS_cell>> InitNext(vector<edgeCell> &residual_edges, int nnode) {
	vector<vector<BFS_cell>> next;
	next.resize(nnode + 2);
	for (int i = 0; i < residual_edges.size(); i++) {
		edgeCell cell = residual_edges[i];
		int capacity = cell.cost >= 0 ? cell.pos_capacity : cell.neg_capacity;
		if (capacity > 0)
			next[cell.start].push_back(BFS_cell{ cell.end, cell.cost, i });
	}
	return next;
}
minMaxResult minMax(vector<edgeBasicCell>& edges, const vector<int> &sourceNodes, int nnode, int s, int t, int f) {
	int res = 0;
	vector<edgeCell> residual_edges = Init(edges, sourceNodes, s, nnode);
	int capacity = 0;
	FORD_result result;
	result = Ford(residual_edges, nnode + 2, s, INT_MAX);
	map<int,int> bases;
	map<int, map<int, int>> bases_more;
	//cout<<"sourceNodes: "<<endl;
	//printvector(sourceNodes);
	//printvector(result.ford_pre);
	//printvector(result.cost);
	/*for (int v = t; v != s; v = result.ford_pre[v]) {
	cout << v << " ";
	}*/
	//cout << endl;
	int time = 0;
	while (result.cost[t] != INT_MAX) {
		int d = f;
		int vv;
		int prev_vv;
		for (vv = t; result.ford_pre[vv] != s; prev_vv = vv,vv = result.ford_pre[vv]) {
			edgeCell edge = residual_edges[result.index[vv]];
			int capacity = edge.cost >= 0 ? edge.pos_capacity : edge.neg_capacity;
			d = min(d, capacity);
		}
		
		edgeCell edge = residual_edges[result.index[vv]];
		int capacity2 = edge.cost >= 0 ? edge.pos_capacity : edge.neg_capacity;
		d = min(d, capacity2);
		f -= d;
		res += d * result.cost[t];
		capacity += d;
		bases[vv]+=d;
		bases_more[vv][prev_vv] += d;
		for (int v = t; v != s; v = result.ford_pre[v]) {
			int index = result.index[v];
			int reverse_index = index % 2 ? index - 1 : index + 1;
			edgeCell * edge = &(residual_edges[result.index[v]]);
			edgeCell * edge_reverse = &(residual_edges[reverse_index]);
			if (edge->cost >= 0) {
				if (edge->pos_capacity == INT_MAX)
					edge->pos_capacity = INT_MAX;
				else
					edge->pos_capacity -= d;
				edge_reverse->neg_capacity += d;
				edge_reverse->cost = edge_reverse->neg_capacity > 0 ? (-abs(edge->cost)) : abs(edge_reverse->cost);
			}
			else {
				if (edge_reverse->pos_capacity == INT_MAX)
					edge_reverse->pos_capacity = INT_MAX;
				else
					edge_reverse->pos_capacity += d;
				edge->neg_capacity -= d;
				edge->cost = edge->neg_capacity > 0 ? (-abs(edge_reverse->cost)) : abs(edge->cost);
			}
		}
		//printcost(residual_edges);
		//cout << "d" << d << " " << result.cost[t]<< " " << res << " "<<capacity<<endl;
		result = Ford(residual_edges, nnode + 2, s, INT_MAX);
		//printvector(result.ford_pre);
		//printvector(result.cost);
		time++;
	}
	if(capacity<param.band_need)res=INT_MAX-2;
	//cout << "FORD循环次数: " << time << endl;
	//cout << "容量为: " << capacity;

	//cout << "-------------------" << endl;
	return minMaxResult{ residual_edges, res, capacity, bases, bases_more};
}

minMaxResult minMax(vector<edgeBasicCell>& edges, const vector<int> &sourceNodes, int nnode) {
	return minMax(edges, sourceNodes, nnode, nnode + 1, nnode, INT_MAX);
}

searchResult parseResidual(const minMaxResult &minmax_result, int nnode, int s, int t) {
	vector<vector<edgeSearchCell>> next;
	vector<int> in(nnode + 2, 0);
	vector<int> out(nnode + 2, 0);
	next.resize(nnode + 2);
	for (int k = 0; k < minmax_result.residual_edges.size(); k++) {
		edgeCell edge = minmax_result.residual_edges[k];
		if (edge.neg_capacity > 0) {
			//cout << edge.start << " " << edge.end << " " << edge.neg_capacity << endl;
			next[edge.end].push_back(edgeSearchCell{ edge.start, abs(edge.cost), edge.neg_capacity });
			out[edge.end] += edge.neg_capacity;
			in[edge.start] += edge.neg_capacity;
		}
	}
	vector<vector<int>> mqueue;
	mqueue.push_back(vector<int>(1, s));
	vector<vector<int>> paths;
	vector<int> costPerPath;
	vector<int> PathCapacity;
	while (mqueue.size() != 0) {
		vector<int> before = mqueue.back();
		//printvector(before);
		int index = before.back();
		if (index == t) {
			//cout << "hello" << endl;
			int pathcapacity_ = INT_MAX;
			int cost = 0;
			for (int k = 0; k < before.size() - 1; k++) {
				for (int j = 0; j < next[before[k]].size(); j++) {
					if (next[before[k]][j].next == before[k + 1]) {
						pathcapacity_ = min(pathcapacity_, next[before[k]][j].capacity);
					}
				}
			}
			for (int k = 0; k < before.size() - 1; k++) {
				for (int j = 0; j < next[before[k]].size(); j++) {
					if (next[before[k]][j].next == before[k + 1]) {
						next[before[k]][j].capacity -= pathcapacity_;
						cost = cost + next[before[k]][j].cost;
					}
				}
			}
			if (pathcapacity_ != 0) {
				paths.push_back(vector<int>(before));
				costPerPath.push_back(cost);
				PathCapacity.push_back(pathcapacity_);
			}
			mqueue.pop_back();
			continue;
		}
		bool repeat = false;
		int son = 0;
		//cout << "--------------------------" << endl;
		for (int i = 0; i < next[index].size(); i++) {
			vector<int> temp;
			for (int j = 0; j < before.size(); j++) {
				temp.push_back(before[j]);
				if (before[j] == next[index][i].next)
					repeat = true;
			}
			if (!repeat) {
				temp.push_back(next[index][i].next);
				mqueue.push_back(temp);
				son++;
			}
			//printvector(temp);
		}
		mqueue.erase(mqueue.end() - son - 1);
	};
	//printvector(costPerPath);
	//printvector(PathCapacity);
	return searchResult{ paths, costPerPath, PathCapacity, minmax_result.costs,minmax_result.capacity };
}
ourResult parseResidual(const minMaxResult &minmax_result) {
	int nnode = param.n_node;
	int s = nnode + 1;
	int t = nnode;
	vector<int> base_level(param.n_node,0);

	for(auto ite=minmax_result.bases.begin();ite!=minmax_result.bases.end();ite++){
        int i=0;
        for(;i<10;i++){
            if(param.base_caps[i] >= ite->second)break;
        }
        base_level[ite->first] = i;
    }

	vector<vector<edgeSearchCell>> next(nnode + 2,vector<edgeSearchCell>());
	//cout<<"size:"<<minmax_result.residual_edges.size()<<endl;

	//next.resize(nnode + 2);
	//for (int k = 0; k < minmax_result.residual_edges.size(); k++) {
	//	edgeCell edge = minmax_result.residual_edges[k];
	for (edgeCell edge:minmax_result.residual_edges) {
		if (edge.neg_capacity > 0) {
			//cout << edge.start << " " << edge.end << " " << edge.neg_capacity << endl;
			next[edge.end].push_back(edgeSearchCell{ edge.start, abs(edge.cost), edge.neg_capacity });
		}
	}
	vector<vector<int>> mqueue;
	mqueue.push_back(vector<int>(1, s));
	vector<vector<int>> paths;
	vector<int> PathCapacity;
	while (mqueue.size() != 0) {
		vector<int> before = mqueue.back();
		//printvector(before);
		int index = before.back();
		if (index == t) {
			//cout << "hello" << endl;
			int pathcapacity_ = INT_MAX;
			int cost = 0;
			for (int k = 0; k < before.size() - 1; k++) {
				for (int j = 0; j < next[before[k]].size(); j++) {
					if (next[before[k]][j].next == before[k + 1]) {
						pathcapacity_ = min(pathcapacity_, next[before[k]][j].capacity);
					}
				}
			}
			for (int k = 0; k < before.size() - 1; k++) {
				for (int j = 0; j < next[before[k]].size(); j++) {
					if (next[before[k]][j].next == before[k + 1]) {
						next[before[k]][j].capacity -= pathcapacity_;
						cost = cost + next[before[k]][j].cost;
					}
				}
			}
			if (pathcapacity_ != 0) {
				paths.push_back(vector<int>(before));
				PathCapacity.push_back(pathcapacity_);
			}
			mqueue.pop_back();
			continue;
		}
		bool repeat = false;
		int son = 0;
		//cout << "--------------------------" << endl;
		for (int i = 0; i < next[index].size(); i++) {
			vector<int> temp;
			for (int j = 0; j < before.size(); j++) {
				temp.push_back(before[j]);
				if (before[j] == next[index][i].next)
					repeat = true;
			}
			if (!repeat) {
				temp.push_back(next[index][i].next);
				mqueue.push_back(temp);
				son++;
			}
			//printvector(temp);
		}
		mqueue.erase(mqueue.end() - son - 1);
	};
	vector<vector<int>> finalpaths;
	for (int i = 0; i < paths.size(); i++) {
		finalpaths.push_back(vector<int>(paths[i].begin() + 1, paths[i].end() - 1));
		finalpaths[i].push_back(param.consume_id[finalpaths[i].back()]);
		finalpaths[i].push_back(PathCapacity[i]);
		finalpaths[i].push_back(base_level[paths[i][1]]);
	}

	//printvector(costPerPath);
	//printvector(PathCapacity);
	int size = finalpaths.size();
	return ourResult{ size, finalpaths };
}

searchResult MinMax_search(vector<edgeBasicCell> ori_edges, vector<int> source_nodes, int nnode) {
	return parseResidual(minMax(ori_edges, source_nodes, nnode, nnode+1, nnode, INT_MAX), nnode, nnode+1, nnode);
}

searchResult MinMax_search(vector<edgeBasicCell> ori_edges, vector<int> source_nodes, int nnode, int s, int t, int f) {
	return parseResidual(minMax(ori_edges, source_nodes, nnode, s, t, f), nnode, s, t);
}


