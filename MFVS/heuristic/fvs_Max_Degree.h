//
// Created by Mert Biyikli on 07.04.22.
//

#ifndef BREAKINGCYCLES_FVS_MAX_DEGREE_H
#define BREAKINGCYCLES_FVS_MAX_DEGREE_H

#include "../algorithms/SCC.h"

int min(const int a, const int& b)
{
    return (a<b? a : b);
}

class fvs_Max_Degree{
    Graph G;
public:
    vector<NodeID> fvs;
    fvs_Max_Degree(Graph& G);
    Graph LevyLowReductions(Graph& G, vector<bool>& inc);
    FVS Compute();
};

fvs_Max_Degree::fvs_Max_Degree(Graph &G)
{
    this->G = G;
}

Graph fvs_Max_Degree::LevyLowReductions(Graph &G, vector<bool>& inc)
{
    Graph H = G;
    // Reduction Rule 1
    for(NodeID v = 0; v<H.GetNumVertices(); v++)
    {
        for(NodeID u : H.neighbors(v))
        {
            if(u==v)
            {
                fvs.push_back(u);
                inc[u]=false;
            }
        }
    }
    vector<int> indegree(H.GetNumVertices(), -1);
    vector<int> outdegree(H.GetNumVertices(), -1);
    for(NodeID v = 0; v<H.GetNumVertices(); v++)
    {
        if(inc[v])
        {
            indegree[v]=0;
            outdegree[v]=0;
        }
    }
    // Reduction Rule 2
    for(NodeID v = 0; v<H.GetNumVertices(); v++)
    {
        if(inc[v]) {
            for (NodeID u : H.neighbors(v)) {
                if(inc[u])
                {
                    indegree[u]++;
                    outdegree[v]++;
                }
            }
        }
    }

    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 || outdegree[v]==0)
        {
            inc[v]=false;
        }
    }


    //cout << "Before red 3 " << endl;
    // Reduction Rule 3
    for(NodeID v = 0; v<H.GetNumVertices(); v++)
    {
        if(indegree[v]==1 || outdegree[v]==1)
        {
            auto ingoing = H.InGoing(v, inc);
            auto outgoing = G.OutGoing(v, inc);/*
            vector<vector<bool> > IncOut(outgoing.size(), vector<bool>(H.GetNumVertices(), false));
            vector<vector<bool> > IncIn(ingoing.size(), vector<bool>(H.GetNumVertices(), false));
            for(NodeID in = 0; in<IncIn.size(); in++)
            {
                for(NodeID it : H.graph[ingoing[in]])
                {
                    IncOut[in][it]=true;
                }
            }

            for(NodeID ut = 0; ut<ingoing.size(); ut++)
            {
                NodeID u = ingoing[ut];
                for (NodeID l : outgoing) {
                    if(!IncOut[ut][l])
                    {
                        H.graph[u].push_back(l);
                    }else{
                        continue;
                    }
                }
            }*/
            for(NodeID u : ingoing)
            {
                for(NodeID l : outgoing)
                {
                    bool exists = false;
                    for(NodeID w : H.neighbors(u))
                    {
                        if(l==w)
                        {
                            exists=true;
                        }
                    }
                    if(!exists)
                    {
                        H.graph[u].push_back(l);
                    }
                }
            }
        }
    }
    return H;
}

int ctr = 0;

FVS fvs_Max_Degree::Compute()
{
    FVS fvs1 = make_pair(vector<bool>(this->G.GetNumVertices(), true), this->G.GetNumVertices());
    vector<bool> inc(G.GetNumVertices(), true);
    auto H = this->LevyLowReductions(G, inc);
    auto scc = Tarjan(H, inc).getSCC();
    int previousSize = H.GetNumVertices();
    while(previousSize!=scc.size())
    {
        // remove g from L
        vector<NodeID> max_scc;
        for(const auto& S : scc)
        {
            if(S.size()>max_scc.size())
            {
                max_scc=S;
            }
        }
        vector<int> indegree(H.GetNumVertices(), -1);
        vector<int> outdegree(H.GetNumVertices(), -1);
        for(NodeID v = 0; v<H.GetNumVertices(); v++)
        {
            if(inc[v]){
                indegree[v]=0;
                outdegree[v]=0;
            }
        }


        for(NodeID v = 0; v<H.GetNumVertices(); v++)
        {
            for(NodeID u : H.neighbors(v))
            {
                outdegree[v]++;
                indegree[u]++;
            }
        }

        vector<int> TotalDegree(H.GetNumVertices(), 0);
        for(NodeID v = 0; v<H.GetNumVertices(); v++)
        {
            TotalDegree[v]=min(indegree[v], outdegree[v]);
        }

        pair<NodeID , int> Max = make_pair(0,0);
        for(NodeID v : max_scc)
        {
            if(TotalDegree[v]<Max.second)
            {
                Max.first=v;
                Max.second=TotalDegree[v];
            }
        }
        NodeID V = Max.first; // v <- max(min(d-(v), d+(v), v e V(H))
        fvs.push_back(V); // S <- S + {v}

        inc[V]=false;
        auto H_ = this->LevyLowReductions(H, inc);
        previousSize = scc.size();
        scc = Tarjan(H_, inc).getSCC();
    }
    //cout << "after the while " << endl;
    for(NodeID v : this->fvs)
    {
        fvs1.first[v]=true;
        fvs1.second++;
    }
    for(NodeID v = 0; v<H.GetNumVertices(); v++)
    {
        if(inc[v])
        {
            if(fvs1.first[v])
            {
                fvs1.first[v]=false;
                if(isAcyclic(H, fvs1.first))
                {
                    fvs1.second--;
                } else{
                    fvs1.first[v]=true;
                }
            }
        }
    }
    return fvs1;
}

#endif //BREAKINGCYCLES_FVS_MAX_DEGREE_H
