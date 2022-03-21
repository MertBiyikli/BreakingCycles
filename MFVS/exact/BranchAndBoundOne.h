//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_BRANCHANDBOUNDONE_H
#define BREAKINGCYCLES_BRANCHANDBOUNDONE_H

#include "../include/graph.h"
#include "../heuristic/MakeMinimalWithSorting.h"
#include "../algorithms/SCC.h"

void combinationUtil(vector<int> arr, int n, int r,
                     int index, int data[], int i);

// The main function that prints all combinations of
// size r in arr[] of size n. This function mainly
// uses combinationUtil()
void printCombination(vector<int> arr, int n, int r)
{

    // A temporary array to store all combination
    // one by one
    int data[r];

    // Print all combination using temporary array 'data[]'
    combinationUtil(arr, n, r, 0, data, 0);
}

vector<vector<int> > store;

/* arr[] ---> Input Array
n	 ---> Size of input array
r	 ---> Size of a combination to be printed
index ---> Current index in data[]
data[] ---> Temporary array to store current combination
i	 ---> index of current element in arr[]	 */
void combinationUtil(vector<int> arr, int n, int r, int index,
                     int data[], int i)
{
    // Current combination is ready, print it
    vector<int> tmp;
    if (index == r) {
        for (int j = 0; j < r; j++)
            tmp.push_back(data[j]);//cout <<" "<< data[j];
        store.push_back(tmp);
        //cout <<"\n";
        return;
    }

    // When no more elements are there to put in data[]
    if (i >= n)
        return;

    // current is included, put next at next location
    data[index] = arr[i];
    combinationUtil(arr, n, r, index + 1, data, i + 1);

    // current is excluded, replace it with next
    // (Note that i+1 is passed, but index is not
    // changed)
    combinationUtil(arr, n, r, index, data, i + 1);
}

class BranchAndBound{
    Graph G;
public:
    BranchAndBound(Graph& G);
    pair<vector<bool>, int> Compute();
    pair<vector<bool>, int> FVS;
};

BranchAndBound::BranchAndBound(Graph &G)
{
    this->G = G;
}

pair<vector<bool>, int> BranchAndBound::Compute()
{
    MakeMinimalWithSorting M(G);
    this->FVS = M.Compute();
    Tarjan T(G);
    int SCC_number = (T.getSCC()).size();
    vector<NodeID> tmp;
    for(int i = 0; i<FVS.first.size(); i++)
    {
        if(FVS.first[i])
        {
            tmp.push_back(i);
        }
    }
    for(int i = FVS.second; i>0; i++)
    {
        cout << i << endl;
        store = vector<vector<int> > ();
        cout << "before print combi" << endl;
        printCombination(tmp, tmp.size(), i);
        cout << "after print combi" << endl;
        for(const auto& sol : store)
        {
            auto fvs_tmp = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
            for(const auto& j : sol)
            {
                fvs_tmp.first[j]=true;
                fvs_tmp.second++;
            }
            if(isAcyclic(G, fvs_tmp.first) && fvs_tmp.second<FVS.second)
            {
                FVS=fvs_tmp;
            }
        }
    }
    return FVS;
}

#endif //BREAKINGCYCLES_BRANCHANDBOUNDONE_H
