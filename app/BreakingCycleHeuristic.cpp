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

int main(int argc, char** argv)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    ios_base::sync_with_stdio(false);
    if (argc < 2) {
        std::cerr << "usage: verifer [graph file] [solution file]" << std::endl;
        return -1;
    }
    Graph G;
    string GraphName = argv[1];
    int error = ReadGraph(GraphName, G);
    if(error!=0)
    {
        std::cerr << "Problems accured!" << std::endl;
        return 1;
    }
    MakeMinimalSorting M(G);
    auto fvs = M.Compute();
    Graph H = G;
    H.DeleteVertices(fvs);
    if(H.isAcyclic())
    {
        cout << "The graph is acyclic and the FVS has size " << fvs.size() << endl;
    }else{
        cout << "the graph still contains cycles" << endl;
    }

    ofstream SolutionFile;
    SolutionFile.open(GraphName+"_solution.txt");
    for(const auto& v : fvs)
    {
        SolutionFile << v << endl;
    }
    SolutionFile.close();
    return 0;
}

