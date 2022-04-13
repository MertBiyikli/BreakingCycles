//
// Created by Mert Biyikli on 22.03.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALSCCWITHSORTING_H
#define BREAKINGCYCLES_MAKEMINIMALSCCWITHSORTING_H

#include "../algorithms/SCC.h"
#include "../include/subgraph.h"


class MakeMinimalSCCWithSorting{
public:
    Graph G;
    MakeMinimalSCCWithSorting(Graph& G);
    FVS Compute();
    FVS fvs;
};

MakeMinimalSCCWithSorting::MakeMinimalSCCWithSorting(Graph &G)
{
    this->G =G;
}


// C++ implementation of the
// priority queue in which elements
// are sorted by the second element

#include <iostream>
#include <queue>
#include <vector>

using namespace std;


struct myComp {
    constexpr bool operator()(
            pair<NodeID , int> const& a,
            pair<NodeID , int> const& b)
    const noexcept
    {
        return a.second > b.second;
    }
};

FVS MakeMinimalSCCWithSorting::Compute() {
    Tarjan T(G);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    //vector<NodeID> Candidate;
    priority_queue<pair<NodeID ,int>, vector<pair<NodeID ,int>>, myComp> Q;

    vector<int> indegree(G.GetNumVertices(), 0);
    vector<int> outdegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(NodeID u: G.neighbors(v))
        {
            indegree[u]++;
            outdegree[v]++;
        }
    }
    auto SCC = T.getSCC();
    for (auto scc : SCC)
    {
        if(scc.size()>1)
        {
            for(auto v : scc)
            {
                Q.push(make_pair(v, indegree[v]+outdegree[v]));
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
    while(!Q.empty())
    {
        auto v_pair = Q.top();
        Q.pop();
        NodeID v = v_pair.first;
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


#endif //BREAKINGCYCLES_MAKEMINIMALSCCWITHSORTING_H
