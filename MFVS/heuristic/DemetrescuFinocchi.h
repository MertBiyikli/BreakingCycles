//
// Created by Mert Biyikli on 10.04.22.
//

#ifndef BREAKINGCYCLES_DEMETRESCUFINOCCHI_H
#define BREAKINGCYCLES_DEMETRESCUFINOCCHI_H

#include "../algorithms/SCC.h"
#include "../algorithms/CycleAlgorithms.h"


class DemetFinocchi{
public:
    Graph G;
    DemetFinocchi(Graph& G);
    FVS Compute();
};

DemetFinocchi::DemetFinocchi(Graph &G) {
    this->G = G;
}

FVS DemetFinocchi::Compute() {
    vector<NodeID> F;

    auto sccc = Tarjan(G).getSCC();
    vector<bool> inc(G.GetNumVertices(), false);
    for (const auto &SCC : sccc) {
        if (SCC.size() > 1) {
            for (const auto &v : SCC) {
                inc[v] = true;
            }
        }
    }

    Cycle C(G, inc);
    vector<int> Creditibility(G.GetNumVertices(), 0);

    for(NodeID u = 0; u<G.GetNumVertices(); u++)
    {
        auto shortestCycles = C.GetShortestCycle(u);
        for(auto v : shortestCycles)
        {
            Creditibility[v]++;
        }
    }

    priority_queue<pair<int, int>> Q;
    for(NodeID u = 0; u<G.GetNumVertices(); u++)
    {
        Q.push(make_pair(Creditibility[u], u));
    }

    /*
    vector<bool> fvs(G.GetNumVertices(), false);
    while(!isAcyclic(G,fvs))
    {
        NodeID v = Q.top().second;
        Q.pop();
        fvs[v]=true;
    }


    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(fvs[v])
        {
            fvs[v]=false;
            if(isAcyclic(G, fvs))
            {
                continue;
            }else{
                fvs[v]=true;
            }
        }
    }*/


    vector<bool> fvs(G.GetNumVertices(), true);
    vector<NodeID> Candidates;
    while(!Q.empty())
    {
        NodeID v = Q.top().second;
        Candidates.push_back(v);
        Q.pop();
    }

    for(auto v : Candidates)
    {
        if(fvs[v])
        {
            fvs[v]=false;
            if(isAcyclic(G, fvs)) {
                continue;
            }else{
                fvs[v]=true;
            }
        }
    }

    for(auto v : Candidates)
    {
        if(fvs[v])
        {
            fvs[v]=false;
            if(isAcyclic(G, fvs)) {
                continue;
            }else{
                fvs[v]=true;
            }
        }
    }

    int FVSSize=0;
    for(auto v : fvs)
    {
        FVSSize+=v;
    }
    return make_pair(inc, FVSSize);
}


#endif //BREAKINGCYCLES_DEMETRESCUFINOCCHI_H
