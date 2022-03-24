//
// Created by Mert Biyikli on 07.03.22.
//

#include "graph.h"
/*

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
*/