//
// Created by Mert Biyikli on 07.03.22.
//

#include "graph.h"


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
std::array<int, 2> Graph::MaxDegree() const
{
    std::array<int, 2> Best = {0,0};
    for(NodeID i = 0; i<graph.size(); i++)
    {
        if(graph[i].size()>Best[1])
        {
            Best[1] = graph[i].size();
            Best[0] = i;
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

