//
// Created by Mert Biyikli on 01.04.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALWITHREDUCTION_H
#define BREAKINGCYCLES_MAKEMINIMALWITHREDUCTION_H

#include "../include/graph.h"
#include "../include/subgraph.h"


/// This is not working!!!!!!!!!


class MakeMinimalRedSort{
public:
    Graph G;
    MakeMinimalRedSort(Graph& G);
    vector<bool> allowed;
    Subgraph Reduce();
    FVS Compute();
};

MakeMinimalRedSort::MakeMinimalRedSort(Graph &G) {
    this->G = G;
    this->allowed.resize(G.GetNumVertices(), true);
}


Subgraph MakeMinimalRedSort::Reduce()
{
    vector<int> indegree(G.GetNumVertices(),0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(NodeID u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 || G.GetDegree(v)==0)
        {
            this->allowed[v]=false;
        }
    }
    return Subgraph(G, allowed);
}

FVS MakeMinimalRedSort::Compute()
{
    Tarjan T(G);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false),0);
    vector<NodeID> Candidate;
    auto SCC = T.getSCC();
    for(auto S : SCC)
    {
        vector<bool> inc(G.GetNumVertices(), false);
        int SCC_numberNodes = 0;
        for(auto it : S)
        {
            inc[it]=true;
            SCC_numberNodes++;
        }

        auto H = Subgraph(G, inc);
        FVS fvs_h = make_pair(vector<bool>(H.GetSubGraph().first.GetNumVertices(), true), H.GetSubGraph().first.GetNumVertices());
        for(NodeID v = 0; v<H.GetSubGraph().first.GetNumVertices(); v++)
        {
            if(fvs_h.first[v])
            {
                fvs_h.first[v]=false;
                if(isAcyclic(H.GetSubGraph().first, fvs_h.first))
                {
                    fvs_h.second--;
                }else{
                    fvs_h.first[v]=true;
                }
            }
        }
        for(NodeID v = 0; v<fvs_h.first.size(); v++)
        {
            if(fvs_h.first[v])
            {
                NodeID tmP;
                for(auto i : H.M)
                {
                    if(i.second==v)
                    {
                        tmP=i.first;
                    }
                }
                fvs.first[tmP]=true;
                fvs.second++;
            }
        }
    }
    return fvs;
}

#endif //BREAKINGCYCLES_MAKEMINIMALWITHREDUCTION_H
