//
// Created by Mert Biyikli on 04.04.22.
//

#ifndef BREAKINGCYCLES_DINICS_H
#define BREAKINGCYCLES_DINICS_H
#include <list>
#include "../include/graph.h"

using namespace std;


typedef int NodeID;
typedef int Flow;
typedef int Cap;


struct Edge
{
    NodeID v ;
    Flow flow ;
    Cap C;
    int rev;
};

// TODO: Implement a constructor s.t. when passing a vec of sources and sinks init the Dinic accordingly

// Residual Graph
class Dinics
{
public :
    int V; // number of vertex
    int *level ; // stores level of a node
    vector< Edge > *adj;

    Dinics(int V)
    {
        adj = new vector<Edge>[V];
        this->V = V;
        level = new int[V];
    }

    Dinics(Graph& G)
    {
        adj = new vector<Edge>[G.GetNumEdges()];
        this->V = G.GetNumVertices();
        level = new int[V];
        /*
        adj = new vector<Edge>[G.GetNumEdges()+2];
        this->V = G.GetNumVertices()+2;
        level = new int[V+2];

        // adding the superSource and superSinks
        auto src = G.GetSources();
        auto sks = G.GetSinks();
        NodeID superSource=G.GetNumVertices()+1, superSink = G.GetNumVertices()+2;
        for(auto s : src)
        {
            this->addEdge(superSource, s, INT_MAX);
        }
        for(auto t : sks)
        {
            this->addEdge(t, superSink, INT_MAX);
        }*/
        for(NodeID v = 0; v<G.GetNumVertices(); v++)
        {
            for(NodeID u : G.neighbors(v))
            {
                this->addEdge(v,u, 1);
            }
        }
    }

    void print()
    {
    for(NodeID v = 0; v<V; v++)
    {
        cout << v << ": ";
        for(auto u : adj[v])
        {
            cout << "(" << u.v << "/" << u.rev << "/" << u.C << "/" << u.flow <<")";
        }
        cout << endl;
    }
    }

    // add edge to the graph
    void addEdge(int u, int v, int C)
    {
        // Forward edge : 0 flow and C capacity
        Edge a{v, 0, C, static_cast<int>(adj[v].size())};

        // Back edge : 0 flow and 0 capacity
        Edge b{u, 0, 0, static_cast<int>(adj[u].size())};

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

    bool BFS(int s, int t);
    int sendFlow(int s, int flow, int t, int ptr[]);
    int DinicMaxflow(int s, int t);
};



// Finds if more flow can be sent from s to t.
// Also assigns levels to nodes.
bool Dinics::BFS(int s, int t)
{
    for (int i = 0 ; i < V ; i++)
        level[i] = -1;

    level[s] = 0; // Level of source vertex

    // Create a queue, enqueue source vertex
    // and mark source vertex as visited here
    // level[] array works as visited array also.
    list< int > q;
    q.push_back(s);

    vector<Edge>::iterator i ;
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++)
        {
            Edge &e = *i;
            if (level[e.v] < 0 && e.flow < e.C)
            {
                // Level of current vertex is,
                // level of parent + 1
                level[e.v] = level[u] + 1;

                q.push_back(e.v);
            }
        }
    }

    // IF we can not reach to the sink we
    // return false else true
    return level[t] < 0 ? false : true ;
}



int Dinics::DinicMaxflow(int s, int t)
{
    // Corner case
    if (s == t)
        return -1;

    int total = 0; // Initialize result
    cout << s << " " << t << endl;
    // Augment the flow while there is path
    // from source to sink
    while (BFS(s, t) == true)
    {
        cout << BFS(s, t) << endl;
        // store how many edges are visited
        // from V { 0 to V }
        int *start = new int[V+1] {0};
        // while flow is not zero in graph from S to D
        while (int flow = sendFlow(s, INT_MAX, t, start)) {
            // Add path flow to overall flow
            cout << "flow: " << flow << endl;
            total += flow;
        }
    }

    // return maximum flow
    return total;
}


int Dinics::sendFlow(int u, int flow, int t, int start[])
{
    // Sink reached
    if (u == t)
        return flow;

    // Traverse all adjacent edges one -by - one.
    for (  ; start[u] < adj[u].size(); start[u]++)
    {
        // Pick next edge from adjacency list of u
        Edge &e = adj[u][start[u]];

        if (level[e.v] == level[u]+1 && e.flow < e.C)
        {
            // find minimum flow from u to t
            int curr_flow = min(flow, e.C - e.flow);

            int temp_flow = sendFlow(e.v, curr_flow, t, start);

            // flow is greater than zero
            if (temp_flow > 0)
            {
                // add flow  to current edge
                e.flow += temp_flow;

                // subtract flow from reverse edge
                // of current edge
                adj[e.v][e.rev].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}


#endif //BREAKINGCYCLES_DINICS_H
