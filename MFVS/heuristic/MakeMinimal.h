//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMAL_H
#define BREAKINGCYCLES_MAKEMINIMAL_H

#include "../include/graph.h"

class MakeMinimal{
public:
    Graph G;
    MakeMinimal(Graph& G);
    pair<vector<bool>, int> fvs;
    pair<vector<bool>, int> Compute();
};

MakeMinimal::MakeMinimal(Graph &G)
{
    this->G = G;
}

pair<vector<bool>, int> MakeMinimal::Compute()
{
    this->fvs.first.resize(G.GetNumVertices(), true);
    this->fvs.second=G.GetNumVertices();
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        fvs.first[v]=false;
        if(isAcyclic(G, fvs.first))
        {
            fvs.second--;
        }else{
            fvs.first[v]=true;
        }
    }
    return this->fvs;
}

#endif //BREAKINGCYCLES_MAKEMINIMAL_H
