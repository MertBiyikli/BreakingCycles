//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_GRAPH_H
#define BREAKINGCYCLES_GRAPH_H

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <array>
#include <list>
#include <stack>
#include <queue>

typedef int NodeID;
typedef int EdgeID;

using namespace std;

template<typename IteratorT>
class IteratorRange {
public:
    IteratorRange(const IteratorT& first, const IteratorT& firstInvalid) :
            __begin(first),
            __end(firstInvalid) { }

    using Iterator = IteratorT; // make publicly visible

    IteratorT begin() {
        return __begin;
    }

    IteratorT end() {
        return __end;
    }

    bool empty() {
        return __begin == __end;
    }

private:
    IteratorT __begin, __end;
};

class Graph
{
    using NeighborIterator = typename std::vector<int>::const_iterator;
    using NeighborRange = IteratorRange<NeighborIterator>;
public:
    std::vector<std::vector<NodeID> > graph;
    vector<int> InDegree;

    void InitTheGraph(const NodeID& n, const EdgeID& m);
    int GetNumEdges() const;
    int GetNumVertices() const;
    pair<NodeID, int> MaxDegree() const;
    int GetDegree(NodeID) const;
    NodeID numVertices;
    EdgeID numEdges;
    NeighborRange neighbors(const int u) const {
        return NeighborRange(
                graph[u].cbegin(), graph[u].cend());
    }

    void print();
    bool isTournemt();

    bool IsConnected();
    int GetInDegree(NodeID u);
    vector<int> Indegree();

    bool DFS(NodeID u, NodeID v);
};

vector<int> Graph::Indegree()
{
    vector<int> Indegree(this->GetNumVertices(), 0);
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        for(auto u : this->neighbors(v))
        {
            Indegree[u]++;
        }
    }
    return Indegree;
}


void Graph::InitTheGraph(const NodeID &n, const EdgeID &m)
{
    this->numEdges = m;
    this->numVertices = n;
    this->graph.resize(this->numVertices);
/*
    this->InDegree.resize(this->GetNumVertices(), 0);
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        for(NodeID u : this->neighbors(v))
        {
            InDegree[u]++;
        }
    }*/
}

int Graph::GetNumEdges() const
{
    return this->numEdges;
}

int Graph::GetNumVertices() const
{
    return this->numVertices;
}


// first one is the is the node and the second is the degree itself
pair<NodeID , int> Graph::MaxDegree() const
{
    auto Best = std::make_pair(0, 0);
    for(NodeID i = 0; i<numVertices; i++)
    {
        if(graph[i].size()>Best.second)
        {
            Best.second = graph[i].size();
            Best.first = i;
        }
    }
    return Best;
}

int Graph::GetDegree(NodeID n) const
{
    if(n<0 || n> this->GetNumVertices())
    {
        std::cerr << "Not existing node" << std::endl;
        return -1;
    }
    return graph[n].size();
}

int Graph::GetInDegree(NodeID u)
{
    if(u<0 || u> this->GetNumVertices())
    {
        std::cerr << "Not existing node" << std::endl;
        return -1;
    }
    return InDegree[u];
}






bool Graph::IsConnected()
{
    vector<bool> visited(numVertices, false);
    cout << "Problem after the visited" << endl;
    for(NodeID u = 0; u<this->GetNumVertices(); u++)
    {
        for(NodeID v = 0; v<this->graph[u].size(); v++)
        {
            visited[this->graph[u][v]]=true;
        }
    }
    cout << "Problem after the 1st loop" << endl;
    for(auto i : visited)
        if(!i)
            return false;
    cout << "Problem after the 2nd loop" << endl;
    return true;
}


bool isAcyclic(const Graph& graph, const std::vector<bool>& fvs) {
    int fvs_size = 0;
    for ( size_t i = 0; i < fvs.size(); ++i ) {
        fvs_size += fvs[i];
    }

    std::vector<int> in_degree(graph.GetNumVertices(), 0);
    for ( int u = 0; u < graph.GetNumVertices(); ++u ) {
        if ( !fvs[u] ) {
            for ( const int v : graph.neighbors(u) ) {
                ++in_degree[v];
            }
        }
    }

    std::queue<int> q;
    for ( int u = 0; u < graph.GetNumVertices(); ++u ) {
        if ( !fvs[u] && in_degree[u] == 0 ) {
            q.push(u);
        }
    }

    int visited_nodes = 0;
    while ( !q.empty() ) {
        int u = q.front();
        q.pop();
        ++visited_nodes;

        for ( const int v : graph.neighbors(u) ) {
            --in_degree[v];
            if ( !fvs[v] && in_degree[v] == 0 ) {
                q.push(v);
            }
        }
    }

    return visited_nodes == (graph.GetNumVertices() - fvs_size);
}

bool Graph::isTournemt() {
    bool result = true;
    vector<vector<NodeID> > AdjacenzMatrix(this->GetNumVertices(), vector<NodeID>(this->GetNumVertices(),0));
    for(NodeID v = 0; v<this->numVertices; v++){
        for(auto u : neighbors(v)){
            AdjacenzMatrix[v][u]=1;
        }
    }
    for (NodeID v = 0; v < this->GetNumVertices(); v++){
        for(NodeID u = 0; u<this->GetNumVertices(); u++)
        {
            if(v==u)
            {
                continue;
            }
            if(AdjacenzMatrix[v][u]==0 && AdjacenzMatrix[u][v]==0)
            {
                result= false;
            }
        }
    }
    return result;
}

bool Graph::DFS(NodeID u, NodeID v)
{
    stack<NodeID> S;
    S.push(u);
    vector<bool> visited(this->GetNumVertices(), false);
    while(!S.empty())
    {
        NodeID tmp = S.top();
        S.pop();
        if(!visited[tmp])
        {
            visited[tmp]=true;
            if(tmp==v)
                return true;
            for(NodeID neig : this->neighbors(tmp))
            {
                S.push(neig);
            }
        }
    }
    return false;
}

void Graph::print() {
    for(NodeID v = 0; v<numVertices; v++)
    {
        cout << v <<": ";
        for(auto u : this->neighbors(v))
        {
            cout << u << " ";
        }
        cout << endl;
    }
}

#endif //BREAKINGCYCLES_GRAPH_H
