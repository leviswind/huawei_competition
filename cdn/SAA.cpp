
#include "SAA.h"
#include <set>
#include "util.h"
#include <math.h>
#include <vector>
#include "minmax2.h"

#define rand_base (double(rand() % 1000000)/double(1000000))
struct low_high{
    int low;
    int high;
    int delta;
};


int getRandIndex(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    //cout<<"end------------------------------"<<endl;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(pow((param.base_costs[20] )/500.0, 3)) /double(ite->second) , 3);
        //end = end + pow(double(1) /double(ite->second) , 3);
        //cout<<end<<" ";
        weights.push_back(end);
    }
    //cout<<endl<<"end------------------------------"<<endl;
    double x = rand_base * end;
    //printvector(weights);
    //////cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    //cout<<"error"<<endl;
    return -1;
}

int getRandIndex_last(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(pow((param.base_costs[ite->first] )/500.0, 3)) , 3);
        weights.push_back(end);
    }
    double x = rand_base * end;
    //printvector(weights);
    //////cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    //////cout<<"error"<<endl;
    return -1;
}


int getRandlevel(vector<int> levels){
    int last=0;
    for(int l:levels){
        l+=last;
        last=l;
    }
    int rands=rand()%last;
    int i=0;
    while(i){
        if(levels[i]>rands){
            return i;
        }
    }
    return param.max_level;
}

low_high get_low_high(int cap){
    low_high tmp;
    for(int i=param.max_level;i>=0;i--){
        if(param.base_costs[i] < cap){
            tmp = low_high{i, i+1, cap-param.base_costs[i]};
            return tmp;
        }
    }
    return tmp;
}


int getRandIndex_2(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(ite->second), 1.5);
        weights.push_back(end);
    }
    double x = rand_base * end;
    //printvector(weights);
    //////cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    //////cout<<"error"<<endl;
    return -1;
}

int getRandIndex_2_old(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(1) / param.base_costs[ite->first], 0.8);
        weights.push_back(end);
    }
    double x = rand_base * end;
    //printvector(weights);
    //////cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    //////cout<<"error"<<endl;
    return -1;
}



int calcost(minmax2_struct &r){
    int result=r.total_cost;
    int nodes_costs = 0;

    for(auto ite=r.bases[param.n_consume+param.n_node].begin();ite!=r.bases[param.n_consume+param.n_node].end();ite++){
        int i=0;
        for(;i<11;i++){
            if(param.base_caps[i] >= ite->second)break;
        }
        nodes_costs += param.base_costs[i] - param.base_costs[i-1] +(ite->second - param.base_caps[i]) * param.base_per_costs[i];
        result+=param.node_costs[ite->first]+param.base_costs[i];
    }
    //cout<<"  bases_cost: "<<result-r.total_cost<<"  nodes_costs" << nodes_costs<<endl;
    return result;
}

