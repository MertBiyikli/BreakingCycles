//
// Created by Mert Biyikli on 31.03.22.
//

#ifndef BREAKINGCYCLES_FINDCYCLE_H
#define BREAKINGCYCLES_FINDCYCLE_H

#define REMOVED -1
#define UNTOUCHED 0
#define OPEN 1
#define CLOSED 2

#include "../include/graph.h"

class FindCycle{
public:
    Graph G;
    FindCycle(Graph& G);
    FVS Compute();
    bool findCycle(Graph& G, vector<int>& inc, vector<NodeID>& cycle);
    void heuristic(Graph &G, vector<NodeID> &best, int& bestCard);
    bool DFS_FindingCycle(Graph& G, NodeID v, vector<NodeID>& parent, vector<int>& status, int& c);
};

FindCycle::FindCycle(Graph &G) {
    this->G =G;
}

FVS FindCycle::Compute()
{
    vector<NodeID> removed;
    vector<NodeID> best;
    int bestcard;
    this->heuristic(this->G, best, bestcard);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(int it = 0; it<best.size(); it++)
    {
        //cout << fvs.first.size() << " "<<  best[it] << endl;
        fvs.first.at(best[it])=true;
        fvs.second++;
    }
    return fvs;
}

void FindCycle::heuristic(Graph &G, vector<NodeID> &best, int &bestCard)
{
    vector<NodeID> removed;
    vector<NodeID> cycle;
    while(this->findCycle(G, removed, cycle))
    {
        removed.push_back(cycle[0]);
    }
    best = removed;
    bestCard = removed.size();
}


bool FindCycle::findCycle(Graph &G, vector<int> &inc, vector<NodeID> &cycle)
{
    vector<NodeID> parent(G.GetNumVertices());
    vector<int> status(G.GetNumVertices(), UNTOUCHED);
    for(auto& it : inc)
    {
        status[it]=REMOVED;
    }
    int c = -1;
    cycle.clear();
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(status[v]==UNTOUCHED)
        {
            if(this->DFS_FindingCycle(G, v, parent, status, c))
            {
                break;
            }
        }
    }
    if(c != -1)
    {
        cycle.push_back(c);
        int pred = parent[c];
        while(pred!= c)
        {
            cycle.push_back(pred);
            pred = parent[pred];
        }
        return true;
    }else{
        return false;
    }
}


bool FindCycle::DFS_FindingCycle(Graph &G, NodeID v, vector<NodeID> &parent, vector<int> &status, int &c)
{
    status[v]=OPEN;
    for(EdgeID e = 0; e < G.graph[v].size(); e++)
    {
        NodeID target = G.graph[v][e];
        if(status[target]==UNTOUCHED)
        {
            parent[target]= v;
            if(this->DFS_FindingCycle(G, target, parent, status, c))
            {
                return true;
            }
        } else if(status[target]==OPEN)
        {
            parent[target]=v;
            c = target;
            return true; // <- there is a cycle
        }
    }
    status[v]=CLOSED;
    return false;
}

#endif //BREAKINGCYCLES_FINDCYCLE_H
