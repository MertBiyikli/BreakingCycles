//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_SCC_H
#define BREAKINGCYCLES_SCC_H

#include "../include/graph.h"


class SCC{
public:
    int numNodes;
    Graph graph;
    SCC(const Graph& G);
    void ComputeSCC(NodeID u);
    int GetSCC();
    int GetSCC_Number() const;
    vector<NodeID> disc;
    vector<NodeID> low;
    vector<bool> stackMember;
    //stack<int> *st = new stack<int>();
    stack<NodeID> st;
    int ctrSCC;
    vector<vector<NodeID> > SCC_components;
    vector<NodeID> components;
};

SCC::SCC(const Graph &G)
{
    this->numNodes = G.GetNumVertices();
    //graph = new list<int>[numNodes];
    /*
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(NodeID w : G.neighbors(v))
        {
            graph[v].push_back(w);
        }
    }*/
    this->graph = G;
    disc.resize(this->numNodes);
    low.resize(this->numNodes);
    components.resize(this->numNodes, -1);
    stackMember.resize(this->numNodes);
    this->ctrSCC = 0;
}
/*
SCC::~SCC()
{
    //delete st;
    //delete [] graph;
}*/

void SCC::ComputeSCC(NodeID u)
{
    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st.push(u);
    stackMember[u] = true;

    // Go through all vertices adjacent to this
    list<int>::iterator i;
    for (auto i : graph.neighbors(u)/* = graph[u].begin(); i != graph[u].end(); ++i*/)
    {
        NodeID v = i; // v is current adjacent of 'u'

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            this->ComputeSCC(v);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            low[u] = min(low[u], low[v]);
        }

            // Update low value of 'u' only of 'v' is still in stack
            // (i.e. it's a back edge, not cross edge).
            // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v]) {
            low[u] = min(low[u], disc[v]);
        }
    }

    // head node found, pop the stack and print an SCC
    NodeID w = 0; // To store stack extracted vertices
    if (low[u] == disc[u])
    {
        while (st.top() != u)
        {
            w = (int) st.top();
            this->SCC_components[ctrSCC].push_back(w);
            //cout << w << " ";
            components[w]=ctrSCC;
            stackMember[w] = false;
            st.pop();
        }
        w = (int) st.top();
        SCC_components[this->ctrSCC].push_back(w);
        //cout << w << "\n" << " new SCC ";
        SCC_components.emplace_back();
        this->ctrSCC++;
        stackMember[w] = false;
        components[w]=ctrSCC;
        st.pop();
    }
}

int SCC::GetSCC()
{
    // Initialize disc and low, and stackMember arrays
    for (NodeID i = 0; i < numNodes; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        stackMember[i] = false;
    }

    vector<NodeID> tmp;
    this->SCC_components.push_back(tmp);
    for (int i = 0; i < numNodes; i++) {
        if (disc[i] == -1){
            this->ComputeSCC(i);
        }
    }
    return this->ctrSCC;
}

int SCC::GetSCC_Number() const
{
    return ctrSCC;
}


#endif //BREAKINGCYCLES_SCC_H
