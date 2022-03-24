//
// Created by Mert Biyikli on 19.03.22.
//

#ifndef BREAKINGCYCLES_BRANCHBOUND_H
#define BREAKINGCYCLES_BRANCHBOUND_H

#include "../algorithms/SCC.h"
#include "../include/graph.h"
#include <algorithm>
#include "../include/subgraph.h"

class BranchBound{
public:
    Graph G_;
    BranchBound(Graph& G);
    int lowerBoundValue(Graph G, bool verbose) const;
    FVS MFVS(bool);
    FVS reduce(Graph& G);
    FVS mfvs(Graph& G, FVS solution, FVS& bestSolution, int lowerBound,
                     int level, bool reducible, bool verbos);
};

BranchBound::BranchBound(Graph &G)
{
    this->G_ = G;
}

FVS BranchBound::reduce(Graph &G) {
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    vector<int> indegree(G.GetNumVertices(), 0);
    for (NodeID v = 0; v < G.GetNumVertices(); v++) {
        for (auto u : G.neighbors(v)) {
            indegree[u]++;
        }
    }

    for (NodeID v = 0; v < G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 || G.GetDegree(v)==0)
        {
            fvs.first[v]= false;
            fvs.second--;
        }
    }
    return fvs;
}

FVS BranchBound::MFVS(bool verbose)
{
    Graph h=this->G_;
    FVS vs = make_pair(vector<bool>(h.GetNumVertices(), true), h.GetNumVertices());
    return this->mfvs(h, make_pair(vector<bool>(h.GetNumVertices(), false), 0), vs, 0, 0, true, verbose);
}

FVS BranchBound::mfvs(Graph &G__, FVS solution, FVS& bestSolution, int lowerBound, int level,
                      bool reducible, bool verbos) {

    Graph G = G__;
    cout << G.GetNumVertices() << endl;
    FVS fvs = this->reduce(G);
    cout << fvs.first.size() << " " << solution.first.size() << endl;
    for (NodeID it = 0; it < G.GetNumVertices(); it++) {
        if(solution.first[it] && fvs.first[it] || !solution.first[it] && !fvs.first[it] || solution.first[it] && !fvs.first[it] )
        {
            continue;
        }
        if(!solution.first[it] && fvs.first[it])
        {
            solution.first[it]=true;
            solution.second++;
        }
    }
    cout << fvs.first.size() << " " << solution.first.size() << endl;

    cout << "before the for loop: " << solution.first.size() << " " << solution.second << endl;

    // This is for constructing each subgraph for every SCC and computing for each SCC again the optimal solution
    Tarjan T(G);
    auto scc = T.getSCC();
    cout << "after computing the Scc" << endl;
    if(scc.size()>1)
    {
        cout << "in the first if" << endl;
        for(auto & it : scc)
        {
            cout << "scc size " << it.size() << endl;
            if(it.size()>1) {
                cout << "in the if from the scc loop" << endl;
                auto sub = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
                for (auto it_ : it) {
                    sub.first[it_] = true;
                    sub.second++;
                }
                auto SubG = Subgraph(G, sub.first).GetSubGraph().first;
                cout << "SubG size: " << SubG.GetNumVertices() << endl;
                auto sccSol = this->mfvs(SubG,
                                         make_pair(vector<bool>(SubG.GetNumVertices(), true), SubG.GetNumVertices()),
                                         sub, 0, 0, false, verbos);
            }
        }
        cout << "after the for loop" << endl;
        cout << solution.second << " ";
        cout << solution.first.size() << endl;
        for(auto i : solution.first)
            cout << i << " ";
        cout << endl;
        return solution;
    }
    cout << "after computing the reduced graphs for the SCC " << level << endl;

    if (G.GetNumVertices() == 0) return solution;

    // Branching
    pair<int, int> maxV = {0,0};
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(G.GetDegree(v)>maxV.second)
        {
            maxV.first=v;
            maxV.second=G.GetDegree(v);
        }
    }
    //cout << G.GetNumVertices() << endl;
    cout << "after computing the maxDegree" << endl;
    vector<bool> left(G.GetNumVertices(), true);
    cout << "after inniting the left" << endl;
    cout << maxV.first << " / " << G.GetNumVertices() << endl;
    left.at(maxV.first)=false;
    cout << "after init the left solution" << endl;
    Graph leftGraph = Subgraph(G, left).GetSubGraph().first;
    solution.first[maxV.first]=true;
    solution.second++;

    cout << "after computing the leftgraph" << level << endl;
    FVS leftSol = this->mfvs(leftGraph, solution, bestSolution, 0, level+1, true, verbos);


    //if (lb == int(bestSolution.size())) return bestSolution;
    if(leftSol.second<bestSolution.second)
    {
        bestSolution=leftSol;
    }

    vector<NodeID> InComingNeig;
    vector<NodeID> OutGoing = G.graph[maxV.first];
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(auto u : G.neighbors(v))
        {
            if(u==maxV.first)
            {
                InComingNeig.push_back(v);
            }
        }
    }
    Graph rightGraph = Subgraph(G, left).GetSubGraph().first;
    for(NodeID In : InComingNeig)
    {
        for(NodeID Out : OutGoing)
        {
            bool EdgeExists = false;
            for(auto it : rightGraph.graph[In])
            {
                if(it == Out)
                {
                    EdgeExists=true;
                }
            }
            if(!EdgeExists)
            {
                rightGraph.graph[In].push_back(Out);
            }
        }
    }
    solution.first[maxV.first]=false;
    solution.second--;
    auto rightSol = this->mfvs(rightGraph, solution, bestSolution, 0, level+1, true, verbos);

    if(rightSol.second<bestSolution.second){
        bestSolution=rightSol;
    }

    return bestSolution;
}


#endif //BREAKINGCYCLES_BRANCHBOUND_H
