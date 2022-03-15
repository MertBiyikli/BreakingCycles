//
// Created by Mert Biyikli on 15.03.22.
//

#ifndef BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H
#define BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H

#include <random>
#include "../include/graph.h"
/*
struct CompareFVS {
    bool operator()(pair<vector<bool>, int> const& a, pair<vector<bool>, int> const& b)
    {
        return a.second > b.second;
    }
};

class EvolutionaryRandomized{
public:
    Graph G;
    int popSize;

    priority_queue<pair<vector<bool>, int>, vector<pair<vector<bool>, int> >, CompareFVS> Q;

    EvolutionaryRandomized(Graph& G, int N);
    pair<vector<bool>, int> Best;
    vector<pair<vector<bool>, int>> population;
    void InitPopulation();
    pair<vector<bool>, int> compare(pair<vector<bool>, int>& a, pair<vector<bool>, int>& b);
    pair<vector<bool>, int> GetTheBest();
};

EvolutionaryRandomized::EvolutionaryRandomized(Graph &G, int N)
{
    this->popSize = N;
    this->G = G;
}

void EvolutionaryRandomized::InitPopulation()
{
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0, G.GetNumVertices());
    for(int i = 0; i<popSize; i++)
    {
        pair<vector<bool>, int> currentFvs;
        currentFvs.first.resize(G.GetNumVertices(), false);
        currentFvs.second=0;
        while(!isAcyclic(G, currentFvs.first)) {
            for (int j = 0; j < sqrt(G.GetNumVertices() / 2); j++) {
                int rand = dis(gen);
                if(!currentFvs.first[rand])
                {
                    currentFvs.first[rand]=true;
                    currentFvs.second++;
                }
            }
        }
        this->population.push_back(currentFvs);
    }
}

pair<vector<bool>, int> EvolutionaryRandomized::compare(pair<vector<bool>, int>& a, pair<vector<bool>, int>& b)
{
    return (a.second < b.second ? a : b);
}

pair<vector<bool>, int> EvolutionaryRandomized::GetTheBest()
{
    for(int i = 0; i<popSize; i+=2)
    {
        Q.push(compare(population[i], population[i+1]));
    }
    this->Best = Q.top();
    return this->Best;
}
*/

#endif //BREAKINGCYCLES_EVOLUTIONARYRANDOMIZED_H
