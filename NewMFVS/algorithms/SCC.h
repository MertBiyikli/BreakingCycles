//
// Created by Mert Biyikli on 21.04.22.
//

#ifndef BREAKINGCYCLES_SCC_H
#define BREAKINGCYCLES_SCC_H

#include "../include/graph.h"
#include <stack>

class Tarjan{
private:
    Graph G;
    vector<bool> included;
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
    Tarjan(Graph& G, vector<bool>& vec);
    vector<vector<NodeID > > SCC;
    vector<vector<int> > getSCC();
    vector<int> getSCC_component();
};

Tarjan::Tarjan(Graph &G)
{
    this->G=G;
    this->included.resize(G.GetNumVertices(), true);
}

Tarjan::Tarjan(Graph &G, vector<bool> &vec)
{
    this->G = G;
    this->included=vec;
}

vector<vector<int> > Tarjan::getSCC() {
    currentIndex = 0;
    this->indices.resize(G.GetNumVertices(), -1);
    this->ancestors.resize(G.GetNumVertices());
    this->IsInStack.resize(G.GetNumVertices(), false);

    this->SCC = vector<vector<int> >();
    for (NodeID it = 0; it < G.GetNumVertices(); it++) {
        if(included[it]) {
            if (indices[it] == -1) {
                compute(it, false);
            }
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
        if(included[it]) {
            if (indices[it] == -1) {
                compute(it, true);
            }
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
    vector<NodeID> outN;
    for(NodeID it : G.OutGoing(v))
    {
        if(included[it])
        {
            outN.push_back(it);
        }
    }
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
            while (u != v) {
                u = S.top();
                SCCbyNum[u] = currentComponent;
                S.pop();
                IsInStack[u] = false;
            }/* while (u != v)*/;
            ++currentComponent;
        } else {
            vector<int> scc = vector<int>();
            int u;
            while (u != v){
                u = S.top();
                scc.push_back(u);
                S.pop();
                IsInStack[u] = false;
            }/* while (u != v)*/;
            this->SCC.push_back(scc);
        }
    }
}

#endif //BREAKINGCYCLES_SCC_H
