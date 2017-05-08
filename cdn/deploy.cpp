#include "deploy.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "DJS.h"
#include "BFS.h"
#include "Ford.h"
#include "util.h"
#include <ctime>
#include <time.h>
#include <algorithm>
#include "SAA.h"
#include <thread>
#include <unistd.h>
#include "load_param.h"





void solve(char* lines[],char * filename) {
    //srand(time(0));
    load_param_base(lines);
    minMaxResult mr = SAA(minMax);
    ourResult r = parseResidual(mr);

    if(r.n_path<param.n_consume||r.n_path>1000000)//fall to solve
    {
        return;
    }

    stringstream ss;
    ss<<r.n_path;
    ss<<"\n";
    for(vector<int> path:r.paths){
        ss<<"\n";
        ss<<path[0];
        for(int k=1;k<path.size();++k){
            ss<<' '<<path[k];
        }
    }
    string tmp=ss.str();
    //cout<<tmp<<endl;
    //printf("line num is :%d \n", tmp[tmp.size()-1]);
    //printf("last:%d",tmp[tmp.size()-1]);

    const char* topo_file=tmp.c_str();
    //char *topo_file=(char *)"ss";

    // 需要输出的内容
    
    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    write_result(topo_file, filename);

    
}
/*
void timelimit(unsigned int s) {
    sleep(s);
    shouldend = true;
    return;
}


void main2(char* lines[],char * filename)
{
    thread t[2];
    t[0] = thread(timelimit, 10000);
    t[1] = thread(solve,lines,filename);
    t[0].join();
    t[1].join();
    cout << "end";
}
*/
//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    int sec=88;
    clock_t start=clock();
    endtime=start+sec*CLOCKS_PER_SEC;
    /*int k=10000000;
    cout<<"time:"<<(clock()-start)/CLOCKS_PER_SEC<<endl; 
    while(1){
        if((clock()-start)/CLOCKS_PER_SEC>13)break;
    }
    cout<<(clock()>endtime)<<endl;
    cout<<"time:"<<(clock()-start)/CLOCKS_PER_SEC<<endl; 
    while(1);*/

/*
    struct timeb rawtime;
    struct tm * timeinfo;
    ftime(&rawtime);
    timeinfo = localtime(&rawtime.time);

    int ms = rawtime.millitm;
    unsigned long s = rawtime.time;
    cout<<"s:"<<s<<"ms"<<ms;
*/
    solve(topo,filename);   

    /*ms = rawtime.millitm;
    s = rawtime.time;
    cout<<"s:"<<s<<"ms"<<ms;*/
    cout<<"time:"<<(clock()-start)/CLOCKS_PER_SEC<<endl; 
}
