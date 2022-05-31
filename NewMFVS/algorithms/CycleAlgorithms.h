//
// Created by Mert Biyikli on 21.04.22.
//

#ifndef BREAKINGCYCLES_CYCLEALGORITHMS_H
#define BREAKINGCYCLES_CYCLEALGORITHMS_H



#include "../include/graph.h"
#include "../algorithms/SCC.h"
#include <map>
#include <queue>

class Cycle{
    Graph G;
public:
    Cycle(Graph& G);
    vector<vector<NodeID> > GetDisjointCycle(Graph& G);
    vector<NodeID> GetShortestCycle(Graph& G, NodeID v);
};

Cycle::Cycle(Graph &G) {this->G =G;};


/*
 * Computing the shortest cycle of a given graph
 */
vector<NodeID> Cycle::GetShortestCycle(Graph &G_, NodeID s) {
    Graph h = G_;
    if(!h.hasVertex(s))
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
        for (NodeID v : h.OutGoing(u)) {
            if(h.hasVertex(v)) {
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

/*
 * To compute the amount of disjoint cycles in a graph. Two cycle are disjoint, if they do not share any common
 * vertex
 */
vector<vector<NodeID>> Cycle::GetDisjointCycle(Graph &G_) {
    Graph H =G_;
    vector<vector<NodeID> > DisjointCyles;
    for(NodeID v = 0; v<H.currentMax; v++)
    {
        if(H.hasVertex(v))
        {
            auto sCycle = GetShortestCycle(H, v);
            if(!sCycle.empty()) {
                DisjointCyles.push_back(sCycle);
                H.DeleteVertices(sCycle);
                H.reduce();
            }
        }
    }
    return DisjointCyles;
}

#endif //BREAKINGCYCLES_CYCLEALGORITHMS_H
