//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_GREEDY_H
#define BREAKINGCYCLES_GREEDY_H

#include "../algorithms/SCC.h"

class Greedy{
public:
    //SCC S;
    Graph G;
    Greedy(Graph& G_);
    FVS GreedyOne();
    FVS GreedyTwo();
    FVS GreedyThree();
};

Greedy::Greedy(Graph &G_)
{
    this->G = G_;
}


pair<vector<bool>, int> Greedy::GreedyOne()
{
    int VertexCtr = G.GetNumVertices();
    vector<bool> FVS;
    FVS.resize(G.GetNumVertices(), true);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(G.GetDegree(v)==0)
        {
            FVS[v]= false;
            VertexCtr--;
        }
    }
    return make_pair(FVS, VertexCtr);
}

pair<vector<bool>, int> Greedy::GreedyTwo() {
    int VertexCtr = G.GetNumVertices();
    vector<bool> FVS(G.GetNumVertices(), true);
    vector<int> indegree(G.GetNumVertices() + 1, 0);
    for (NodeID v = 0; v < G.GetNumVertices(); v++) {
        for (auto tmp = 0; tmp < G.graph[v].size(); tmp++) {
            NodeID u = G.graph[v][tmp];
            indegree.at(u)++;
        }
    }
    for (NodeID v = 0; v < G.GetNumVertices(); v++) {
        if (G.GetDegree(v) == 0 || indegree.at(v) == 0) {
            FVS[v] = false;
            VertexCtr--;
        }
    }
    return make_pair(FVS, VertexCtr);
}

FVS Greedy::GreedyThree() {
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    vector<int> indegree(G.GetNumVertices() + 1, 0);
    for (NodeID v = 0; v < G.GetNumVertices(); v++) {
        for (auto tmp = 0; tmp < G.graph[v].size(); tmp++) {
            NodeID u = G.graph[v][tmp];
            indegree.at(u)++;
        }
    }
    for (NodeID v = 0; v < G.GetNumVertices(); v++) {
        if (G.GetDegree(v) == 0 || indegree.at(v) == 0) {
            fvs.first[v] = false;
            fvs.second--;
        }
        if(G.GetDegree(v)==1 && indegree[v]==1)
        {
            if(fvs.first[G.graph[v][0]])
            fvs.first[v]=false;
            fvs.second--;
        }
    }
    return fvs;
}



#endif //BREAKINGCYCLES_GREEDY_H