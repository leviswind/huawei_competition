#include "minmax2.h"
#include <map>
#include <queue>
#include <cmath>
#include "util.h"
#include <ctime>
#include <cstring>
#include <set>
using namespace std;


int pEdge;
int total_flow = 0;
int total_cost = 0;
int MAX_NODE_NUM = 10000;
int source;
int target;

edgeLink *edges[2 * 30000];
edgeLink edges_pool[5 * 300000];


void attach_edge(int node1, int node2, int bandwidth, int cost){

    edgeLink * edge1 = edges_pool + pEdge++;
    edgeLink * edge2 = edges_pool + pEdge++;

    *edge1 = (edgeLink){node2, bandwidth, cost, bandwidth, edges[node1], edge2};
    edges[node1] = edge1;

    *edge2 = (edgeLink){node1, 0, -cost, 0, edges[node2], edge1};
    edges[node2] = edge2;

}

int cal_flow(int node, int flow){

//    ////cout << "node" << node<<"\t" << flow << endl;
    if (node == target){
        
        total_cost = total_cost + param.distance * flow;
        return flow;
    }
    int tmp = flow;
    param.visited[node] = true;

    for (edgeLink * i = edges[node];i; i = i -> next){
//        ////cout << "i" << i ->node<<",";
//        ////cout << "i" << i ->bandwidth1 << "\t"<< i ->cost << "\t"<< param.visited[i->node] <<",";
        if(i -> bandwidth1 && !i ->cost && !param.visited[i->node]){
//            ////cout << "next" << i ->node<<",";
            int use_flow = cal_flow(i -> node, min(tmp, i -> bandwidth1));

            i -> bandwidth1 = i ->bandwidth1 - use_flow;
            i -> reverse -> bandwidth1 = i -> reverse -> bandwidth1 + use_flow;

            tmp = tmp - use_flow;

            if(!tmp){
                return flow;
            }
        }
    }
    return flow - tmp;

}

bool max_flow(){

    deque <int> q;

    memset(param.visited, 0, sizeof(param.visited));

    //param.visited = vector<int> (param.visited.size(),0);

    vector<int> d(MAX_NODE_NUM, INT_MAX);

    q.push_back(source);

    d[source] = 0;
    param.visited[source] = true;

    while(!q.empty()){
        int top = q.front();

        q.pop_front();
        param.visited[top] = false;

        for( edgeLink *i=edges[top];i;i = i->next){

            int v = i ->node;

            if(i->bandwidth1 && d[top] + i->cost < d[v]){

                d[v] = d[top] + i->cost;

                if(param.visited[v]) continue;

                param.visited[v] = true;

                if(q.size() && d[v] < d[q[0]]){

                    q.push_front(v);

                }else{

                    q.push_back(v);

                }
            }
        }
    }

    for (edgeLink * i = edges_pool; i < edges_pool + pEdge; ++i){

        int used_cost = (d[i->node] - d[i->reverse->node]);
        i->cost = i->cost - used_cost;

    }

    param.distance = param.distance + d[target];
    return d[target] < INT_MAX;

}

int flow_cost(){

    total_cost = 0;
    param.distance = 0;
    total_flow = 0;

//    ////cout << "f1" <<endl;

    while(max_flow()){
        int tmp_flow;

//        ////cout << "f4" <<endl;
        do {
//            ////cout << "f3" <<endl;
            memset(param.visited, 0, sizeof(param.visited));
            
            tmp_flow = cal_flow(source, INT_MAX);
//            ////cout << "tmp_flow"<<tmp_flow << endl;
            total_flow = total_flow + tmp_flow;

        } while (tmp_flow != 0);
        
    }

//    ////cout << "f2" <<endl;

    if (total_flow != param.band_need) {
        total_cost = INT_MAX;
    }

    return total_cost;

}


