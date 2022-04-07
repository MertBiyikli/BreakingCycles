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

using namespace std;
typedef int NodeID;

typedef int EdgeID;

typedef pair<vector<bool>, int> FVS;

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

    void InitTheGraph(const NodeID& n, const EdgeID& m);
    int GetNumEdges() const;
    int GetNumVertices() const;
    pair<NodeID, int> MaxDegree() const;
    int GetDegree(NodeID) const;
    NodeID numVertices;
    EdgeID numEdges;
    void deleteVertex(NodeID);
    NeighborRange neighbors(const int u) const {
        return NeighborRange(
                graph[u].cbegin(), graph[u].cend());
    }

    void print() const;
    bool DFS(NodeID u, NodeID v);
    vector<NodeID> GetSources();
    vector<NodeID> GetSinks();

    // this for the case that we are exluding some vertices
    vector<NodeID> GetSources(const vector<bool>&);
    vector<NodeID> GetSinks(const vector<bool>&);


    //for in and outgoing neighbours
    vector<NodeID> InGoing(NodeID v, const vector<bool>& inc);
    vector<NodeID> OutGoing(NodeID v, const vector<bool>& inc);


    vector<NodeID> InGoing(NodeID v);
    vector<NodeID> OutGoing(NodeID v);

    int GetDegree(NodeID n, const vector<bool>& inc) const;

    //bool isInLoop(NodeID);
};

bool isAcyclic(const Graph& graph, const std::vector<bool>& fvs);

void Graph::InitTheGraph(const NodeID &n, const EdgeID &m)
{
    this->numEdges = m;
    this->numVertices = n;
    this->graph.resize(this->numVertices);
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






bool isAcyclic(const Graph& graph, const std::vector<bool>& fvs) {
    int fvs_size = 0;
    for ( size_t i = 0; i < fvs.size(); ++i ) {
        fvs_size += fvs[i];
    }

    std::vector<int> in_degree(graph.GetNumVertices(), 0);
    for ( int u = 0; u < graph.GetNumVertices(); u++ ) {
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



bool isAcyclic(const Graph& graph, const std::vector<bool>& fvs, const vector<bool>& inc) {
    int fvs_size = 0;
    for ( size_t i = 0; i < fvs.size(); ++i ) {
        fvs_size += fvs[i];
    }

    std::vector<int> in_degree(graph.GetNumVertices(), 0);
    for ( int u = 0; u < graph.GetNumVertices(); u++ ) {
        if(inc[u]) {
            if (!fvs[u]) {
                for (const int v : graph.neighbors(u)) {
                    ++in_degree[v];
                }
            }
        }
    }

    std::queue<int> q;
    for ( int u = 0; u < graph.GetNumVertices(); ++u ) {
        if(inc[u]) {
            if (!fvs[u] && in_degree[u] == 0) {
                q.push(u);
            }
        }
    }

    int visited_nodes = 0;
    while ( !q.empty() ) {
        int u = q.front();
        q.pop();
        ++visited_nodes;

        for ( const int v : graph.neighbors(u) ) {
            if (inc[v]) {
                --in_degree[v];
                if (!fvs[v] && in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }
    }

    return visited_nodes == (graph.GetNumVertices() - fvs_size);
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

void Graph::print() const {
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        cout << v <<": ";
        for(auto u : this->neighbors(v))
        {
            cout << u << " ";
        }
        cout << endl;
    }
}

void Graph::deleteVertex(NodeID v)
{
    this->numVertices--;
    this->graph.erase(graph.begin()+v);
    for(NodeID it = 0; it<this->graph.size(); it++)
    {
        for(NodeID u=0; u< this->graph[it].size(); u++)
        {
            if(graph[it][u]==v)
            {
                graph[it].erase(graph[it].begin()+u);
            }
        }
    }
}

vector<NodeID> Graph::GetSources()
{
    vector<NodeID> srcs;
    vector<int> indegree(this->GetNumVertices(), 0);
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        for(NodeID u : this->neighbors(v))
        {
            indegree[u]++;
        }
    }
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        if(indegree[v]==0)
        {
            srcs.push_back(v);
        }
    }
    return srcs;
}



vector<NodeID> Graph::GetSinks()
{
    vector<NodeID> sks;
    for(NodeID v = 0; v< this->GetNumVertices(); v++)
    {
        if(this->GetDegree(v)==0)
        {
            sks.push_back(v);
        }
    }
    return sks;
}

vector<NodeID> Graph::GetSources(const vector<bool> & inc) {
    vector<NodeID> srcs;
    vector<NodeID> degree(this->GetNumVertices(), -1);
    for(int it = 0; it<inc.size(); it++)
    {
        if(inc[it])
        {
            degree[it]=0;
        }
    }
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        if(inc[v]) {
            for (NodeID u : this->neighbors(v)) {
                if(inc[u])
                {
                    degree[v]++;
                }
            }
        }
    }

    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        if(degree[v]==0)
        {
            srcs.push_back(v);
        }
    }
    return srcs;
}


vector<NodeID> Graph::GetSinks(const vector<bool> &inc) {
    vector<NodeID> sks;
    vector<int> indegree(this->GetNumVertices(), -1);
    for (int it = 0; it < this->GetNumVertices(); it++) {
        if (inc[it]) {
            indegree[it] = 0;
        }
    }

    for (NodeID v = 0; v < this->GetNumVertices(); v++)
    {
        if(inc[v]) {
            for (NodeID u : this->neighbors(v)) {
                if(inc[u])
                {
                    indegree[u]++;
                }
            }
        }
    }

    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        if(indegree[v]==0)
        {
            sks.push_back(v);
        }
    }
    return sks;
}

vector<NodeID> Graph::InGoing(NodeID _v_, const vector<bool> &inc)
{
    vector<NodeID> ingoing;
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
        if(inc[v]) {
            for (NodeID u : this->neighbors(v)) {
                if(inc[u])
                {
                    if(u == _v_)
                    {
                        ingoing.push_back(v);
                    }
                }
            }
        }
    }
    return ingoing;
}


vector<NodeID> Graph::InGoing(NodeID _v_)
{
    vector<NodeID> ingoing;
    for(NodeID v = 0; v<this->GetNumVertices(); v++)
    {
            for (NodeID u : this->neighbors(v))
                {
                    if(u == _v_)
                    {
                        ingoing.push_back(v);
                    }
                }
            }
    return ingoing;
}


vector<NodeID> Graph::OutGoing(NodeID v, const vector<bool> &inc)
{
    vector<NodeID> outgoing;
    for(NodeID u : this->neighbors(v))
    {
        if(inc[u])
        {
            outgoing.push_back(u);
        }
    }
    return outgoing;
}


vector<NodeID> Graph::OutGoing(NodeID v)
{
    vector<NodeID> outgoing;
    for(NodeID u : this->neighbors(v))
    {
        outgoing.push_back(u);
    }
    return outgoing;
}


int Graph::GetDegree(NodeID n, const vector<bool>& inc) const {
    int Degree = 0;
    for(NodeID u : this->neighbors(n))
    {
        if(inc[u])
        {
            Degree++;
        }
    }
    return Degree;
}

#endif //BREAKINGCYCLES_GRAPH_H