vector<int> SAA_100(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }
    nodes.pop_back();
    //nodes.pop_back();
    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    vector<int> minnodes;
    minmax2_struct m2 = minMax2(nodes);

    cap = m2.total_flow;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(m2)) {
        costs = calcost(m2);
        min = costs;
        minnodes = nodes;       
    }
    map<int,map<int,int>> bestbase=m2.bases; 

    int count0=4000;
    int count=count0;
    int increment = 0;
    vector<int> nodes_cap(param.n_node, param.max_level);
    while (count--) {
        //////cout<<"jieshushijian:"<<endtime<<" dangqian:"<<clock()<<" haishen:------"<<(clock()-endtime)/CLOCKS_PER_SEC<<endl;
        //////cout<<"cishu:"<<cishu++<<"max_node"<<max_node<<"chazhi"<<(param.n_consume-r.bases.size())<<endl;
        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        int change = count / (count0 - count + 50 + increment) + 2;
        //int change = 103;
        cout<< "change: "<<change<<"  " << "increment" <<increment << " ";        //////cout<<"change"<<change<<endl;
        while(change--) {
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % (max_node);
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                int rands=rand() % param.n_node;
                nodes[index]=bestbase[param.n_node+param.n_consume].find(rands)==bestbase[param.n_node+param.n_consume].end()?rands:-1;
                continue;
            }else if(rand_base < 0.03){
                nodes[index] = -1;
                continue;
            }
            int max=0;
            for(int next_index :param.next_nodes[nodes[index]]){                
                if(m2.bases[param.n_node+param.n_consume][next_index]>max){
                    max=m2.bases[param.n_node+param.n_consume][next_index];
                    nodes[index]=next_index;
                }
            }
            nodes[index] = rand() % ((3*count)/count0+1) ?nodes[index]: rand() % param.n_node;
        }
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        vector<int> level(nodeset.size(), 0);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            //level[k]=getRandlevel(levelmap[*t]);
            input[k++] = *t;
        }
        //////cout << "node:";
        //printvector(input);
        m2 = minMax2(input);


        cout << "costs:" << calcost(m2) << " min:" << min<<"  size:"<<nodeset.size()<<"   time:"<<(clock()-endtime)/CLOCKS_PER_SEC << endl;

        if (m2.total_flow > cap||(m2.total_flow==cap&&calcost(m2) < costs)) {
                cap = m2.total_flow;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {

                    costs = calcost(m2);
                    if (min > costs) {
                        min = costs;
                        minnodes = input;
                        count=count0;
                        bestbase=m2.bases;
                        increment++;
                        //max_node = min / param.base_cost;
                        //cout<<"jieidan:";
                        //printvector(input);
                    }
                }
        }
        else {
                nodes = tmp;
        }

        
    }
    return minnodes;
}
vector<int> SAA_300(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }
    nodes.pop_back();
    //nodes.pop_back();
    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    vector<int> minnodes;
    minmax2_struct m2 = minMax2(nodes);

    cap = m2.total_flow;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(m2)) {
        costs = calcost(m2);
        min = costs;
        minnodes = nodes;       
    }

    map<int,map<int,int>> bestbase=m2.bases;
    int count0=5000;
    int count=count0;
    int increment = 0;
    while (count--) {

        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        
        int change = count / (count0 - count + 50 + increment) + 2;
        //int change = 103;
        cout<< "change: "<<change<<"  " << "increment" <<increment << " ";
        while(change--) {
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % (max_node);
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                //int rands=rand() % param.n_node;
                nodes[index]=rand()%3?getRandIndex_2(bestbase[param.n_node+param.n_consume]):-1;
                continue;
            }
            int base_index = getRandIndex(m2.bases[param.n_node+param.n_consume]);
            //cout<<"more --------------------"<<endl;
            for(int i=0;i<nodes.size();i++){
                if(nodes[i] == base_index){
                    index = i;
                    break;
                }
            }
            nodes[index] = rand() % ((3*count)/count0+2) ?getRandIndex_2(bestbase[param.n_node+param.n_consume]): rand() % param.n_node;
        }
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            input[k++] = *t;
        }
        //////cout << "node:";
        
        m2 = minMax2(input);


        cout << "costs:" << calcost(m2) << " min:" << min<<"   time:"<<(clock()-endtime)/CLOCKS_PER_SEC << endl ;

        if (m2.total_flow > cap||(m2.total_flow==cap&&calcost(m2) < costs)) {
                cap = m2.total_flow;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {
                    costs = calcost(m2);
                    if (min > costs) {
                        nodes=vector<int>(max_node,-1);
                        int i=0;
                        for(auto ite=m2.bases[param.n_consume+param.n_node].begin();ite!=m2.bases[param.n_consume+param.n_node].end();ite++){
                            nodes[i++]=ite->first;
                        }
                        min = costs;
                        minnodes = input;
                        count=count0;
                        bestbase=m2.bases;
                        increment ++;
                        //max_node = min / param.base_cost;
                    }
                }
        }
        else {
                nodes = tmp;
        }

    }
    //cout<<"----------"<<endl;
    //printvector(minnodes);
    //cout<<"-------------"<<endl;
    return minnodes;
}
vector<int> SAA_800(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }
    nodes.pop_back();
    nodes.pop_back();
    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    vector<int> minnodes;
    minmax2_struct m2 = minMax2(nodes);

    cap = m2.total_flow;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(m2)) {
        costs = calcost(m2);
        min = costs;
        minnodes = nodes;       
    }
    map<int,map<int,int>> bestbase=m2.bases;
    int count0=param.n_node*1 ;
    int count=count0;
    int increment = 0;
    int nodeset_size = param.n_consume;
    int targetnum=param.band_need/param.max_cap;
    while (1) {
        count--;
         //////cout<<"11more --------------------"<<endl;
        //////cout<<"jieshushijian:"<<endtime<<" dangqian:"<<clock()<<" haishen:------"<<(clock()-endtime)/CLOCKS_PER_SEC<<endl;

        //////cout<<"cishu:"<<cishu++<<"max_node"<<max_node<<"chazhi"<<(param.n_consume-r.bases.size())<<endl;
        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        //int change = max_node/2/(param.n_consume-max_node)+2;
        
        //int change = (count0 - count)/count0*param.n_node + bestbase[param.n_node+param.n_consume].size()/10;
        int change =  (count0) / (count0 - count + 5) + nodeset_size/5;
        int del = nodeset_size/5;
        //int change = 103;
        cout<< "change: "<<change<<"  " << "increment" <<increment << " ";
        //////cout<<"change: "<<change<<endl;
        //change+=rand()%3;

        while(del--){
            int index = 0;
            int base_index = getRandIndex(m2.bases[param.n_node+param.n_consume]);
            for(int i=0;i<nodes.size();i++){
                if(nodes[i] == base_index){
                    index = i;
                    break;
                }
            }
            nodes[index] = rand()%(max_node-nodeset_size+1)?nodes[index]:-1;
        }
        while(change--) {
            //////cout<<" --------------------"<<endl;
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % max_node;
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                int rands=rand() % param.n_node;
                nodes[index]=rand_base < 0.3 ?getRandIndex_2(bestbase[param.n_node+param.n_consume]):-1;
                continue;
            }
            int base_index = getRandIndex(m2.bases[param.n_node+param.n_consume]);
            //////cout<<"more --------------------"<<endl;
            for(int i=0;i<nodes.size();i++){
                if(nodes[i] == base_index){
                    index = i;
                    break;
                }
            }

            int index_node = getRandIndex_2(m2.bases[base_index]);
            nodes[index] = index_node<param.n_node?index_node:-1;                 
        } 
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        //vector<int> level(nodeset.size(), param.max_level);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            //level[k]=getRandlevel(levelmap[*t]);
            input[k++] = *t;
        }

        m2 = minMax2(input);
        int costtmp=calcost(m2);
        cout << "  costs:" << costtmp<<"  flow:"<<m2.total_flow << "  min:" << min<<"  size:"<<nodeset.size()<<"   time:"<<(clock()-endtime)/CLOCKS_PER_SEC << endl;
        //////cout << "max_node"<<max_node<<endl;
        if (m2.total_flow > cap||(m2.total_flow==cap&&costtmp < costs)) {
                cap = m2.total_flow;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {
                    nodeset_size = nodeset.size();
                    costs = costtmp;
                    if (min > costs) {
                        min = costs;
                        minnodes = input;
                        count=count0;
                        bestbase=m2.bases;
                        increment ++;
                        //max_node = min / param.base_cost;
                        //cout<<"jinlaile"<<endl;
                    }
                }
        }
        else {
            //if(cap==param.band_need&&m2.total_flow<param.band_need && (rand()%(m2.total_flow-param.band_need + 1)*(1+param.n_consume-nodeset_size) == 0)){
            //    costs=INT_MAX;
            //    nodeset_size = nodeset.size();
            //    cap=m2.total_flow;
            //    cout<<"-----------------------------"<<endl;
            //}
            if(m2.total_flow == param.band_need && rand()%(costtmp - costs+1)*(1+param.n_consume-nodeset_size) == 0){
                costs = costtmp;
                nodeset_size = nodeset.size();
            }
            else
                nodes = tmp;
        }


    }
    return minnodes;
}


