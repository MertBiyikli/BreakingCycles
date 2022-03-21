//
// Created by Mert Biyikli on 17.03.22.
//

#ifndef BREAKINGCYCLES_SUBGRAPH_H
#define BREAKINGCYCLES_SUBGRAPH_H

#include "graph.h"
#include "../algorithms/SCC.h"
#include <map>

class SubGraph{ // TODO: Even there arent any errors, this is still very hard to handle for other instances to catch up with the indices...
public:
    Graph G;
    vector<Graph> subGraphs;
    //std::map<Graph, vector<pair<int, int> > > M;
    vector<pair<Graph, vector<pair<int, int> > > > M;
    void addSubGraph( vector<bool>&);
    SubGraph(Graph& G, Tarjan& S);
    vector<vector<NodeID> > scc;
    bool isCorrect();
    void printSubGraph(int i) const;
    bool checkValuesAreOkay(int) const;
    int ctr;
};

SubGraph::SubGraph(Graph &G, Tarjan& S)
{
    ctr=0;
    this->G = G;
    scc = S.getSCC();
    for(auto i : scc)
    {
        if(i.size()>1)
        {
            vector<bool> included(G.GetNumVertices(), false);
            for(auto it : i)
            {
                included[it]=true;
            }
            cout << "everyting fine before the addSubGraph call" << endl;
            this->addSubGraph( included);
        }
    }
}

void SubGraph::addSubGraph(vector<bool>& vec) {
    Graph G_ = this->G;
    /*
    cout << "we are in the addSubGraph" << endl;
    for(NodeID it = 0; it<vec.size(); it++)
    {
        cout << "Before the if " << endl;
        if(!vec[it])
        {
            cout << "in the if" << endl;
            G_.graph.erase(G_.graph.begin()+it);
            cout << G_.graph.size() << endl;
            G_.numVertices--;
        }else {
            cout << "in the second case" << endl;
            for (NodeID u=0; u<G_.graph[it].size(); u++)
            {
                if(!vec[u])
                {
                    cout <<  G_.graph[it].size() << ": " << G_.graph[it][u] << endl;
                    G_.graph.at(it).erase(G_.graph.at(it).begin()+u);
                    cout << "after the removal" << G_.graph.at(it).size() << endl;
                    G_.numEdges--;
                }
            }
        }
    }
    cout << "just before pushing the graph to the vector"<< endl;
    this->subGraphs.push_back(G_);*/

    vector<pair<int, int> > map;
    for (NodeID v = 0;  v<G_.graph.size(); v++) {
        if (!vec[v]) {
            map.emplace_back(v, ctr);
            ctr++;
            G_.graph.erase(G_.graph.begin() + v);
            //map.push_back(make_pair(v, ctr));
            G_.numVertices--;
        }
    }

    for (NodeID v = 0;  v<G_.graph.size(); v++)
    {
        if(!vec[v]) {

            map.emplace_back(v, ctr);
            ctr++;
            G_.graph.erase(G_.graph.begin() + v);
            //map.push_back(make_pair(v, ctr));
            G_.numVertices--;
        }else {
            for (NodeID u = 0; u < G_.graph[v].size(); u++) {
                if (!vec[G_.graph[v][u]]) {
                    G_.graph[v].erase(G_.graph[v].begin() + u);
                    G_.numEdges--;
                }
            }
        }
    }
    this->subGraphs.push_back(G_);
    this->M.push_back(make_pair(G_, map));
}

bool SubGraph::isCorrect()
{
    int ctr=0;
    for(auto i : scc)
    {
        if(i.size()>1)
        {
            ctr++;
        }
    }
    return (ctr==this->subGraphs.size());
}


void SubGraph::printSubGraph(int i) const{
    for(NodeID v = 0; v<this->subGraphs[i].graph.size(); v++)
    {
        if(subGraphs[i].graph[v].size()>1)
        {
            cout << v << ": ";
            for(auto u : subGraphs[i].neighbors(v))
            {
                cout << u << " ";
            }
            cout << endl;
        }
    }
}

bool SubGraph::checkValuesAreOkay(int index) const
{
    bool result = true;
    for(NodeID v = 0; v<subGraphs[index].graph.size(); v++)
    {
        for(auto u : subGraphs[index].neighbors(v))
        {
            if(u>subGraphs[index].graph.size())
            {
                result = false;
            }
        }
    }
    return result;
}


#endif //BREAKINGCYCLES_SUBGRAPH_H
