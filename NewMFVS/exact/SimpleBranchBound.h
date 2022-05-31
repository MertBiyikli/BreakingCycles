//
// Created by Mert Biyikli on 22.04.22.
//

#ifndef BREAKINGCYCLES_SIMPLEBRANCHBOUND_H
#define BREAKINGCYCLES_SIMPLEBRANCHBOUND_H

#include "../include/graph.h"
#include "../algorithms/CycleAlgorithms.h"



FVS MakeMinimalSCC(Graph& G);


/*
 * Eventhough this code works, I withdraw the exact solver from the competition
 */
class SimpleBranchBound{
    Graph G_;
public:
    SimpleBranchBound(Graph& G);
    int upperBoundValue(Graph& G);
    vector<NodeID> upperBound(Graph& G);
    int lowerBoundValue(Graph& G);
    vector<NodeID> minimumFeedbackVertexSet(Graph& G, bool print);
    vector<NodeID> MFVS(Graph& G, vector<int> solution, vector<int> bestSolution, int lowerBound,
    int level, bool reducible, bool verbose);

};

SimpleBranchBound::SimpleBranchBound(Graph &G) {
    this->G_ = G;
}

int SimpleBranchBound::upperBoundValue(Graph& G) {
    return int(upperBound(G).size());
}

vector<int> SimpleBranchBound::upperBound(Graph &G) {
    vector<int> solution;
    Graph h = G;
    while (h.GetNumVertices() > 0) {
        vector<int> partialSolution = h.reduce();
        for (vector<int>::iterator it = partialSolution.begin();
             it != partialSolution.end(); ++it) {
            solution.push_back(*it);
        }
        if (h.GetNumVertices() > 0) {
            int v = h.MinTotalDegree().first;
            h.MergeVertex(v);
        }
    }
    return solution;
}


int SimpleBranchBound::lowerBoundValue(Graph& G) {
    int lb = 0;
    Graph h = G;
    while (h.GetNumVertices() > 0) {
        lb += int(h.reduce().size());
        if (h.GetNumVertices() > 0) {
            vector<int> shortestCycle = h.shortestCycle();
            h.DeleteVertices(shortestCycle);
            lb += 1;
        }
    }
    return lb;
}


vector<int> SimpleBranchBound::minimumFeedbackVertexSet(Graph& G, bool print) {
    Graph h = G;
    vector<int> vs(h.GetNumVertices());
    copy(h.GetVertices().begin(), h.GetVertices().end(), vs.begin());
    auto fvs = MakeMinimalSCC(G);
    vector<NodeID> upperBound;
    for(NodeID v = 0; v<fvs.first.size(); v++)
    {
        if(fvs.first[v])
        {
            upperBound.push_back(v);
        }
    }
    return MFVS(h, vector<int>(), upperBound, 0, 0, true, print);
}

vector<int> SimpleBranchBound::MFVS(Graph& G, vector<int> solution, vector<int> bestSolution, int lowerBound,
                         int level, bool reducible, bool print) {
    Graph h=G;
    vector<int> partialSolution;
    if (reducible) {
        // Reducing the graph
        partialSolution = h.reduce();
        for (vector<int>::iterator it = partialSolution.begin(); it != partialSolution.end(); ++it)
            solution.push_back(*it);
        // Partitioning according to strongly connected components
        vector< vector<int> > sccs = h.getstronglyconnectedcomponents();
        if (int(sccs.size()) > 1) {
            for (vector< vector<int> >::iterator it = sccs.begin(); it != sccs.end(); ++it) {
                Graph scc = h.GetSubgraph(*it);
                vector<int> sccSolution = MFVS(h, vector<int>(), *it, 0, 0, false, print);
                solution.insert(solution.end(), sccSolution.begin(), sccSolution.end());
            }
            return solution;
        }
    }
    cout << "kommen wir überhauot hier hin???" << endl;
    if (print) {
        cout << "Level : " << level << endl;
        //h.print(false);
        cout << endl;
        cout << "u = " << upperBoundValue(h) << endl;
        cout << "l = " << lowerBoundValue(h) << endl;
    }
    // Lower bound and initial solution
    int lb = lowerBoundValue(h);
    if (level == 0) {
        lowerBound = lb;
        bestSolution = upperBound(h);
        for (vector<int>::iterator it = solution.begin(); it != solution.end(); ++it)
            bestSolution.push_back(*it);
    }
    // Bounding the search
    if (h.GetNumVertices() == 0) return solution;
    else if (int(solution.size()) + lb > int(bestSolution.size())) return bestSolution;
    // Branching
    int v = h.MaxTotalDegree().first;
    Graph leftGraph(h);
    leftGraph.DeleteVertex(v);
    solution.push_back(v);
    vector<int> leftSolution = MFVS(leftGraph, solution, bestSolution, lowerBound,
                                               level + 1, true, print);
    if (int(leftSolution.size()) < int(bestSolution.size())) bestSolution = leftSolution;
    // Rebounding the search
    if (lb == int(bestSolution.size())) return bestSolution;
    Graph rightGraph(h);
    rightGraph.MergeVertex(v);
    solution.pop_back();
    vector<int> rightSolution = MFVS(rightGraph, solution, bestSolution, lowerBound,
                                                 level + 1, true, print);
    if (int(rightSolution.size()) < int(bestSolution.size())) bestSolution = rightSolution;
    return bestSolution;
}


FVS MakeMinimalSCC(Graph& G)
{
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    auto scc = G.getstronglyconnectedcomponents();
    vector<NodeID> CandidateList;
    for(auto S : scc)
    {
        if(S.size()>1)
        {
            for(NodeID v : S) {
                fvs.first[v]=true;
                fvs.second++;
                CandidateList.push_back(v);
            }
        }
    }
    for(NodeID v : CandidateList)
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
    return fvs;
}

#endif //BREAKINGCYCLES_SIMPLEBRANCHBOUND_H
