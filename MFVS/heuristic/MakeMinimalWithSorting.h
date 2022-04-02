//
// Created by Mert Biyikli on 17.03.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALWITHSORTING_H
#define BREAKINGCYCLES_MAKEMINIMALWITHSORTING_H

#include "../include/graph.h"

struct com {
    bool operator()(pair<int, int> const& a, pair<int, int> const& b)
    {
        return a.second < b.second;
    }
};

class MakeMinimalWithSorting{
public:
    Graph G;
    MakeMinimalWithSorting(Graph& G);
    pair<vector<bool>, int> fvs;
    pair<vector<bool>, int> Compute();
    void reduce();
};

MakeMinimalWithSorting::MakeMinimalWithSorting(Graph &G)
{
    this->G = G;
}

void MakeMinimalWithSorting::reduce()
{
    vector<int> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(auto u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 || G.GetDegree(v))
        {
            this->fvs.first[v]=false;
            this->fvs.second--;
        }
    }
}

pair<vector<bool>, int> MakeMinimalWithSorting::Compute()
{
    this->fvs.first.resize(G.GetNumVertices(), true);
    this->fvs.second=G.GetNumVertices();
    //this->reduce();
    vector<int> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(const auto& u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }

    priority_queue<pair<NodeID , int> > Q;
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]+G.GetDegree(v)<2) {
            continue;
        }
        if(indegree[v]==1 && G.GetDegree(v)==1 && fvs.first[G.graph[v][0]])
        {
            continue;
        }
        Q.push(make_pair(v, indegree[v] + G.GetDegree(v)));
    }

    while(!Q.empty())
    {
        auto v_pair = Q.top();
        Q.pop();
        if(v_pair.first+v_pair.second<2)
        {
            continue;
        }
        NodeID v = v_pair.first;
        if(fvs.first[v]) {
            this->fvs.first[v] = false;
            if (isAcyclic(G, fvs.first)) {
                this->fvs.first[v] = false;
                this->fvs.second--;
            } else {
                this->fvs.first[v] = true;
            }
        }
    }
    return this->fvs;
}

#endif //BREAKINGCYCLES_MAKEMINIMALWITHSORTING_H
