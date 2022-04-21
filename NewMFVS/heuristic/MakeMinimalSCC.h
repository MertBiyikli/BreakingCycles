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
};

MakeMinimal::MakeMinimal(Graph &G) {
    this->G_ =G;
}

vector<NodeID> MakeMinimal::Compute() {
    Graph G = G_;
    auto scc = Tarjan(G).getSCC();
    queue<NodeID> Candidate;
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false) ,0);
    G.reduce(true);
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

    cout << "before the while loop" << endl;
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

#endif //BREAKINGCYCLES_MAKEMINIMALSCC_H
