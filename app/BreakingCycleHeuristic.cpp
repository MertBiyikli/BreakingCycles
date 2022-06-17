//
// Created by Mert Biyikli on 31.05.22.
//
#include <iostream>
#include "signal.h"
#include "../NewMFVS/include/graph.h"
#include "../NewMFVS/io/graph_io.h"
#include "../NewMFVS/heuristic/MakeMinimalSCCSorting.h"
#include "fstream"

using namespace std;

volatile sig_atomic_t tle = 0;
int n;
float worker;

void term(int signum)
{
    tle = 1;
}

int main()
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    ios_base::sync_with_stdio(false);
    Graph G;
    int error = ReadGraph(G);
    if(error!=0)
    {
        std::cerr << "Problems accured!" << std::endl;
        return 1;
    }
    MakeMinimalSorting M(G);
    auto fvs = M.Compute();
    for(const auto& v : fvs)
    {
        cout << v << endl;
    }
    return 0;
}

