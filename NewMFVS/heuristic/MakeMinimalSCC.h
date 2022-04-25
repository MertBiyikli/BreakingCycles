//
// Created by Mert Biyikli on 22.04.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALSCC_H
#define BREAKINGCYCLES_MAKEMINIMALSCC_H

#include "../include/graph.h"
#include "../algorithms/SCC.h"

class MakeMinimal{
    Graph G_;
public:
    MakeMinimal(Graph& G);
    vector<NodeID> Compute();
    vector<NodeID> ReducedCompute();
};

MakeMinimal::MakeMinimal(Graph &G) {
    this->G_ =G;
}

vector<NodeID> MakeMinimal::Compute() {
    Graph G = G_;
    //G.reduce(true, true, true, true, true, true, true, true, true);
    auto scc = G.getstronglyconnectedcomponents();
    queue<NodeID> Candidate;
    FVS fvs = make_pair(vector<bool>(G.currentMax, false) ,0);
    for(auto S : scc)
    {
        if(S.size()>1)
        {
            for(NodeID v : S)
            {
                fvs.first[v]=true;
                fvs.second++;
                Candidate.push(v);
            }
        }
    }

    while (!Candidate.empty())
    {
        NodeID v = Candidate.front();
        Candidate.pop();
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            if(isAcyclic(G_, fvs.first))
            {
                fvs.second--;
            }else{
                fvs.first[v]=true;
            }
        }
    }
    vector<NodeID> sol;
    for(int i = 0; i<G_.GetNumVertices(); i++)
    {
        if(fvs.first[i])
        {
            sol.push_back(i);
        }
    }
    return sol;
}

vector<NodeID> MakeMinimal::ReducedCompute() {
    Graph G = G_;
    //G.reduce(true, true, true, true, true, true, true, true, true);
    Graph h = G;
    auto removal = h.reduce();
    auto scc = G.getstronglyconnectedcomponents();
    queue<NodeID> Candidate;
    FVS fvs = make_pair(vector<bool>(G.currentMax, false) ,0);
    for(auto S : scc)
    {
        if(S.size()>1)
        {
            for(NodeID v : S)
            {
                fvs.first[v]=true;
                fvs.second++;
                Candidate.push(v);
            }
        }
    }

    while (!Candidate.empty())
    {
        NodeID v = Candidate.front();
        Candidate.pop();
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            if(isAcyclic(G_, fvs.first))
            {
                fvs.second--;
            }else{
                fvs.first[v]=true;
            }
        }
    }
    for(NodeID v : removal)
    {
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            fvs.second--;
        }
    }
    vector<NodeID> sol;
    for(int i = 0; i<G_.GetNumVertices(); i++)
    {
        if(fvs.first[i])
        {
            sol.push_back(i);
        }
    }
    return sol;
}

#endif //BREAKINGCYCLES_MAKEMINIMALSCC_H
