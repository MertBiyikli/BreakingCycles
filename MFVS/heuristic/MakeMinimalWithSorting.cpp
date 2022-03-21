//
// Created by Mert Biyikli on 17.03.22.
//

#include "MakeMinimalWithSorting.h"

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
        for(auto u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }
    vector<pair<int, int> > TotalDegree(G.GetNumVertices());
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        TotalDegree[v]=make_pair(v, indegree[v]+G.GetDegree(v));
    }

    sort(TotalDegree.begin(), TotalDegree.end(), [](pair<int, int>& a, pair<int, int>& b){
        return a.second < b.second;
    });

    for(auto v_pair : TotalDegree)
    {
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