//
// Created by Mert Biyikli on 21.04.22.
//

#ifndef BREAKINGCYCLES_CYCLEALGORITHMS_H
#define BREAKINGCYCLES_CYCLEALGORITHMS_H



#include "../include/graph.h"
#include "../algorithms/SCC.h"
#include <list>
#include <queue>

class Cycle{
    Graph G;
    vector<NodeID> _shortestCycle(Graph& G, NodeID vertex, int maxLength);
public:
    Cycle(Graph& G);
    bool isAcyclic(Graph& G) const;
    vector<NodeID> shortestCycles();
};

Cycle::Cycle(Graph &G) {this->G =G;};

vector<NodeID> Cycle::_shortestCycle(Graph& G, NodeID vertex, int maxLength) {
    queue< list<int> > paths;
    list<int> path;
    path.push_back(vertex);
    paths.push(path);
    bool cycleFound = false;
    while (!cycleFound && !paths.empty()) {
        path = paths.front();
        paths.pop();
        cycleFound = int(path.size()) > 1 && path.front() == path.back();
        if (!cycleFound && int(path.size()) - 1 < maxLength) {
            vector<int> outN = G.OutGoing(path.back());
            for (vector<int>::const_iterator it = outN.begin();
                 it != outN.end(); ++it) {
                path.push_back(*it);
                paths.push(path);
                path.pop_back();
            }
        }
    }
    vector<int> cycle(int(path.size()));
    copy(path.begin(), path.end(), cycle.begin());
    return cycle;
}

vector<NodeID> Cycle::shortestCycles() {
    Graph h=G;
    if (isAcyclic(h)) {
        throw "No shortest cycle ! This graph is acyclic";
    } else {
        int n;
        do {
            n = h.GetNumVertices();
            h.In0();
            h.Out0();
        } while (n != h.GetNumVertices());
        vector<int> shortestCycle;
        vector<int> currentCycle;
        int maxLength = h.GetNumVertices();
        for (set<int>::iterator it = h.GetVertices().begin(); it != h.GetVertices().end(); ++it) {
            currentCycle = _shortestCycle(h, *it, maxLength);
            if (int(currentCycle.size() - 1)  < maxLength) {
                shortestCycle = currentCycle;
                maxLength = int(currentCycle.size()) - 1;
            }
        }
        return shortestCycle;
    }
}

bool Cycle::isAcyclic(Graph& G) const {
    bool hasCycle = false;
    for(auto S : Tarjan(G).getSCC())
    {
        cout << S.size() << " ";
        cout << "ok " << endl;
        if(S.size()!=1)
        {
            hasCycle = true;
        }
    }
    return !hasCycle;
}

#endif //BREAKINGCYCLES_CYCLEALGORITHMS_H
