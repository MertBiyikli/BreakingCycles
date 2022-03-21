//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_EVOLUTIONARYSCC_H
#define BREAKINGCYCLES_EVOLUTIONARYSCC_H

#include "../include/graph.h"
#include "../algorithms/SCC.h"
#include <random>

class EvolutionarySCC{
public:
    Graph G;
    Tarjan T;
    int popSize;
    vector<vector<NodeID>> SCC;
    vector<FVS> population;
    EvolutionarySCC(Graph& G);
    FVS Mutate(const FVS& a, const FVS& b);
    FVS Compute();
    void Init(int);
};

EvolutionarySCC::EvolutionarySCC(Graph &G) : T(G)
{
    this->G = G;
    this->SCC = T.getSCC();
}

void EvolutionarySCC::Init(int N) {
    this->popSize = N;
    for (int i = 0; i < this->popSize; i++)
    {
        FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
        for(auto& S : this->SCC) {
            if (S.size() > 1) {
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<> dis(0, S.size());

                for (int j = 0; j < sqrt(S.size()); j++) {
                    int RandomValue = dis(gen);
                    if (!fvs.first[RandomValue]) {
                        fvs.first[RandomValue] = true;
                        fvs.second++;
                    }
                }
            }
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, G.GetNumVertices());
        while(!isAcyclic(G, fvs.first))
        {
            int randomValue = dis(gen);
            if(!fvs.first[randomValue])
            {
                fvs.first[randomValue]= true;
                fvs.second++;
            }
        }
        this->population.push_back(fvs);
    }
}


FVS EvolutionarySCC::Mutate(const FVS &a, const FVS &b)
{
    int ctr=0;
    //FVS fvs_second = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(NodeID it = 0; it<G.GetNumVertices(); it++)
    {
        if(a.first[it] && b.first[it])
        {

            fvs.first[it]=true;
            fvs.second++;
        }
    }
    for(auto& S : this->SCC) {
        if (S.size() > 1) {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0, S.size());

            for (int j = 0; j < sqrt(S.size()); j++) {
                int RandomValue = dis(gen);
                if (!fvs.first[RandomValue]) {
                    fvs.first[RandomValue] = true;
                    fvs.second++;
                }
            }
        }
    }
    while(!isAcyclic(G, fvs.first))
    {
        for(auto& S : this->SCC) {
            if (S.size() > 1) {
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<> dis(0, S.size());

                for (int j = 0; j < sqrt(S.size()); j++) {
                    int RandomValue = dis(gen);
                    if (!fvs.first[RandomValue]) {
                        fvs.first[RandomValue] = true;
                        fvs.second++;
                    }
                }
            }
        }
    }
    return fvs;
}

FVS EvolutionarySCC::Compute()
{
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    //for(int i = 0; i<10; i++) {
        for (int it = 0; it < population.size(); it += 2) {
            auto mutation = this->Mutate(population[it], population[it + 1]);
            if (population[it].second < population[it + 1].second) {
                population[it + 1] = mutation;
            }

        }
    //}
    FVS Best = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    for(auto& pop : population)
    {
        if(pop.second<Best.second)
        {
            Best = pop;
        }
    }
    return Best;
}

#endif //BREAKINGCYCLES_EVOLUTIONARYSCC_H
