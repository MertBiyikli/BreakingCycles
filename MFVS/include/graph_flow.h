//
// Created by Mert Biyikli on 04.04.22.
//

#ifndef BREAKINGCYCLES_GRAPH_FLOW_H
#define BREAKINGCYCLES_GRAPH_FLOW_H

#include "../include/graph.h"

using namespace std;

typedef int ReNodeID;
typedef int Flow;
typedef int Cap;

struct ReEdge{
    Flow flow;
    Cap capacity;
    ReNodeID u, v;

    ReEdge(const Flow & flow, const Cap& capacity, const ReNodeID & u, const ReNodeID & v)
    {
        this->flow = flow;
        this->capacity = capacity;
        this->u = u;
        this->v = v;
    }
};

struct ReVertex{
    int h, e_flow;

    ReVertex(const int& h, const int& e_flow)
    {
        this->h = h;
        this->e_flow = e_flow;
    }
};

class FlowGraph{
public:
    Graph G;
    int numNodes, numEdges;
    ReNodeID superSource, superSink;
    vector<ReVertex> vertices;
    vector<ReEdge> edges;
    bool push(Flow);
    bool relabel(Flow);
    void preflow(ReNodeID s);
    void updateReverseEdgeFlow(ReNodeID i,Flow flow);
    FlowGraph();
    int Init(Graph& G, vector<NodeID> sources, vector<NodeID> sinks);
    void AddEdge(ReNodeID u, ReNodeID v, int w);
    int getMaxFlow();
};

FlowGraph::FlowGraph() {}

int FlowGraph::Init(Graph &G, vector<NodeID> sources, vector<NodeID> sinks) {
    for(int i = 0; i<G.GetNumVertices()+2; i++)
    {
        vertices.push_back(ReVertex(0,0));
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(NodeID u : G.neighbors(v))
        {
            this->AddEdge(v, u, 1);
        }
    }

    // adding the superSource and SuperSource for multiple sources and sinks
    this->superSource = G.GetNumVertices()+1;
    this->superSink = G.GetNumVertices()+2;
    vertices.push_back(ReVertex(0, 0));
    vertices.push_back(ReVertex(0, 0));
    for(auto src : sources)
    {
        this->AddEdge(superSource, src, INT_MAX);
    }
    for(auto sks : sinks)
    {
        this->AddEdge(sks, superSource, INT_MAX);
    }
}


void FlowGraph::AddEdge(ReNodeID u, ReNodeID v, int w)
{
    edges.push_back(ReEdge(0, w, u, v));
}


void FlowGraph::preflow(ReNodeID s)
{
    vertices[s].h = vertices.size();

    //
    for (int i = 0; i < edges.size(); i++)
    {
        // If current edge goes from source
        if (edges[i].u == s)
        {
            // Flow is equal to capacity
            edges[i].flow = edges[i].capacity;

            // Initialize excess flow for adjacent v
            vertices[edges[i].v].e_flow += edges[i].flow;

            // Add an edge from v to s in residual graph with
            // capacity equal to 0
            edges.push_back(ReEdge(-edges[i].flow, 0, edges[i].v, s));
        }
    }
}


void FlowGraph::updateReverseEdgeFlow(ReNodeID i, Flow flow)
{
    int u = edges[i].v, v = edges[i].u;

    for (int j = 0; j < edges.size(); j++)
    {
        if (edges[j].v == v && edges[j].u == u)
        {
            edges[j].flow -= flow;
            return;
        }
    }

    // adding reverse Edge in residual graph
    ReEdge e = ReEdge(0, flow, u, v);
    edges.push_back(e);
}


bool FlowGraph::push(ReNodeID u)
{
    for (int i = 0; i < edges.size(); i++)
    {
        // Checks u of current edge is same as given
        // overflowing vertex
        if (edges[i].u == u)
        {
            // if flow is equal to capacity then no push
            // is possible
            if (edges[i].flow == edges[i].capacity)
                continue;

            // Push is only possible if height of adjacent
            // is smaller than height of overflowing vertex
            if (vertices[u].h > vertices[edges[i].v].h)
            {
                // Flow to be pushed is equal to minimum of
                // remaining flow on edge and excess flow.
                int flow = min(edges[i].capacity - edges[i].flow,
                               vertices[u].e_flow);

                // Reduce excess flow for overflowing vertex
                vertices[u].e_flow -= flow;

                // Increase excess flow for adjacent
                vertices[edges[i].v].e_flow += flow;

                // Add residual flow (With capacity 0 and negative
                // flow)
                edges[i].flow += flow;

                updateReverseEdgeFlow(i, flow);

                return true;
            }
        }
    }
    return false;
}


bool FlowGraph::relabel(Flow u) {
    // Initialize minimum height of an adjacent
    int mh = INT_MAX;

    // Find the adjacent with minimum height
    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i].u == u)
        {
            // if flow is equal to capacity then no
            // relabeling
            if (edges[i].flow == edges[i].capacity)
                continue;

            // Update minimum height
            if (vertices[edges[i].v].h < mh)
            {
                mh = vertices[edges[i].v].h;

                // updating height of u
                vertices[u].h = mh + 1;
            }
        }
    }
}

int overFlowVertex(vector<ReVertex>& ver);

int FlowGraph::getMaxFlow()
{
    preflow(superSource);

    // loop until none of the Vertex is in overflow
    while (overFlowVertex(vertices) != -1)
    {
        cout << vertices.back().e_flow << endl;
        int u = overFlowVertex(vertices);
        if (!push(u))
            relabel(u);
    }

    // ver.back() returns last Vertex, whose
    // e_flow will be final maximum flow
    return vertices.back().e_flow;
}

int overFlowVertex(vector<ReVertex>& ver)
{
    for (int i = 1; i < ver.size() - 1; i++)
        if (ver[i].e_flow > 0)
            return i;

    // -1 if no overflowing Vertex
    return -1;
}

#endif //BREAKINGCYCLES_GRAPH_FLOW_H
