//
// Created by Mert Biyikli on 11.04.22.
//

#ifndef BREAKINGCYCLES_TOPOLOGICALSORTING_H
#define BREAKINGCYCLES_TOPOLOGICALSORTING_H

#include "../include/graph.h"

class TopologicalSorting{
    int V;
    Graph G;
    bool isAcyclic_;
    stack<NodeID> S;
    vector<bool> visited;
    void HelperFunction(NodeID v);
    vector<NodeID> TopSorting;
    vector<bool> allowed;
public:
    TopologicalSorting(Graph& G);
    TopologicalSorting(Graph& G, vector<bool>& inc);
    vector<NodeID> GetSorting() const;
    void ComputeSorting();
    void Kahn();
};

void TopologicalSorting::ComputeSorting() {
    cout << isAcyclic_ << endl;
    if(!isAcyclic_)
    {
        this->TopSorting={};
        return;
    }else{
        cout << "we get in here" << endl;
        this->visited.resize(G.GetNumVertices(), false);
        cout << "after the visited" << endl;
        for(NodeID v = 0; v<this->G.GetNumVertices(); v++)
        {
            cout << v << endl;
            if(this->allowed.at(v)) {
                cout << v << endl;
                if (!visited.at(v)) {
                    this->HelperFunction(v);
                }
            }
        }
        cout << S.size() << endl;
        while(!S.empty())
        {
            this->TopSorting.push_back(S.top());
            S.pop();
        }
    }
}



vector<NodeID> TopologicalSorting::GetSorting() const {
    //cout << TopSorting.size() << endl;
    return TopSorting;
}

TopologicalSorting::TopologicalSorting(Graph &G) {
    this->G = G;
    this->allowed.resize(G.GetNumVertices(), true);
    this->V = G.GetNumVertices();
    if(isAcyclic(this->G, this->allowed))
    {
        this->isAcyclic_=true;
    }else{
        this->isAcyclic_=false;
    }
    cout << (isAcyclic_ ? "It is acyclic" : "Sorry") << endl;
}

TopologicalSorting::TopologicalSorting(Graph &G, vector<bool> &inc) {
    this->G = G;
    this->allowed=inc;
    this->isAcyclic_=true;
    Tarjan T(G, this->allowed);
    for(auto S : T.getSCC())
    {
        //cout << S.size() << " ";
        if(S.size()!=1)
        {
            this->isAcyclic_=false;
        }
    }
    //cout << endl;
    this->V = 0;
    for(auto i : allowed)
        this->V+=i;/*
    if(isAcyclic(this->G, this->allowed))
    {
        this->isAcyclic_=true;
    }else{
        this->isAcyclic_=false;
    }
    cout << (isAcyclic_ ? "It is acyclic" : "Sorry") << endl;*/
}

void TopologicalSorting::HelperFunction(NodeID v) {
    this->visited.at(v)=true;
    for(NodeID u : G.neighbors(v))
    {
        if(this->allowed.at(u))
        {
            if(!visited.at(u))
            {
                this->HelperFunction(v);
            }
        }
    }
    this->S.push(v);
}

void TopologicalSorting::Kahn() {
    if(!isAcyclic_)
    {
        return;
    }
    //cout << isAcyclic_ << endl;
    vector<int> indegree(G.GetNumVertices(), -1);
    for(NodeID v = 0; v<G.GetNumVertices(); v++){
        if(this->allowed[v])
        {
            indegree[v]=0;
        }
    }

    for(NodeID v= 0; v<G.GetNumVertices(); v++)
    {
        if(this->allowed[v])
        {
            for(NodeID u : G.neighbors(v))
            {
                if(allowed[u])
                {
                    indegree[u]++;
                }
            }
        }
    }
/*
    for(auto u : indegree)
        cout << u << " ";
    cout << endl;*/


    queue<NodeID> Q;
    for(NodeID v= 0; v<G.GetNumVertices(); v++){
        if(this->allowed.at(v))
        {
            if(indegree.at(v)==0)
            {
                //cout << v << endl;
                Q.push(v);
            }
        }
    }

    int cnt = 0;
    while (!Q.empty())
    {
        NodeID u = Q.front();
        //cout << u << endl;
        Q.pop();
        this->TopSorting.push_back(u);
        for(NodeID w : G.neighbors(u))
        {
            if(this->allowed.at(w))
            {
                if(--indegree.at(w)==0)
                {
                    Q.push(w);
                }
            }
        }
        cnt = 0;
    }
}


#endif //BREAKINGCYCLES_TOPOLOGICALSORTING_H
