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
    void reduce();
};

MakeMinimal::MakeMinimal(Graph &G)
{
    this->G = G;
}

void MakeMinimal::reduce()
{
    vector<int> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(auto u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 || G.GetDegree(v))
        {
            this->fvs.first[v]=false;
            this->fvs.second--;
        }
    }
}

pair<vector<bool>, int> MakeMinimal::Compute()
{
    this->fvs.first.resize(G.GetNumVertices(), true);
    this->fvs.second=G.GetNumVertices();
    //this->reduce();
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(fvs.first[v]) {
            this->fvs.first[v] = false;
            if (isAcyclic(G, fvs.first)) {
                this->fvs.first[v] = false;
                this->fvs.second--;
            } else {
                this->fvs.first[v] = true;
            }
        }
    }
    return this->fvs;
}

#endif //BREAKINGCYCLES_MAKEMINIMAL_H
