//
// Created by Mert Biyikli on 17.03.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALSCC_H
#define BREAKINGCYCLES_MAKEMINIMALSCC_H

#include "../algorithms/SCC.h"
#include "../include/subgraph.h"
#include <random>

class MakeMinimalSCC{
public:
    Graph G;
    MakeMinimalSCC(Graph& G);
    FVS Compute();
    FVS fvs;
};

MakeMinimalSCC::MakeMinimalSCC(Graph &G)
{
    this->G =G;
}


FVS MakeMinimalSCC::Compute() {
    Tarjan T(G);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    auto SCC = T.getSCC();
    for (auto scc : SCC)
    {
        if(scc.size()>1)
        {
            for(auto v : scc)
            {
                fvs.first[v]=true;
                fvs.second++;
            }
        }
    }
    for(auto scc : SCC)
    {
        if(scc.size()>1)
        {

        }
    }
    return fvs;
}
#endif //BREAKINGCYCLES_MAKEMINIMALSCC_H
