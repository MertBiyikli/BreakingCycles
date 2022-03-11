//
// Created by Mert Biyikli on 07.03.22.
//

#include "BranchAndBoundOne.h"

BranchAndBound::BranchAndBound(Graph &G)
{
    this->G=G;
    this->people.resize(G.GetNumVertices());
    this->FVS.resize(G.GetNumVertices(), true);
    for(NodeID I = 0; I<people.size(); I++)
    {
        people[I]=I;
    }
    //this->recurse();
}

void BranchAndBound::Reduction()
{
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(G.GetDegree(v)==0)
        {
            FVS[v]=false;
        }
    }
}

vector<bool> BranchAndBound::MinimumFVS() {
    this->Reduction();
    int FVS_Size = 0;
    for(auto i : FVS)
    {
        if(i)
            FVS_Size++;
    }
    this->recurse(0, FVS_Size, 0);
    return vector<bool>(false);
}

void BranchAndBound::recurse(int i, int M, int P) {

}

void BranchAndBound::pretty_print(const vector<NodeID> &comb)
{
    static int count = 0;
    cout << "combination no " << (++count) << ": [ ";
    for (int i = 0; i < comb.size(); ++i) { cout << comb[i] << " "; }
    cout << "] " << endl;
}

void BranchAndBound::ComputeSolution(int offset, int k)
{
    if (k == 0) {
        pretty_print(combination);
        return;
    }
    for (int i = offset; i <= people.size() - k; ++i) {
        combination.push_back(people[i]);
        this->ComputeSolution(i+1, k-1);
        combination.pop_back();
    }
}
