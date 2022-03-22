//
// Created by Mert Biyikli on 17.03.22.
//

#ifndef BREAKINGCYCLES_MAKEMINIMALSCC_H
#define BREAKINGCYCLES_MAKEMINIMALSCC_H

#include "../algorithms/SCC.h"
#include "../include/subgraph.h"
#include <random>

class MakeMinimalSCC{
public:
    Graph G;
    Tarjan T;
    MakeMinimalSCC(Graph& G): T(G){this->G = G;};
    FVS Compute();
    FVS fvs;
};



FVS MakeMinimalSCC::Compute()
{
    vector<Subgraph> subgraphs;
    auto scc = T.getSCC();
    for(const auto& S : scc)
    {
        if(S.size()>1)
        {
            vector<bool> sub(G.GetNumVertices(), false);
            for(auto i : S)
            {
                sub[i]=true;
            }
            subgraphs.emplace_back(G, sub);
        }
    }
    /*
    vector<vector<int> > indegree(subgraphs.size());
    for(auto sub : subgraphs)
    {
        vector<int> indeg(sub.GetSubGraph().first.GetNumVertices(),0);
        for(NodeID v = 0; v<sub.GetSubGraph().first.GetNumVertices(); v++)
        {
            for(auto u : sub.GetSubGraph().first.neighbors(v))
            {
                indeg[u]++;
            }
        }
        indegree.push_back(indeg);
    }

    cout << "after computing the indegrees" << endl;
    vector<vector<pair<int, int> > > TotalDegree(subgraphs.size());
    for(int it =0; it<subgraphs.size(); it++) {
        vector<pair<int, int> > Totdeg(subgraphs[it].GetSubGraph().first.GetNumVertices());
        for (NodeID v = 0; v < subgraphs[it].GetSubGraph().first.GetNumVertices(); v++)
        {
            Totdeg[v]=make_pair(v, subgraphs[it].GetSubGraph().first.GetDegree(v)+indegree[it][v]);
        }
        TotalDegree.emplace_back(Totdeg);
    }

    for(auto& deg : TotalDegree)
    {
        sort(deg.begin(), deg.end(), [](pair<int, int>& a, pair<int, int>& b){
            return a.second < b.second;
        });
    }*/

    vector<FVS> fvs_s(subgraphs.size());
    for(int it = 0; it<subgraphs.size(); it++)
    {
        fvs_s[it]=make_pair(vector<bool>(subgraphs[it].GetSubGraph().first.GetNumVertices() ,true), subgraphs[it].GetSubGraph().first.GetNumVertices());
        for(NodeID v = 0; v<subgraphs[it].GetSubGraph().first.GetNumVertices(); v++)
        {
            if(fvs_s[it].first[v])
            {
                fvs_s[it].first[v]=false;
                if(isAcyclic(subgraphs[it].GetSubGraph().first, fvs_s[it].first))
                {
                    fvs_s[it].first[v]=false;
                    fvs_s[it].second--;
                }else{
                    fvs_s[it].first[v]=true;
                }
            }
        }
    }

    FVS supreme = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(int it = 0; it<subgraphs.size(); it++)
    {
        auto M = subgraphs[it].GetSubGraph().second;
        for(int I = 0; I<subgraphs[it].GetSubGraph().first.GetNumVertices(); I++)
        {
            if(fvs_s[it].first[I])
            {
                int Converted;
                for(auto j : M)
                {
                    if(j.second==I)
                    {
                        Converted=j.first;
                    }
                }
                supreme.first[Converted]=true;
                supreme.second++;
            }
        }
    }

    return supreme;
}
#endif //BREAKINGCYCLES_MAKEMINIMALSCC_H
