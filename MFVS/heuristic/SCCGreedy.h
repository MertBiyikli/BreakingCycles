//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_SCCGREEDY_H
#define BREAKINGCYCLES_SCCGREEDY_H

#include "../algorithms/SCC.h"


struct CompareFVS {
    bool operator()(pair<int, int> const& a, pair<int, int> const& b)
    {
        return a.second < b.second;
    }
};

class SCCGreedy{
public:
    Graph G;
    SCCGreedy(Graph& G): T(G){this->G = G;};
    Tarjan T;
    pair<vector<bool>, int> fvs;
    pair<vector<bool>, int> Compute();
};

pair<vector<bool>, int> SCCGreedy::Compute() // TODO: try to fix this, the idea is good and might be very fast
{
    auto SCC = T.getSCC(); // TODO: This causes a seg fault
    this->fvs.first.resize(G.GetNumVertices(), false);
    this->fvs.second=0;
    vector<int> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(auto& u : G.neighbors(v))
        {
            indegree.at(u)++;
        }
    }
    vector<int> TotalDegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        TotalDegree[v]=G.GetDegree(v)+indegree[v];
    }


    priority_queue<pair<int, int>, vector<pair<int, int>>, CompareFVS> Queues;

    for(auto& i : SCC)
    {
        if(i.size()>1)
        {
            for(auto& u : i)
            {
                Queues.push(make_pair(u, TotalDegree[u]));
            }
        }
    }
    for(int it = 0; it<sqrt(Queues.size()); it++)
    {
        //cout << Queues.top().first << " " << Queues.top().second << endl;
        auto u = Queues.top();
        Queues.pop();
        this->fvs.first.at(u.first)=true;
        this->fvs.second++;
    }

    int ctr = 0;
    while(!isAcyclic(G, fvs.first)) // TODO: Here is the problem
    {
        //cout << Queues.top().first << " " << Queues.top().second << endl;
        if(Queues.size()>10) {
            for (int it = 0; it < sqrt(Queues.size()); it++) {
                //cout << Queues.size() << endl;
                auto u = Queues.top();
                Queues.pop();
                this->fvs.first.at(u.first) = true;
                this->fvs.second++;
            }
        }else{
            //cout << Queues.size() << endl;
            auto u = Queues.top();
            Queues.pop();
            this->fvs.first[u.first] = true;
            this->fvs.second++;
        }
    }
    return this->fvs;
}




#endif //BREAKINGCYCLES_SCCGREEDY_H
