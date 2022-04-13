//
// Created by Mert Biyikli on 07.04.22.
//

#ifndef BREAKINGCYCLES_CYCLEHEURISTIC_H
#define BREAKINGCYCLES_CYCLEHEURISTIC_H

#include "../include/graph.h"
#include "../algorithms/CycleAlgorithms.h"
#include "../algorithms/SCC.h"

class CycleHeuristic
{
    Graph G;
public:
    CycleHeuristic(Graph& G);
    vector<NodeID> _Compute_();
    FVS Compute();
};


CycleHeuristic::CycleHeuristic(Graph &G)
{
    this->G = G;
}

vector<NodeID> CycleHeuristic::_Compute_()
{
    Graph H = this->G;
    vector<NodeID> fvs;
    vector<bool> inc(H.GetNumVertices(), true);
    Cycle C(G);
    while(C.GetCycles().size()!=0)
    {
        C = Cycle(G, inc);
        auto cycles = C.GetCycles();
        if(cycles.empty())
            return fvs;
        for(auto& cycle : cycles)
        {
            int maxDeg = 0;
            NodeID best = -1;
            bool doneAlready = false;
            for(NodeID u : cycle)
            {
                if(!inc[u])
                {
                    doneAlready=true;
                    break;
                }
                if(H.GetDegree(u, inc)>maxDeg)
                {
                    maxDeg = H.GetDegree(u, inc);
                    best = u;
                }
            }
            if(doneAlready)
            {
                continue;
            }
            fvs.push_back(best);
            inc[best]=false;
        }
    }
    return fvs;
}


FVS CycleHeuristic::Compute()
{
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    auto scc = Tarjan(G).getSCC();
    //priority_queue<pair<int, NodeID> > candidates;
    vector<pair<NodeID, int> > candidates;
    vector<bool> inc(G.GetNumVertices(), false);
    for(const auto& S : scc)
    {
        if(S.size()>1) {
            for (const auto& v : S) {
                inc[v]=true;
                fvs.first[v]=true;
                fvs.second++;
            }
        }
    }

    auto cycles = Cycle(G, inc).GetCycles();
    vector<int> cycleCredibility(G.GetNumVertices(), 0);
    for(auto& cycle : cycles)
    {
        for(auto v : cycle)
        {
            cycleCredibility[v]++;
        }
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        candidates.push_back(make_pair(v, cycleCredibility[v]));//make_pair(cycleCredibility[v], v));
    }

    std::sort(candidates.begin(), candidates.end(), [](pair<NodeID, int>& a, pair<NodeID, int>& b){return a.second > b.second;});

    //for(int i = 0; i<candidates.size(); i++)
    for(auto v_ : candidates)
    {/*
        auto V = candidates.top();
        candidates.pop();
        NodeID v = V.second;*/
        NodeID v = v_.first;
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            if(isAcyclic(G, fvs.first))
            {
                fvs.second--;
            }else{
                fvs.first[v]=true;
            }
        }
    }
    return fvs;
}

#endif //BREAKINGCYCLES_CYCLEHEURISTIC_H
