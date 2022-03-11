//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_BRANCHANDBOUNDONE_H
#define BREAKINGCYCLES_BRANCHANDBOUNDONE_H

#include "../include/graph.h"

class BranchAndBound{
    Graph G;
public:
    BranchAndBound(Graph& G);
    void Reduction();
    vector<bool> MinimumFVS();
    void recurse(int, int, int);
    void ComputeSolution(int, int);
    void pretty_print(const vector<NodeID>& comb);
    vector<bool> FVS;
    vector<NodeID> combination;
    vector<NodeID> people;
};

#endif //BREAKINGCYCLES_BRANCHANDBOUNDONE_H