/*
map < map<int, int>>  near_nodes_and_band(vector<vector<int>> & nodes){

//    ////cout <<"near_nodes_and_band" <<endl;

    map<map<int,int>> saves;
    /*

    memset(param.visited, 0, sizeof(param.visited));
    for (int i = 0; i < nodes.size(); ++i){
        for (int j = 0; j < nodes[i].size();++j){
            int node = nodes[i][j];

            if (node < param.n_node && !param.visited[node] ){
                
                //////cout << "nodes" << nodes[i][j] << "\t";
                v.push_back((next_and_band){node, 0});
                for (edgeLink *i = edges[node]; i; i = i ->next){

                if (i->node < param.n_node&& i ->bandwidth2){
//                  ////cout <<"next" << i->node <<","<< "band" << i ->bandwidth2 << " ";
                    map.insert((next_and_band){i->node, i ->bandwidth2});
                }

            }

            saves.push_back(v);

            }

            

            param.visited[node] = true;
        }
    }
    return saves;
}
*/

void printNear(vector<vector<next_and_band>> node){
    for (int i = 0; i < node.size(); ++i){
        for (int j = 0; j < node[i].size(); ++j){
            ////cout <<"next" << node[i][j].next << "band"<<node[i][j].band<<",";
        }
        ////cout << endl;
    }
}


minmax2_struct minMax2(const vector<int> &cdn,const vector<int> &level){

    //cout <<"tag2"<<endl;

    ////cout <<"minMax2 begin" << endl;

    source = param.n_node + param.n_consume;
    target = source + 1;
//    ////cout << source << target << endl;


    map<int,map<int,int>> node_and_band;

    pEdge = 0;
    /*
    for (int i = 0; i < param.graph.size(); ++i){
        for (int j = 0; j < param.graph[i].size(); ++j){
            ////cout << param.graph[i][j].bandwidth << ",";
        }
        ////cout << endl;
    }*/

    //cout <<"1" << endl;

    memset(&edges, 0, sizeof(edges));


    //cout <<"2" << endl;

    for (int i =0; i < param.graph.size(); ++i){
        for (int j =0; j < param.graph[i].size(); ++j){
            attach_edge(i, param.graph[i][j].node, param.graph[i][j].bandwidth, param.graph[i][j].cost);
        }
    }

    for ( int i = 0; i < param.n_consume; ++i){
        attach_edge(param.customer_ids[i].network_id, param.customer_ids[i].customer_id + param.n_node, param.customer_ids[i].bandwidth, 0);
        attach_edge(param.customer_ids[i].customer_id + param.n_node, target, param.customer_ids[i].bandwidth, 0);
    }

    for (int i = 0; i < cdn.size(); ++i){
        for(int j=1;j<=param.max_level;j++){
            attach_edge(source, cdn[i], param.base_caps[j] - param.base_caps[j-1], param.base_per_costs[j]);
        }
        
    }
    int final_cost = flow_cost();



    vector <vector <int>> nodes;
    map <int, map <int, int>> useMap;

    while(true){
        vector<int> tmp;
        int tmp_node = source;
        int useflow = INT_MAX;
        int counts = 0;

        while(tmp_node!=target){

            bool flag = false;

            for (edgeLink *i = edges[tmp_node]; i; i =i->next){
                int next_node = i -> node;

            //    ////cout << next_node << endl;

                if(i->bandwidth2 > i->bandwidth1){
                    useflow = min(useflow, (i ->bandwidth2) -(i -> bandwidth1));
                    counts  ++;

                    if (counts <= 2){

                        if(!useMap[tmp_node][next_node]){
                            useMap[tmp_node][next_node] = (i ->bandwidth2) -(i -> bandwidth1);
                        }
                        ////cout << tmp_node <<"\t" << next_node << "\t"<<useflow<< endl;
                    }

                    tmp_node = next_node;
                    flag = true;
                    break;
                }
            }

            if(!flag) break;
        }

        if(tmp_node!=target) break;
        tmp_node = source;


        while(tmp_node!= target){
            
            for(edgeLink *i = edges[tmp_node];i; i = i->next){
                int next_node = i->node;
    //          ////cout <<"v" << "\t"<< v << endl;

                if(i -> bandwidth2 > i->bandwidth1){
                    i ->bandwidth1 += useflow;
                    tmp_node = next_node;
                    break;
                }
            }

            if(tmp_node!= target) {
                tmp.push_back(tmp_node);}
        }
        nodes.push_back(tmp);
    }

    //cout <<"tag2"<<endl;

    return (minmax2_struct){useMap,total_flow, final_cost};
}



minmax2_struct minMax2(const vector<int> &cdn){
    vector<int> level(cdn.size(),param.max_level);
    return minMax2(cdn,level);
}