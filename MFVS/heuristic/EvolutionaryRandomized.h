//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H
#define BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H

#include <random>
#include "../include/graph.h"


class EvolutionaryRandomized{
public:
    Graph G;
    int popSize;

    //priority_queue<pair<vector<bool>, int>, vector<pair<vector<bool>, int> >, CompareFVS> Q;

    EvolutionaryRandomized(Graph& G, int N);
    pair<vector<bool>, int> Best;
    vector<pair<vector<bool>, int>> population;
    void InitPopulation();
    pair<vector<bool>, int> Mutate(pair<vector<bool>, int>& a, pair<vector<bool>, int>& b);
    pair<vector<bool>, int> GetTheBest();
};

EvolutionaryRandomized::EvolutionaryRandomized(Graph &G, int N)
{
    this->popSize = N;
    this->G = G;
    this->Best.first.resize(G.GetNumVertices(), true);
    this->Best.second=G.GetNumVertices();
}

void EvolutionaryRandomized::InitPopulation() {
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0, G.GetNumVertices());

    for (int i = 0; i < popSize; i++)
    {
        auto fvs = make_pair(vector<bool>(G.GetNumVertices()), 0);
        for(int j = 0; j<sqrt(G.GetNumVertices()); j++)
        {
            int randomValue = rand()%G.GetNumVertices();//(gen);
            if(!fvs.first[randomValue])
            {
                fvs.first[randomValue]=true;
                fvs.second++;
            }
        }
        cout << "Before the while loop" << i << endl;/*
        while(!isAcyclic(G, fvs.first))
        {
            int randomValue = dis(gen);
            if(!fvs.first[randomValue])
            {
                fvs.first[dis(gen)]=true;
                fvs.second++;
            }
        }*/
        this->population.push_back(fvs);
    }

}

pair<vector<bool>, int> EvolutionaryRandomized::Mutate(pair<vector<bool>, int>& a, pair<vector<bool>, int>& b)
{
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0, G.GetNumVertices());
    auto fvs=make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(a.first[v] && b.first[v])
        {
            fvs.first[v]=true;
            fvs.second++;
        }
    }
    while(!isAcyclic(G, fvs.first))
    {
        int randomValue = dis(gen);
        if(!fvs.first[randomValue])
        {
            fvs.first[randomValue]=true;
            fvs.second++;
        }
    }
    return fvs;
}

pair<vector<bool>, int> EvolutionaryRandomized::GetTheBest()
{
    cout << "reached GetTheBest" << endl;
    //for(int i = 0; i<sqrt(popSize); i++) {
        for (int i = 0; i < population.size(); i++) {
            auto mutation = this->Mutate(population[i], population[i + 1]);
            if (population[i].second < population[i + 1].second) {
                population[i + 1] = mutation;
            }
        }
    //}

    for(auto& i : population) {
        if (this->Best.second > i.second) {
            this->Best = i;
        }
    }
    return this->Best;
}


#endif //BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H
