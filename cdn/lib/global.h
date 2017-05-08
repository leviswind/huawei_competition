#ifndef GLOBAL_H
#define GLOBAL_H
#include <vector>
#include <map>
#include <ctime>
using namespace std;

#define INT_MAX 6553699


struct ourResult {
	int n_path;
	vector<vector<int>> paths;
};
struct edgeCell {
	int start;
	int end;
	int cost;
	int pos_capacity;
	int neg_capacity;
};



struct edgeBasicCell {
	int start;
	int end;
	int cost;
	int capacity;
};

struct edgeSearchCell {
	int next;
	int cost;
	int capacity;
};

struct minMaxResult {
	vector<edgeCell> residual_edges;
	int costs;
	int capacity;
	map<int,int> bases;
    map<int, map<int, int>> bases_more;
};

struct searchResult {
	vector<vector<int>> paths;
	vector<int> costPerPath;
	vector<int> PathCapacity;
	int costs;
	int cap_total;
};

struct FORD_result {
	vector<int> ford_pre;
	vector<int> cost;
	vector<int> index;
};

struct BFS_result {
	vector<bool> visited;
	vector<int> prev;
	vector<int> cost;
	vector<int> index;
};

struct BFS_cell {
	int next;
	int cost;
	int index;
};

struct edgeLink{

	int node, bandwidth1, cost, bandwidth2;

	edgeLink * next;
	edgeLink * reverse;
	
};

struct cust_net{

	int customer_id, network_id, bandwidth;
	
};

struct params {
	int w_num;
	int l_num;
	int x_num;
	int bushu;
	std::vector<std::vector<int>> map_feiyong;
	std::vector<std::vector<int>> map_rongliang;
	std::vector<int> xuqiu;
};

struct LinkInfo
{
	int node;
	int cost;
	int bandwidth;
};

struct params_base {
	int n_node;
	int n_edge;
	int n_consume;
	vector<edgeBasicCell> edge;
	vector<int> node_band;
	int band_need;
	int base_cost;
	vector<int> consume_id;
	vector<vector<int>> next_nodes;

	vector<vector<LinkInfo>> graph;
	vector<cust_net> customer_ids;
	int distance;
	bool visited[10000];
	int max_cap;
	int max_level;
	vector<int> base_costs;
	vector<int> base_caps;
	vector<int> node_costs;
	vector<int> base_per_costs;
};



struct next_and_band
{
	int next;
	int band;
	
};

struct minmax2_struct
{
	map<int,map<int,int>> bases;
	int total_flow;
	int total_cost;

};

typedef searchResult(FIND)(vector<edgeBasicCell>, vector<int>, int);
typedef minMaxResult(FIND2)(vector<edgeBasicCell>&,const vector<int>&, int);


extern bool shouldend;
extern clock_t endtime;
extern params_base param;


#endif
