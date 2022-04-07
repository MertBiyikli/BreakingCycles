//
// Created by Mert Biyikli on 07.04.22.
//

#ifndef BREAKINGCYCLES_CYCLEALGORITHMS_H
#define BREAKINGCYCLES_CYCLEALGORITHMS_H

#include "../include/graph.h"
#include <map>

class Cycle{
    vector<NodeID> shortestCycle;
    Graph G;
    vector<bool> inc;
public:
    Cycle(Graph& G,const  vector<bool>& inc);
    Cycle(Graph& G);
    vector<NodeID> GetShortestCycle(NodeID s);
    vector<vector<NodeID> > GetCycles();
};

Cycle::Cycle(Graph &G)
{
    this->G = G;
    this->inc.resize(G.GetNumVertices(), true);
}

Cycle::Cycle(Graph &G, const vector<bool> &inc)
{
    this->G = G;
    this->inc = inc;
}

vector<NodeID> Cycle::GetShortestCycle(NodeID s)
{
    if(!inc[s])
    {
        return {};
    }
    vector<NodeID> queue;
    queue.push_back(s);
    map<NodeID , NodeID> pred;
    pred[s] = -1;
    while (!queue.empty()) {
        NodeID u = queue.back();
        queue.pop_back();
        for (NodeID v : G.neighbors(u)) {
            if(inc[v]) {
                if (pred.find(v) == pred.end()) {
                    queue.push_back(v);
                    pred[v] = u;
                } else if (v != pred[u]) {
                    std::vector<NodeID> cu, cv;
                    for (NodeID w = u; w != -1; w = pred[w])
                        cu.push_back(w);
                    for (NodeID w = v; w != -1; w = pred[w])
                        cv.push_back(w);
                    while (cu.size() > 1 && cv.size() > 1 && cu[cu.size() - 2] == cv[cv.size() - 2]) {
                        cu.pop_back();
                        cv.pop_back();
                    }
                    cv.pop_back();
                    for (NodeID u : cv)
                        cu.push_back(u);
                    return cu;
                }
            }
        }
    }
    return {};
}


vector<vector<NodeID>> Cycle::GetCycles()
{
    vector<vector<NodeID>> shortestCycles;
    for (NodeID u = 0; u<G.GetNumVertices(); u++) {
        if (inc[u]) {
            auto cycle = this->GetShortestCycle(u);
            if (cycle.empty())
                continue;
            if (shortestCycles.empty() || cycle.size() < shortestCycles[0].size())
                shortestCycles = {cycle};
            else if (cycle.size() == shortestCycles[0].size())
                shortestCycles.push_back(cycle);
        }
    }
    return shortestCycles;
}

#endif //BREAKINGCYCLES_CYCLEALGORITHMS_H
