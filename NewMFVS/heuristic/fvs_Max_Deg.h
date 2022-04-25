//
// Created by Mert Biyikli on 26.04.22.
//

#ifndef BREAKINGCYCLES_FVS_MAX_DEG_H
#define BREAKINGCYCLES_FVS_MAX_DEG_H

#include "../include/graph.h"

class Max_deg{
public:
    Graph G;
    Max_deg(Graph& G);
    vector<NodeID> Compute();
};

Max_deg::Max_deg(Graph &G) {
    this->G = G;
}

vector<NodeID> Max_deg::Compute() {
    vector<NodeID> fvs;
    Graph H = G;
    //auto scc = H.getstronglyconnectedcomponents();
    while(!H.isAcyclic())
    {
        NodeID v = H.MaxTotalDegree().first;
        if(H.hasVertex(v)) {
            H.DeleteVertex(v);
            H.reduce(true, true, true, false, true, false, false,false, false);
            fvs.push_back(v);
        }
    }
    return fvs;
}


#endif //BREAKINGCYCLES_FVS_MAX_DEG_H
