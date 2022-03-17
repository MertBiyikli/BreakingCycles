//
// Created by Mert Biyikli on 16.03.22.
//

#ifndef BREAKINGCYCLES_SCCRANDOM_H
#define BREAKINGCYCLES_SCCRANDOM_H

#include "../include/graph.h"
#include "../algorithms/SCC.h"
#include <random>



class SCCRandom{
public:
    Graph G;
    SCCRandom(Graph& G): T(G){this->G = G;};
    Tarjan T;
    pair<vector<bool>, int> fvs;
    pair<vector<bool>, int> Compute();
};

pair<vector<bool>, int> SCCRandom::Compute() // TODO: try to fix this, the idea is good and might be very fast
{
    auto SCC = T.getSCC();
    this->fvs.first.resize(G.GetNumVertices(), false);
    this->fvs.second=0;


    for(auto scc: SCC)
    {
        random_device rd;  // Will be used to obtain a seed for the random number engine
        mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        uniform_real_distribution<> SCC_random(0, scc.size());
        for(int it = 0; it<sqrt(scc.size()); it++) {
            NodeID u = scc[SCC_random(gen)];
            if (!fvs.first[u]) {
                fvs.first[u] = true;
                fvs.second++;
            }
        }
    }

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> SCC_random(0, SCC.size());
    while(!isAcyclic(G, fvs.first))
    {
        int randSCC =SCC_random(gen);
        uniform_real_distribution<> scc_random(0, SCC[randSCC].size());
        NodeID u = scc_random(gen);
        if(!fvs.first[u])
        {
            fvs.first[u]=true;
            fvs.second++;
        }
    }
    return this->fvs;
}

#endif //BREAKINGCYCLES_SCCRANDOM_H