minMaxResult SAA(FIND2 f) {

    vector<int> nodes;
    if(param.n_node > 600)
        nodes = SAA_800(f);
    else if(param.n_node > 200)
        nodes = SAA_300(f);
    else
        nodes = SAA_100(f);

        //cout<<"jieidan:";
        //printvector(nodes);
    return f(param.edge,nodes,param.n_node);
}
/*
#include "SAA.h"
#include <set>
#include "util.h"
#include <math.h>
#include <vector>

static double T = 10;
#define rand_base (double(rand() % 1000000)/double(1000000))
bool erand(double k) {
    //if(T<1)
        return false;
    if (k < 0)k = -k;
    k /= T;
    T *= 0.99;
    double r = exp(-k) * 1000;
    //cout << "rand:"<<r/1000<<endl;
    return r > rand() % 1000;
}

int getRandIndex(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(1)/double(ite->second), 3);
        weights.push_back(end);
    }
    double x = rand_base * end;
    //printvector(weights);
    //cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    cout<<"error"<<endl;
    return orders[0];
}
int getRandIndex4(map<int, int> bases){
    int min = INT_MAX;
    int minorder = -1;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        if(ite->second < min){
            minorder = ite->first;
            min = ite->second; 
        }
    }
    return minorder;
}

int getRandIndex_2(map<int, int> bases){
    vector<int> orders;
    vector<double> weights;
    double end = 0;
    for(map<int,int>::iterator ite=bases.begin();ite!=bases.end();ite++){
        orders.push_back(ite->first);
        end = end + pow(double(ite->second), 1.5);
        weights.push_back(end);
    }
    double x = rand_base * end;
    //printvector(weights);
    //cout<<"x :" <<x<<endl;
    for(int i=0;i<orders.size();i++){
        if(x <= weights[i]){
            return orders[i];
        }
    }
    cout<<"error"<<endl;
    return orders[0];
}

int calcost(const minMaxResult &r){
    int result=r.costs;
    for(auto ite=r.bases.begin();ite!=r.bases.end();ite++){
        int i=0;
        for(;i<10;i++){
            if(param.base_caps[i] >= ite->second)break;
        }
        result+=param.node_costs[ite->first]+param.base_costs[i];
    }
    return result;
}

minMaxResult SAA_100(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }
    nodes.pop_back();
    //nodes.pop_back();
    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    minMaxResult rmin;
    minMaxResult r = f(param.edge, nodes, param.n_node);
    cap = r.capacity;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(r)) {
        costs = calcost(r);
        min = costs;
        rmin = r;       
    }
    map<int,int> bestbase=r.bases;
    int count0=4000;
    int count=count0*2;
    while (count--) {
        //cout<<"jieshushijian:"<<endtime<<" dangqian:"<<clock()<<" haishen:------"<<(clock()-endtime)/CLOCKS_PER_SEC<<endl;
        //cout<<"cishu:"<<cishu++<<"max_node"<<max_node<<"chazhi"<<(param.n_consume-r.bases.size())<<endl;
        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        int change = max_node/3/(param.n_consume-max_node)+2;
        //cout<<"change"<<change<<endl;
        while(change--) {
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % (max_node);
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                int rands=rand() % param.n_node;
                nodes[index]=bestbase.find(rands)==bestbase.end()?rands:-1;
                continue;
            }
            int max=0;
            for(int next_index :param.next_nodes[nodes[index]]){                
                if(r.bases[next_index]>max){
                    max=r.bases[next_index];
                    nodes[index]=next_index;
                }
            }
            nodes[index] = rand() % ((3*count)/count0+1) ?nodes[index]: rand() % param.n_node;
        }
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            input[k++] = *t;
        }
        //cout << "node:";
        //printvector(input);
        r = f(param.edge, input, param.n_node);
        //cout<<"size: "<<(r.bases_more.begin()->second).size()<<endl;
        //int p = 0;
        //for (vector<int> path : r.paths) {
        //  cout << "ÈÝÁ¿" << r.PathCapacity[p++] << "Â·¾¶:";
        //  printvector(path);
        //}
        //cout << "band_need:" << param.band_need << endl;
        //cout << "cap:" << r.capacity << endl;
        cout<<param.max_cap<<":";
        for(map<int,int>::iterator ite=r.bases.begin();ite!=r.bases.end();ite++){
            cout<<ite->second<<" ";
        }
        cout<<endl;
        cout << "costs:" << calcost(r) << " min:" << min << endl;
        if (r.capacity >=cap&&calcost(r) <= costs) {
                cap = r.capacity;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {
                    costs = calcost(r);
                    if (min >= costs) {
                        min = costs;
                        rmin = r;
                        //cout<<"size---: "<<rmin.residual_edges.size()<<endl;
                        count=count0;
                        bestbase=r.bases;
                    }
                }
        }
        else {
            nodes = tmp;
        }
        
    }
    return rmin;
}
minMaxResult SAA_300(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }
    nodes.pop_back();
    //nodes.pop_back();
    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    minMaxResult rmin;
    minMaxResult r = f(param.edge, nodes, param.n_node);
    cap = r.capacity;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(r)) {
        costs = calcost(r);
        min = costs;
        rmin = r;       
    }
    map<int,int> bestbase=r.bases;
    int count0=5000;
    int count=count0*2;
    while (count--) {
        //cout<<"jieshushijian:"<<endtime<<" dangqian:"<<clock()<<" haishen:------"<<(clock()-endtime)/CLOCKS_PER_SEC<<endl;

        //cout<<"cishu:"<<cishu++<<"max_node"<<max_node<<"chazhi"<<(param.n_consume-r.bases.size())<<endl;
        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        int change = param.n_consume/3/(param.n_consume-max_node)+2;
        //int change = param.n_consume-max_node;
        while(change--) {
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % (max_node);
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                //int rands=rand() % param.n_node;
                nodes[index]=rand()%3?getRandIndex_2(bestbase):-1;
                continue;
            }
            int base_index = getRandIndex(r.bases);
            //cout<<"more --------------------"<<endl;
            for(int i=0;i<nodes.size();i++){
                if(nodes[i] == base_index){
                    index = i;
                    break;
                }
            }
            //int tmpmax=0;
            //for(int next_index :param.next_nodes[nodes[index]]){
            //    if(r.bases[next_index]>tmpmax){
            //        tmpmax=r.bases[next_index];
            //        nodes[index]=next_index;
            //    }
            //}
            //int tempcount = max(0, 4 * count - int(2 * count0));
            //nodes[index] = rand() % int(double(2*tempcount)/double(count0) + 2) ?nodes[index]: rand() % param.n_node;
            nodes[index] = rand() % ((3*count)/count0+2) ?getRandIndex_2(bestbase): rand() % param.n_node;
        }
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            input[k++] = *t;
        }
        //cout << "node:";
        //printvector(input);
        r = f(param.edge, input, param.n_node);
        //int p = 0;
        //for (vector<int> path : r.paths) {
        //  cout << "ÈÝÁ¿" << r.PathCapacity[p++] << "Â·¾¶:";
        //  printvector(path);
        //}


        cout<<param.max_cap<<":";
        for(map<int,int>::iterator ite=r.bases.begin();ite!=r.bases.end();ite++){
            cout<<ite->second<<" ";
        }
        cout<<endl;


        //cout << "band_need:" << param.band_need << endl;
        //cout << "cap:" << r.capacity << endl;
        //cout<<"base size"<<r.bases.size()<<endl;
        cout << "costs:" << calcost(r) << " min:" << min << endl;
        if (r.capacity >=cap&&calcost(r) <= costs) {
                cap = r.capacity;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {
                    costs = calcost(r);
                    if (min >= costs) {
                        min = costs;
                        rmin = r;
                        count=count0;
                        bestbase=r.bases;
                    }
                }
        }
        else {
            nodes = tmp;
        }

    }
    return rmin;
}

minMaxResult SAA_800(FIND2 f) {

    vector<int> nodes;
    for (int i = param.edge.size() - 1; i>0; i--) {
        if (param.edge[i].start == param.n_node) {
            nodes.push_back(param.edge[i].end);
        }
    }    
    nodes.pop_back();
    nodes.pop_back();
    //vector<vector<<int>> v;
    //for(int i=param.n_node-1;i>=0;--i){
    //    v.push_back({i,param.node_band});
    //}
    //sort(v.begin(),v.end(),[](vector<int> v1,vector<int> v2){return v1[1]>v2[1];});
    //for(int i=0;i<nodes.size();++i){
    //    nodes[i]=v[i][0];
    //}

    map<int,int> v;
    for(int i=param.n_node-1;i>=0;--i){
        v[i]=param.node_band[i];
    }
    for(int i=0;i<nodes.size();++i){
        getRandIndex_2(v);
    }


    //for(int i=param.n_consume;i>1;--i){
    //  nodes.push_back(rand()%param.n_node);
    //}
    int max_node = nodes.size();
    int costs = INT_MAX;
    int min = costs;
    int cap = 0;
    minMaxResult rmin;
    minMaxResult r = f(param.edge, nodes, param.n_node);
    cap = r.capacity;
    //int avgcost = (r.costs + param.base_cost*max_node) / cap;
    if (cap == param.band_need&&min > calcost(r)) {
        costs = calcost(r);
        min = costs;
        rmin = r;       
    }
    map<int,int> bestbase=r.bases;
    int count0=2000;
    int count=count0*2;
    while (count--) {
         //cout<<"11more --------------------"<<endl;
        //cout<<"jieshushijian:"<<endtime<<" dangqian:"<<clock()<<" haishen:------"<<(clock()-endtime)/CLOCKS_PER_SEC<<endl;

        //cout<<"cishu:"<<cishu++<<"max_node"<<max_node<<"chazhi"<<(param.n_consume-r.bases.size())<<endl;
        if(clock()>endtime)break;
        vector<int> tmp = nodes;
        int change = max_node/2/(param.n_consume-max_node)+2;
        //cout<<"change: "<<change<<endl;
        //change+=rand()%3;
        while(change--) {
            //if(rand()%1000)--change;
            //nodes[rand() % max_node] = (rand()%(param.n_consume-r.bases.size()+1)<2)?-1:rand()%param.n_node;
            int index = rand() % (max_node);
            if(nodes[index] == -1){
                //nodes[index] = rand() % 50 ? rand() % param.n_node:-1;
                int rands=rand() % param.n_node;
                nodes[index]=rand() % (param.n_consume-max_node) ?getRandIndex_2(bestbase):-1;
                continue;
            }
            int base_index = getRandIndex(r.bases);
            //cout<<"more --------------------"<<base_index<<endl;
            for(int i=0;i<nodes.size();i++){
                if(nodes[i] == base_index){
                    index = i;
                    break;
                }
            }
            //cout<<" ----------1----------"<<endl;
            if(r.bases_more[base_index].size()==0){
                nodes[index]=-1;
            }else{
                int index_node = getRandIndex_2(r.bases_more[base_index]);
                nodes[index] = index_node<param.n_node?index_node:-1;     
            }
            //cout<<" ----------2----------"<<endl;            
              

            //index = rand() % 10 ? index: rand_idx;
            //int max=0;
            //for(int next_index :param.next_nodes[nodes[index]]){
            //  if(r.bases[next_index]>max){
            //      max=r.bases[next_index];
            //      nodes[index]=next_index;
            //  }
            //}
            //nodes[index] = rand() % ((3*count)/count0+1) ?nodes[index]: rand() % param.n_node;
        } 
        set<int> nodeset;
        for (int i = 0; i < max_node; ++i) {
            if (nodes[i] != -1) {
                nodeset.insert(nodes[i]);
                nodes[i] = -1;
            }
        }
        vector<int> input(nodeset.size(), 0);
        int k = 0;
        for (auto t = nodeset.begin(); t != nodeset.end(); ++t) {
            nodes[k] = *t;
            input[k++] = *t;
        }
        //printvector(input);
        //cout << "node:";
        //printvector(input);
        r = f(param.edge, input, param.n_node);  
        //cout<<"size: "<<r.bases_more.size()<<endl;
        //int p = 0;
        //for (vector<int> path : r.paths) {
        //  cout << "ÈÝÁ¿" << r.PathCapacity[p++] << "Â·¾¶:";
        //  printvector(path);
        //}
        //cout << "band_need:" << param.band_need << endl;
        //cout << "cap:" << r.capacity << endl;
        cout << "costs:" << calcost(r) << " min:" << min<<"   time:"<<(clock()-endtime)/CLOCKS_PER_SEC << endl;
        //cout << "max_node"<<max_node<<endl;
        if (r.capacity >=cap&&calcost(r) <= costs) {
                cap = r.capacity;
                //avgcost = (r.costs + param.base_cost*input.size()) / cap;
                if (cap == param.band_need) {
                    costs = calcost(r);
                    if (min >= costs) {
                        //cout<<"change"<<min-cost<<
                        min = costs;
                        rmin = r;
                        count=count0;
                        bestbase=r.bases;
                    }
                }
        }
        else {
            nodes = tmp;
            //if (r.capacity < cap&&erand(cap - r.capacity)) {
            //  cap = r.capacity;
            //  costs = INT_MAX;
            //}
            //else {
            //  //cout << "no change--------------------" << endl;
            //  nodes = tmp;
            //}
        }

    }
    return rmin;
}

minMaxResult SAA(FIND2 f) {
    //if(param.n_node > 600)
       return SAA_800(f);
    //else if(param.n_node > 200)
        return SAA_300(f);
    //else
        return SAA_100(f);
}

*/