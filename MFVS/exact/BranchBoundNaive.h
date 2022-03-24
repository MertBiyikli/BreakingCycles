//
// Created by Mert Biyikli on 24.03.22.
//

#ifndef BREAKINGCYCLES_BRANCHBOUNDNAIVE_H
#define BREAKINGCYCLES_BRANCHBOUNDNAIVE_H

#include "../algorithms/SCC.h"
#include "../include/graph.h"
#include "../include/subgraph.h"
#include "../heuristic/MakeMinimal.h"

vector<int> reduce_(Graph& G, bool applyIn0,  bool applyOut0, bool applyLoop,
                    bool applyIn1,  bool applyOut1, bool applyPie,
                    bool applyCore, bool applyDome, bool verbose);

void deleteVertices(Graph& G, const vector<int>& vertices);


vector<int> in0(Graph& G);

vector<int> out0(Graph& G);


vector<NodeID> sources(const Graph& G)
{
    vector<NodeID> srcs;
    vector<NodeID> indegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        for(NodeID u : G.neighbors(v))
        {
            indegree[u]++;
        }
    }

    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0)
        {
            srcs.push_back(v);
        }
    }
    return srcs;
}

vector<NodeID> sinks(const Graph& G)
{
    vector<NodeID> sks;
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(G.GetDegree(v)==0)
        {
            sks.push_back(v);
        }
    }
    return sks;
}

vector<NodeID> OutGoingNeig(Graph& G, const NodeID& v)
{
    return G.graph[v];
}

vector<NodeID> IncomingNeig(Graph& G, const NodeID& v)
{
    vector<NodeID> in;
    for(NodeID it = 0; it<G.GetNumVertices(); it++)
    {
        for(auto u : G.neighbors(it))
        {
            if(u==v)
            {
                in.push_back(it);
            }
        }
    }
    return in;
}


void deleteVertex(Graph& G, NodeID vertex) {
    if (vertex>=0 && vertex<G.GetNumVertices() ) {
        //vector<int> outN = OutGoingNeig(G, vertex);
        vector<bool> out(G.GetNumVertices(), false);
        for(auto it : OutGoingNeig(G, vertex))
        {
            out[it]=true;
        }
        //vector<int> inN  = IncomingNeig(G,vertex);
        vector<bool> in(G.GetNumVertices(), false);
        for(auto it : IncomingNeig(G, vertex))
        {
            in[it]=true;
        }
        for(NodeID u = 0; u<G.graph[vertex].size(); u++)
        {
            if(out[G.graph[vertex][u]])
            {
                G.graph[vertex].erase(G.graph[vertex].begin()+u);
                G.numEdges--;
            }
        }
        for(NodeID v = 0; v<G.GetNumVertices(); v++)
        {
            for(NodeID u = 0; u<G.graph[v].size(); u++)
            {
                NodeID u_tmp = G.graph[v][u];
                if(in[u_tmp])
                {
                    G.graph[v].erase(G.graph[v].begin()+u);
                    G.numEdges--;
                }
            }
        }
        G.graph.erase(G.graph.begin()+vertex);
        G.numVertices--;
    }
}

void deleteVertices(Graph& G, const vector<int>& vertices) {
    for (auto& it : vertices)
        deleteVertex(G,it);
}


vector<int> in0(Graph& G) {
    vector<int> in0Vertices = sources(G);
    if(!in0Vertices.empty()) {
        for (auto it : in0Vertices) {
            G.graph.erase(G.graph.begin() + it);
        }
        return in0Vertices;
    }
}

vector<NodeID> out0(Graph& G)
{
    vector<NodeID> out0vertices = sinks(G);
    vector<bool> out(G.GetNumVertices(), false);
    for(auto it : out0vertices)
    {
        out[it]=true;
    }
    if(!out0vertices.empty())
    {
        vector<int> indegree(G.GetNumVertices(), 0);
        for(NodeID v = 0; v<G.GetNumVertices(); v++){
            for(NodeID u_=0; u_<G.graph[v].size(); u_++)
            {
                NodeID u = G.graph[v][u_];
                if(out[u])
                {
                    G.graph[v].erase(G.graph[v].begin()+u_);
                }
            }
        }
        return out0vertices;
    }

}


vector<int> reduce_(const Graph& G) {
    vector<int> solution;
    vector<int> in0Vertices = sinks(G);
    vector<int> out0Vertices = sources(G);
    return solution;
}

vector<NodeID> _shortestCycle(Graph& G, NodeID vertex, int maxLength)
{
    queue< list<int> > paths;
    list<int> path;
    path.push_back(vertex);
    paths.push(path);
    bool cycleFound = false;
    while (!cycleFound && !paths.empty()) {
        path = paths.front();
        paths.pop();
        cycleFound = int(path.size()) > 1 && path.front() == path.back();
        if (!cycleFound && int(path.size()) - 1 < maxLength) {
            vector<int> outN = OutGoingNeig(G, path.back());
            for (vector<int>::const_iterator it = outN.begin();
                 it != outN.end(); ++it) {
                path.push_back(*it);
                paths.push(path);
                path.pop_back();
            }
        }
    }
    vector<int> cycle(int(path.size()));
    copy(path.begin(), path.end(), cycle.begin());
    return cycle;
}

