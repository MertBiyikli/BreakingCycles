//
// Created by Mert Biyikli on 17.03.22.
//

#ifndef BREAKINGCYCLES_SUBGRAPH_H
#define BREAKINGCYCLES_SUBGRAPH_H

#include "graph.h"
#include "../algorithms/SCC.h"
#include <map>

class Subgraph{
public:
    Graph G;
    map<NodeID , int> M; // the first value: index in original graph; second is index for the subgraph
    Subgraph(Graph& G_, vector<bool>& included);
    Subgraph(Graph& G_, const NodeID& v);
    //Subgraph(Graph& G_, vector<bool>& included, bool merged);
    pair<Graph, map<NodeID, int> > GetSubGraph();
};

Subgraph::Subgraph(Graph &G_, vector<bool> &included) {
    int ctr = 0;
    for (int I=0; I<included.size(); I++) {
        if (included[I]) {
            M.insert({I, ctr});
            ctr++;
        }
    }

    this->G.graph.resize(ctr);
    this->G.numVertices=ctr;

    for(NodeID v = 0; v<G_.GetNumVertices(); v++) {
        if(included[v]) {
            pair<int, int> tmp;
            for(auto i : M)
            {
                if(i.first==v)
                {
                    tmp = i;
                }
            }
            for (NodeID u = 0; u < G_.graph[v].size(); u++) {
                if(included[G_.graph[v][u]])
                {
                    pair<int, int> tmp_2;
                    for(auto I : M)
                    {
                        if(I.first == G_.graph[v][u])
                        {
                            tmp_2=I;
                        }
                    }
                    G.graph[tmp.second].push_back(tmp_2.second);
                    G.numEdges++;
                    }
                }
            }
        }
    }

Subgraph::Subgraph(Graph &G_, const NodeID &v)
{
    this->G = G_;
    for(NodeID it = 0; it<G.GetNumVertices(); it++)
    {
        if(it == v)
        {
            G.graph.erase(G.graph.begin()+it);
            continue;
        }
        for(NodeID jt = 0; jt<G.graph[it].size(); jt++)
        {
            NodeID u = G.graph[it][jt];
            if(u==v)
            {
                G.graph[it].erase(G.graph[it].begin()+jt);
            }
        }
    }
}
/*
Subgraph::Subgraph(Graph &G_, vector<bool> &included, bool merged)
{





    for(NodeID v = 0; v<G.GetNumVertices(); v++) {
        vector<NodeID> InComingNeig;
        vector<NodeID> OutGoing = G.graph[maxV.first];
        for (NodeID v = 0; v < G.GetNumVertices(); v++) {
            for (auto u : G.neighbors(v)) {
                if (u == maxV.first) {
                    InComingNeig.push_back(v);
                }
            }
        }
        Graph rightGraph = Subgraph(G, left).GetSubGraph().first;
        for (NodeID In : InComingNeig) {
            for (NodeID Out : OutGoing) {
                bool EdgeExists = false;
                for (auto it : rightGraph.graph[In]) {
                    if (it == Out) {
                        EdgeExists = true;
                    }
                }
                if (!EdgeExists) {
                    rightGraph.graph[In].push_back(Out);
                }
            }
        }
    }
}*/

pair<Graph, map<NodeID, int> >  Subgraph::GetSubGraph()
{
    return make_pair(this->G, this->M);
}
#endif //BREAKINGCYCLES_SUBGRAPH_H
