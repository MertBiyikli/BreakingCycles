//
// Created by Mert Biyikli on 31.03.22.
//

#ifndef BREAKINGCYCLES_SIMPLEBRANCHBOUND_H
#define BREAKINGCYCLES_SIMPLEBRANCHBOUND_H

#include "../include/graph.h"
#include "../heuristic/MakeMinimalWithSorting.h"
#include "../algorithms/SCC.h"

#define REMOVED -1
#define UNTOUCHED 0
#define OPEN 1
#define CLOSED 2


class BranchBound{
public:
    Graph G_;
    int lowerbound, upperbound;
    BranchBound(Graph& G);
    FVS BandB(Graph& G, bool print);
    void heuristic(Graph& G, vector<NodeID>& best, int& bestCard);
    bool coreBfvsB(Graph& G, vector<NodeID>& inc, vector<NodeID>& best, int& bestCard, bool print);
    bool findCycle(Graph& G, vector<NodeID>& inc, vector<NodeID>& cycle);
    bool DFS_FindingCycle(Graph& G, NodeID v, vector<NodeID>& parent, vector<int>& status, int& c);
    void lowerBound(Graph& G, vector<NodeID>& best, int& bestCard);
};

BranchBound::BranchBound(Graph &G)
{
    this->G_ = G;
    this->lowerbound = Tarjan(G).getSCC().size();
}

FVS BranchBound::BandB(Graph &G_, bool print )
{
    vector<NodeID> removed;
    vector<NodeID> best;
    int bestcard;
    this->heuristic(G_, best, bestcard);
    this->lowerBound(G_, best, bestcard);
    this->coreBfvsB(G_, removed, best, bestcard, print);
    FVS fvs = make_pair(vector<bool>(G_.GetNumVertices(), false), 0);
    for(auto it : best)
    {
        fvs.first[it]=true;
        fvs.second++;
    }
    return fvs;
}

void BranchBound::heuristic(Graph &G, vector<NodeID> &best, int& bestCard) {/*
    vector<NodeID> removed;
    vector<NodeID> cycle;
    while(this->findCycle(G, removed, cycle))
    {
        removed.push_back(cycle[0]);
    }
    best = removed;
    bestCard = removed.size();*/
    MakeMinimalWithSorting M(G);
    auto fvs = M.Compute();
    for(NodeID i = 0; i<fvs.first.size(); i++)
    {
        if(fvs.first[i])
        {
            best.push_back(i);
        }
    }
    this->upperbound = fvs.second;
    bestCard = fvs.second;
}


void BranchBound::lowerBound(Graph &G, vector<NodeID> &best, int &bestCard)
{
    Graph g = G;
    vector<bool> tmp(g.GetNumVertices(), true);
    size_t lb = 0;
    while (true) {
        auto cycles = Cycle(g, tmp).GetCycles();
        if (cycles.empty())
            break;
        for (auto& cycle : cycles) {
            bool doneAlready = false;
            for (NodeID u : cycle) {
                if (!tmp[u]) {
                    doneAlready = true;
                    break;
                }
            }
            if (doneAlready)
                continue;
            for (NodeID u : cycle){
                tmp[u]=false;
            }
            ++lb;
        }
    }
    this->lowerbound = lb;
}

bool BranchBound::findCycle(Graph &G, vector<NodeID> &inc, vector<NodeID> &cycle)
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

bool BranchBound::DFS_FindingCycle(Graph &G, NodeID v, vector<NodeID> &parent, vector<int> &status, int &c)
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

int ctr = 0;

bool BranchBound::coreBfvsB(Graph &G, vector<NodeID> &inc, vector<int> &best, int& bestCard, bool print) {
    if(print)
    {
        cout << "---------------------------" << endl;
        cout << "bestCard: " << bestCard << endl;
        cout << "lowerbound: " << lowerbound << endl;
        cout << "upperbound: " << upperbound << endl;
        cout << "---------------------------" << endl;
    }
    vector<NodeID> cycle;
    bool timeout = false;
    if (inc.size() == bestCard)
    {
        return false;
    }
    if(this->findCycle(G, inc, cycle))
    {
        for(auto& node : cycle)
        {
            inc.push_back(node);
            timeout = this->coreBfvsB(G, inc, best, bestCard, print);
            inc.pop_back();
            if(timeout)
            {
                return false;
            }
        }
    }else{
        best = inc;
        bestCard = inc.size();
    }
    return false;
}


#endif //BREAKINGCYCLES_SIMPLEBRANCHBOUND_H