vector<NodeID> ShortestCycle(Graph& G)
{
    Graph h = G;
    if (isAcyclic(h, vector<bool>(h.GetNumVertices(), true))) {
        throw "No shortest cycle ! This graph is acyclic";
    } else {
        int n;
        while (n != h.GetNumVertices()) {
            n = h.GetNumVertices();
            in0(h);
            out0(h);
        }
        vector<int> shortestCycle;
        vector<int> currentCycle;
        int maxLength = h.GetNumVertices();
        for (NodeID it = 0; it<h.GetNumVertices(); it++) {
            currentCycle = _shortestCycle(h, it, maxLength);
            if (int(currentCycle.size() - 1)  < maxLength) {
                shortestCycle = currentCycle;
                maxLength = int(currentCycle.size()) - 1;
            }
        }
        return shortestCycle;
    }
}


void mergeVertex(Graph& G, NodeID vertex) {
    if (G.GetDegree(vertex)>0) {
        vector<int> inN = IncomingNeig(G, vertex);
        vector<int> outN = OutGoingNeig(G, vertex);
        for (vector<int>::iterator it = inN.begin(); it != inN.end(); ++it) {
            bool EdgeExists = false;
            for (vector<int>::iterator jt = outN.begin(); jt != outN.end(); ++jt) {
                for (auto ii : G.neighbors(*it)) {
                    if (ii == *jt) {
                        EdgeExists = true;
                    }
                }
                if (!EdgeExists) {
                    G.graph[*it].push_back(*jt);
                }
            }
        }
        deleteVertex(G, vertex);
    }else throw "The vertex does not exist.";
}

class BranchBoundNaive{
public:
    Graph G;
    BranchBoundNaive(Graph& G);
    pair<Subgraph, vector<bool> > reduce(Graph& G, vector<bool>& included);
    int upperBound(Graph& G,  vector<bool>& included);
    int lowerBound(Graph& G,  vector<bool>& included);
    FVS MFVS(Graph& G);
    FVS mfvs(Graph& G,vector<bool>& included, FVS& currentSol, FVS& bestSol, int level);
};


int BranchBoundNaive::upperBound(Graph &G, vector<bool>& included)
{
    auto sub = Subgraph(G, included).GetSubGraph().first;
    MakeMinimal M(sub);
    return M.Compute().second;
}

int BranchBoundNaive::lowerBound(Graph &G,  vector<bool>& included)
{
    auto sub = Subgraph(G, included).GetSubGraph().first;
    Tarjan T(sub);
    FVS fvs = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    for(auto scc : T.getSCC())
    {
        if(scc.size()>1)
        {
            fvs.first[scc[random()%scc.size()]]=true;
            fvs.second++;
        }
    }
    return fvs.second;
}

pair<Subgraph, vector<bool> > BranchBoundNaive::reduce(Graph &G, vector<bool>& included)
{
    vector<int> indegree(G.GetNumVertices(), 0);
    vector<int> outdegree(G.GetNumVertices(), 0);
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(included[v]) {
            for (NodeID u : G.neighbors(v)) {
                if(included[u]){
                    outdegree[v]++;
                    indegree[u]++;
                }
            }
        }
    }
    for(NodeID v = 0; v<G.GetNumVertices(); v++)
    {
        if(indegree[v]==0 && outdegree[v]==0)
        {
            included[v]=false;
        }
    }
}

BranchBoundNaive::BranchBoundNaive(Graph &G) {
    this->G = G;
}


FVS BranchBoundNaive::MFVS(Graph &G)
{
    FVS fvs_best = make_pair(vector<bool>(G.GetNumVertices(), true), G.GetNumVertices());
    FVS fvs_cur = make_pair(vector<bool>(G.GetNumVertices(), false), 0);
    vector<bool> included(G.GetNumVertices(), true);
    this->mfvs(G, included,fvs_cur, fvs_best, 0);
}

/*
 * TODO: implement a SCC function for a given included vector, s.t. we can compute the SCC for a given Graph but only
 * TODO: considering the vertices that are in the vector
 */

/*
 * We are not removing anything from the graph, we keeping a vector<bool> include with us, to
 * see which vertex is still considered for the computations
 */
FVS BranchBoundNaive::mfvs(Graph &G, vector<bool>& included, FVS &currentSol, FVS &bestSol, int level)
{
    Graph h = G;
    FVS partialSolution = make_pair(vector<bool>(h.GetNumVertices(), true), h.GetNumVertices());
    this->reduce(h, included);
    partialSolution.first=included;
    partialSolution.second=0;
    for(auto i : partialSolution.first)
    {
        partialSolution.second+=i;
    }
    for(NodeID it = 0; it<currentSol.first.size(); it++)
    {
        if(!currentSol.first[it] && partialSolution.first[it])
        {
            currentSol.first[it]=true;
            currentSol.second++;
        }
    }
    auto SCC = Tarjan(G).getSCC();
    if(SCC.size()>1)
    {
        for(const auto& scc : SCC)
        {
            if(scc.size()>1)
            {
                vector<bool> sccInc(h.GetNumVertices(), false);
                FVS fvs = make_pair(vector<bool>(h.GetNumVertices(), false), 0);
                for(auto it : scc)
                {
                    sccInc[it]=true;
                    fvs.first[it]=true;
                    fvs.second++;
                }
                for(auto it : included)
                {
                    sccInc[it]=true;
                }
                auto cur = make_pair(vector<bool>(h.GetNumVertices(), false), 0);
                auto sccSol = this->mfvs(h, sccInc, cur, fvs, 0);
                for(NodeID it =0; it<h.GetNumVertices(); it++)
                {
                    if(sccSol.first[it] && !currentSol.first[it])
                    {
                        currentSol.first[it]=true;
                        currentSol.second++;
                    }
                }
            }
        }
        return currentSol;
    }

    // Here comes the rest from the code...
}




#endif //BREAKINGCYCLES_BRANCHBOUNDNAIVE_H
