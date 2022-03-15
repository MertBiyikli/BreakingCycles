//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_SCCGREEDY_H
#define BREAKINGCYCLES_SCCGREEDY_H

#include "../algorithms/SCC.h"


struct CompareFVS {
    bool operator()(pair<int, int> const& a, pair<int, int> const& b)
    {
        return a.second > b.second;
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
    auto SCC = T.getSCC();
    this->fvs.first.resize(G.GetNumVertices(), false);
    this->fvs.second=0;
    vector<int> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(auto& u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }
    cout << "before one" << endl;
    vector<int> TotalDegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        TotalDegree[v]=G.GetDegree(v)+indegree[v];
    }

    cout << "before two" << endl;
    vector<priority_queue<pair<int, int>, vector<pair<int, int>>, CompareFVS> > Queues;

    for(auto& i : SCC)
    {
        if(i.size()>1)
        {
            priority_queue<pair<int, int>, vector<pair<int, int>>, CompareFVS> Q;
            for(auto& u : i)
            {
                Q.push(make_pair(u, TotalDegree[u]));
            }
            Queues.push_back(Q);
        }
    }
    cout << "before three" << endl;
    for(auto i: Queues)
    {
        cout << i.size() << " / " << Queues.size() << endl;
    }
    for(auto& Q : Queues)
    {
        this->fvs.first[(Q.top()).first];
        fvs.second++;
        Q.pop();
    }
    int ctr = 0;
    while(!isAcyclic(G, fvs.first)) // TODO: Here is the proble
    {
        for(auto& Q : Queues)
        {
            auto u = Q.top();
            Q.pop();
            cout << Q.size() << endl;
            fvs.first[u.first];
            fvs.second++;
        }
    }
    cout << "before five" << endl;
    return this->fvs;
}




#endif //BREAKINGCYCLES_SCCGREEDY_H
