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
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    vector<NodeID> Candidate;
    auto SCC = T.getSCC();
    for (const auto& scc : SCC)
    {
        if(scc.size()>1)
        {
            for(auto v : scc)
            {
                Candidate.push_back(v);
            }
        }
        if(scc.size()==1)
        {
            for(auto v : scc)
            {
                fvs.first[v]=false;
                fvs.second--;
            }
        }
    }
    for(const auto& v : Candidate)
    {
        if(fvs.first.at(v)) {
            fvs.first[v] = false;
            if (isAcyclic(G, fvs.first)) {
                fvs.first[v] = false;
                fvs.second--;
            } else {
                fvs.first[v] = true;
            }
        }
    }
    return fvs;
}
#endif //BREAKINGCYCLES_MAKEMINIMALSCC_H
