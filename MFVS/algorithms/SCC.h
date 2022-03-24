//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_SCC_H
#define BREAKINGCYCLES_SCC_H

#include "../include/graph.h"



class Tarjan{
private:
    Graph G;
    int currentIndex;
    int currentComponent;
    vector<NodeID> indices;
    vector<NodeID> ancestors;
    stack<NodeID> S;
    vector<bool> IsInStack;
    vector<int> SCCbyNum;
    void compute(int, bool);
public:
    Tarjan(Graph& G);
    vector<vector<NodeID > > SCC;
    vector<vector<int> > getSCC();
    vector<int> getSCC_component();
};

Tarjan::Tarjan(Graph &G)
{
    this->G=G;
}

vector<vector<int>> Tarjan::getSCC() {
    currentIndex = 0;
    this->indices.resize(G.GetNumVertices(), -1);
    this->ancestors.resize(G.GetNumVertices());
    this->IsInStack.resize(G.GetNumVertices(), false);

    this->SCC = vector<vector<int> >();
    for (NodeID it = 0; it<G.GetNumVertices(); it++) {
        if (indices[it] == -1) {
            compute(it, false);
        }
    }
    return this->SCC;
}

vector<int> Tarjan::getSCC_component()
{
    currentIndex=0;
    indices.resize(G.GetNumVertices(), -1);
    this->ancestors.resize(G.GetNumVertices());
    this->IsInStack.resize(G.GetNumVertices(), false);
    currentComponent=0;

    this->SCCbyNum.resize(G.GetNumVertices());
    for (NodeID it = 0; it<G.GetNumVertices(); it++) {
        if (indices[it] == -1) {
            compute(it, true);
        }
    }
    return SCCbyNum;
}

void Tarjan::compute(int v, bool isNumber)
{
    indices[v] = currentIndex;
    ancestors[v] = currentIndex;
    ++currentIndex;
    S.push(v);
    IsInStack[v] = true;
    vector<NodeID> outN = G.graph[v];
    for (auto it : outN) {
        if (indices[it] == -1) {
            compute(it, isNumber);
            ancestors[v] = min(ancestors[v],ancestors[it]);
        } else if (IsInStack[it]) {
            ancestors[v] = min(ancestors[v],indices[it]);
        }
    }
    if (ancestors[v] == indices[v]) {
        if (isNumber) {
            NodeID u;
            do {
                u = S.top();
                SCCbyNum[u] = currentComponent;
                S.pop();
                IsInStack[u] = false;
            } while (u != v);
            ++currentComponent;
        } else {
            vector<int> scc = vector<int>();
            int u;
            do {
                u = S.top();
                scc.push_back(u);
                S.pop();
                IsInStack[u] = false;
            } while (u != v);
            this->SCC.push_back(scc);
        }
    }
}


#endif //BREAKINGCYCLES_SCC_H
