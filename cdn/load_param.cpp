#include "load_param.h"
#include <iostream>
#include "util.h"
using namespace std;
//params_base param;

void char2num(char *buf, vector<int> &r) {
	r.clear();
	int tmp = 0, n = *buf - '0';
	do {
		while (n >= 0 && n <= 9) {
			tmp = tmp * 10 + n;
			++buf;
			n = *buf - '0';
		}
		r.push_back(tmp);
        //cout<<tmp<<" ";
		tmp = 0;
		n = *(++buf) - '0';
	} while (n >= 0 && n <= 9);
    //cout<<endl;
}

/*
bool load_param(char* lines[], params &param) {
	vector<int> intbuf;
	int line_i=0;
	char2num(lines[line_i++], intbuf);
	param.n_node = intbuf[0];
	param.n_edge = intbuf[1];
	param.n_consume = intbuf[2];

	++line_i;
	char2num(lines[line_i++], intbuf);
	param.base_cost = intbuf[0];
	++line_i;
	int max_cost = 0;

	vector<vector<basicCell>> basicMap(param.n_node + 2, vector<basicCell>(param.n_node + 2, basicCell{ INT_MAX, 0 }));
	for (int i = 0; i < param.n_edge; ++i) {
		char2num(lines[line_i++], intbuf);
		basicMap[intbuf[0]][intbuf[1]] = basicCell{ intbuf[3], intbuf[2] };
		basicMap[intbuf[1]][intbuf[0]] = basicCell{ intbuf[3], intbuf[2] };
		if (intbuf[3] > max_cost)
			max_cost = intbuf[3];
	}
	param.max_cost = max_cost + 1;
	++line_i;
	vector<int> consume_init(param.n_node,-1);	
	for (int i = 0; i < param.n_consume; ++i) {
		char2num(lines[line_i++], intbuf);
		consume_init[intbuf[1]]=i;
		basicMap[intbuf[1]][param.n_node] = basicCell{ 0, intbuf[2] };
		basicMap[param.n_node][intbuf[1]] = basicCell{ 0, intbuf[2] };
	}
	param.consume_init=consume_init;
	param.basicMap = basicMap;
	return true;
}
*/
void load_param_base(char* lines[]) {
	vector<int> intbuf;
	int line_i=0;
	char2num(lines[line_i++], intbuf);
	param.n_node = intbuf[0];
	param.n_edge = intbuf[1];
	param.n_consume = intbuf[2];
    param.base_costs=vector<int>(11,0);
    param.base_caps=vector<int>(11,0);
    param.base_per_costs=vector<int>(11, 0);
    while(++line_i){
        char2num(lines[line_i], intbuf);
        if(intbuf.size()!=3)break;
        if(param.max_cap<intbuf[1]){
            param.max_level=intbuf[0];
            param.max_cap=intbuf[1];
            param.base_cost=intbuf[2];
        }
        param.base_caps[intbuf[0] + 1]=intbuf[1];
        param.base_costs[intbuf[0] + 1]=intbuf[2];
        param.base_per_costs[intbuf[0] + 1] = \
                    (param.base_costs[intbuf[0] + 1] - param.base_costs[intbuf[0]]) / (param.base_caps[intbuf[0] + 1] - param.base_caps[intbuf[0]]);
    }

    ////cout<<"max_cap:"<<param.max_cap<<endl;//cout<<"  max_cost:"<<param.base_cost<<endl;while(1);

    param.node_costs=vector<int>(param.n_node,0);
    while(++line_i){
        char2num(lines[line_i], intbuf);
        if(intbuf.size()!=2)break;
        param.node_costs[intbuf[0]]=intbuf[1];
    }
    ++line_i;

	unsigned int n_edge_all = param.n_edge * 2 + 2 * param.n_consume;
	param.next_nodes.resize(param.n_node);
	param.customer_ids.resize(param.n_node);
	param.graph.resize(param.n_node, vector <LinkInfo> ());

	param.edge = { n_edge_all, edgeBasicCell{ 0,0,0,0 } };
	param.node_band = vector<int>(param.n_node, 0);
	param.consume_id = vector<int>(param.n_node,-1);
	param.band_need = 0;
	for (int i = 0; i < param.n_edge; ++i) {
		char2num(lines[line_i++], intbuf);
		param.edge[2 * i] = edgeBasicCell{ intbuf[0],intbuf[1],intbuf[3] ,intbuf[2] };
		param.edge[2 * i + 1] = edgeBasicCell{ intbuf[1],intbuf[0],intbuf[3],intbuf[2] };
		param.node_band[intbuf[0]] += intbuf[2];
		param.node_band[intbuf[1]] += intbuf[2];
		param.next_nodes[intbuf[0]].push_back(intbuf[1]);
		param.next_nodes[intbuf[1]].push_back(intbuf[0]);

		param.graph[intbuf[0]].push_back(LinkInfo{intbuf[1],intbuf[3],intbuf[2]}) ;
		param.graph[intbuf[1]].push_back(LinkInfo{intbuf[0],intbuf[3],intbuf[2]}) ;

	}
	++line_i;
	for (int i = param.n_edge; i < param.n_edge + param.n_consume; ++i) {
		char2num(lines[line_i++], intbuf);
		param.consume_id[intbuf[1]] = i- param.n_edge;
		param.edge[2 * i] = edgeBasicCell{ intbuf[1],param.n_node,0,intbuf[2] };
		param.edge[2 * i + 1] = edgeBasicCell{ param.n_node,intbuf[1],0,0 };
		param.node_band[intbuf[1]] += intbuf[2];
		param.band_need += intbuf[2];

		param.customer_ids[i - param.n_edge] = (cust_net){intbuf[0], intbuf[1], intbuf[2]};
	}
}
