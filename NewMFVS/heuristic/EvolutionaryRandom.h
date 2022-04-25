//
// Created by Mert Biyikli on 23.04.22.
//

#ifndef BREAKINGCYCLES_EVOLUTIONARYRANDOM_H
#define BREAKINGCYCLES_EVOLUTIONARYRANDOM_H

#include "../include/graph.h"
#include "../algorithms/CycleAlgorithms.h"

class EvolutionaryRandom{
public:
    Graph G;
    queue<vector<NodeID> > population;
    vector<vector<NodeID> > DisjointCycles;
    EvolutionaryRandom(Graph& G);
    void InitPopulation(const int& N);
    void Mutate();
    void MakeMinimal();
};

EvolutionaryRandom::EvolutionaryRandom(Graph &G) {
    this->G = G;
}

void EvolutionaryRandom::InitPopulation(const int &N) {
    for(int i = 0; i<N; i++)
    {
        cout << "The " << i << "-th pop" << endl;
        Graph h = G;
        vector<NodeID> FVS;
        while(!h.isAcyclic())
        {
            NodeID v = rand()%h.currentMax;
            if(h.hasVertex(v)) {
                FVS.push_back(v);
                h.DeleteVertex(v);
                h.reduce();
            }
        }
        cout << "FVS size: " << FVS.size() << endl;
        population.push(FVS);
    }
}

void EvolutionaryRandom::Mutate() {
    auto fvs1 = population.front();
    population.pop();
    auto fvs2 = population.front();
    population.pop();
    vector<bool> fvs1_bool(G.GetNumVertices(), false), fvs2_bool(G.GetNumVertices(), false), best_bool(
            G.GetNumVertices(), false);
    for (const auto &v : fvs1) {
        fvs1_bool[v] = true;
    }
    for (const auto &v : fvs2) {
        fvs2_bool[v] = true;
    }
    for (int i = 0; i < G.GetNumVertices(); i++){
        if(fvs1_bool[i] && fvs2_bool[i])
        {
            best_bool[i]=true;
        }
    }
    vector<NodeID> best;
    for(NodeID v=0; v<G.GetNumVertices(); v++)
    {
        if(best_bool[v])
        {
            best.push_back(v);
        }
    }
    Graph h = G;
    h.DeleteVertices(best);

    cout << "This is the common size: " << best.size() << endl;
    auto DisCycles = Cycle(h).GetDisjointCycle(h);
    while(!DisCycles.empty())
    {
        for(auto cyc : DisCycles)
        {
            NodeID v = cyc[rand()&cyc.size()];
            best.push_back(v);
            h.DeleteVertices(cyc);
        }
        DisCycles = Cycle(h).GetDisjointCycle(h);
    }
    h.DeleteVertices(best);
    if(h.isAcyclic())
    {
        population.push(best);
    }else{
        cout << "Sorry bro" << endl;
    }



    /*
    auto DisjointCycle = Cycle(G).GetDisjointCycle(G);
    for(auto cyc : DisjointCycle)
    {
        for(NodeID v : cyc)
        {
            cout << v << " ";
        }
        cout << endl;
    }
    for(auto cycle : DisjointCycle)
    {
        NodeID v = cycle[rand()%cycle.size()];
        h.DeleteVertex(v);
        best.push_back(v);
    }

    Graph K = G;
    K.DeleteVertices(best);
    if(K.isAcyclic())
    {
        cout << "Yes bro we made it" << endl;
    }else{
        cout << "Sorry bro" << endl;
    }
    population.push(best);*/
}

void EvolutionaryRandom::MakeMinimal() {
    auto fvs_pop = population.front();
    population.pop();
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(NodeID v : fvs_pop)
    {
        fvs.first[v]=true;
        fvs.second++;
    }
    for(NodeID v : fvs_pop)
    {
        if(fvs.first[v])
        {
            fvs.first[v]=false;
            if(isAcyclic(G, fvs.first))
            {
                fvs.second--;
            }else{
                fvs.first[v]=true;
            }
        }
    }
    fvs_pop.clear();
    for(NodeID it = 0; it< G.GetNumVertices(); it++)
    {
        if(fvs.first[it])
        {
            fvs_pop.push_back(it);
        }
    }
    population.push(fvs_pop);
}

#endif //BREAKINGCYCLES_EVOLUTIONARYRANDOM_H
