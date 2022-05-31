//
// Created by Mert Biyikli on 30.04.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALSCCSORTING_H
#define BREAKINGCYCLES_MAKEMINIMALSCCSORTING_H

#include "../include/graph.h"


class MakeMinimalSorting{
public:
    Graph H;
    MakeMinimalSorting(Graph& G_);
    vector<NodeID> Compute();
};

MakeMinimalSorting::MakeMinimalSorting(Graph &G) {
    this->H=G;
}

vector<NodeID> MakeMinimalSorting::Compute() {
    Graph G = H;

    // one could call the reduce method to remove all 'unnecessary' vertices
    // but since, we only consider the SCC's with size larger than 1, we already ignoring 'unnecessary' vertices
    //G.reduce(true, true, true, true, true, true, true, true, true);
    auto scc = G.getstronglyconnectedcomponents();
    priority_queue<pair<int,NodeID> > Candidate; // storing the elements of the PQ according to their degree
    FVS fvs = make_pair(vector<bool>(G.currentMax, false) ,0);
    for(const auto& S : scc)
    {
        if(S.size()>1)
        {
            for(NodeID v : S)
            {
                fvs.first[v]=true;
                fvs.second++;
                Candidate.push(make_pair(G.TotalDegree(v), v));
            }
        }
    }

    while (!Candidate.empty())
    {
        NodeID v = (Candidate.top()).second;
        Candidate.pop();
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            if(isAcyclic(H, fvs.first))
            {
                fvs.second--;
            }else{
                fvs.first[v]=true;
            }
        }
    }
    vector<NodeID> sol;
    for(int i = 0; i<H.GetNumVertices(); i++)
    {
        if(fvs.first[i])
        {
            sol.push_back(i);
        }
    }
    return sol;
}

#endif //BREAKINGCYCLES_MAKEMINIMALSCCSORTING_H
