//
// Created by Mert Biyikli on 21.04.22.
//

#ifndef BREAKINGCYCLES_GRAPH_H
#define BREAKINGCYCLES_GRAPH_H

#include <vector>
#include <set>
#include <iostream>
#include <queue>
#include <stack>
#include <list>


using namespace std;
typedef int NodeID;
typedef pair<vector<bool>,NodeID> FVS;

class Edge{
public:
    int u, v;
    Edge(){this->u=-1; this->v=-1;};
};

class Graph{
    vector<vector<NodeID> > OutGoingVertices, InGoingVertices;
    set<NodeID> vertices;
    vector<bool> hasVertices;

    int _tarjanCurrentIndex;                   // The current index in the search
    vector<int> _tarjanIndex;                  // The search index of each vertex
    vector<int> _tarjanAncestor;               // The lowest ancestor of each vertex
    stack<int> _tarjanStack;                   // The stack used for Tarjan's algorithm
    vector<bool> _tarjanInStack;               // Tells if the vertex is in the stack
    int _currentComponent;                     // The current strongly connected component
    vector<int> _sccsByNum;                    // A vector associating each vertex with
    // its corresponding strongly connected
    // component
    vector< vector<int> > _sccs;               // The strongly connected components
    void _tarjan(int vertex, bool byNumber);
public:
    int numNodes, numEdges, currentMax, currentMin;
    void Init(const int&);

    // GetMethods
    int GetNumVertices() const;
    int GetNumEdges() const;
    bool hasVertex(NodeID v) const;
    bool hasEdge(NodeID u, NodeID v) const;
    set<NodeID>& GetVertices();
    vector<Edge> GetEdges();
    vector<NodeID>& InComingNodes(NodeID);
    vector<NodeID>& OutGoing(NodeID);
    Graph GetSubgraph(vector<NodeID>&);
    int Indegree(NodeID v) const;
    int OutDegree(NodeID v) const;
    int TotalDegree(NodeID v) const;
    pair<NodeID , int> MinIndegree() const;
    pair<NodeID , int> MaxIndegree() const;
    pair<NodeID , int> MinOutdegree() const;
    pair<NodeID , int> MaxOutdegree() const;
    pair<NodeID , int> MaxTotalDegree() const;
    pair<NodeID , int> MinTotalDegree() const;
    bool isSourceNode(NodeID v) const;
    bool isSinkNode(NodeID v) const;
    bool hasSelfLoop(NodeID v) const;
    vector<NodeID> GetSources();
    vector<NodeID> GetSinks();
    vector<NodeID> GetLoops();
    vector<vector<NodeID> > SCC();
    vector<NodeID> SCC_component();
    Graph GroundingKernel();
    vector<NodeID> shortestCycle();
    vector<int> _shortestCycle(int vertex, int maxLength);
    //vector<NodeID> shortestCycle();
    //vector<NodeID> _shortestCycle(NodeID v, int maxLength);
    bool isAcyclic_;
    bool isAcyclic();

    // These are for the reduction rules

    // GetMethods
    vector<NodeID> In0();
    vector<NodeID> Out0();
    vector<NodeID> In1();
    vector<NodeID> Out1();
    vector<NodeID> loops();

    /* Not sure, if you need those */
    vector<Edge> acyclicEdges();
    vector<Edge> piEdges();
    vector<Edge> pseudoAcyclicEdges();
    vector<NodeID> PieVertcices();
    bool isClique(vector<NodeID>& nodes);
    vector<NodeID> Core();
    vector<NodeID> Piedecessor(NodeID v);
    vector<NodeID> Piesuccessor(NodeID v);
    vector<NodeID> NonePiedecessor(NodeID v);
    vector<NodeID> NonePiesuccessor(NodeID v);
    bool isDominated(const Edge& e) const;
    vector<Edge> GetDominatedEdges();
    vector<vector<NodeID> > getstronglyconnectedcomponents();
    vector<NodeID> vertexToStronglyConnectedComponentNumber();

    // RemovalMethods
    vector<Edge> pie();
    bool isPiVertex(NodeID vertex);
    vector<Edge> dome();
    vector<NodeID> core();
    vector<int> piPredecessors(int vertex);
    vector<int> piSuccessors(int vertex);
    vector<int> nonPiPredecessors(int vertex);
    vector<NodeID> nonPiSuccessors(int vertex);
    bool isDominated(int source, int target);
    vector<Edge> dominatedEdges();


    vector<NodeID> reduce(bool, bool, bool, bool, bool, bool, bool, bool, bool);
    vector<NodeID> reduce(bool);

    void print();

    // modification methods
    void AddVertex(NodeID);
    void AddEdge(NodeID, NodeID v);
    void DeleteVertex(NodeID v);
    void DeleteVertices(vector<NodeID>& vec);
    void DeleteEdge(NodeID s, NodeID t);
    void DeleteEdges(vector<Edge>& e);
    void MergeVertex(NodeID& v);
    void MergeVertices(vector<NodeID>& vec);
};


void Graph::Init(const int& Number) {
    OutGoingVertices.resize(Number);
    InGoingVertices.resize(Number);
    hasVertices.resize(Number, false);
    currentMax=Number;
    numNodes=0;
    numEdges=0;
}

int Graph::GetNumVertices() const {
    return numNodes;
}

int Graph::GetNumEdges() const{
    return numEdges;
}

bool Graph::hasVertex(NodeID v) const {
    if (v < 0 || v >= currentMax) {
    return false;
}else {
        return hasVertices[v];
    }
}

bool Graph::hasEdge(NodeID u, NodeID v) const {
    if(!hasVertex(u) || !hasVertex(v))
    {
        return false;
    }else{
        return find(OutGoingVertices[u].begin(), OutGoingVertices[u].end(), v)
               != OutGoingVertices[u].end();
    }
}

set<NodeID> & Graph::GetVertices() {
    return vertices;
}

vector<Edge> Graph::GetEdges(){
    vector<Edge> edges_;
    for (int i = 0; i < currentMax; ++i) {
        for (vector<int>::const_iterator jt = OutGoingVertices[i].begin();
             jt != OutGoingVertices[i].end(); ++jt) {
            Edge e;
            e.u = i;
            e.v = *jt;
            edges_.push_back(e);
        }
    }
    return edges_;
}

vector<NodeID> & Graph::InComingNodes(NodeID v) {
    if (hasVertex(v)) return InGoingVertices[v];
    else throw "Vertex does not exist";
}

vector<NodeID> & Graph::OutGoing(NodeID v) {
    if(hasVertex(v)) {
        return OutGoingVertices[v];
    }else{
        throw "Vertex does not exist";
    }
}


//for debugging purposes only
void Graph::print() {
    for(auto v : vertices)
    {
        for(auto u : OutGoing(v))
        {
            cout << u << " ";
        }
        cout << endl;
    }
}


void Graph::AddVertex(NodeID v) {
    if (v >= 0 && v < currentMax && !hasVertex(v)) {
        vertices.insert(v);
        hasVertices[v] = true;
        ++numNodes;
    }
}

void Graph::AddEdge(NodeID s, NodeID t) {
    if (hasVertex(s) && hasVertex(t) && !hasEdge(s, t)) {
        OutGoingVertices[s].push_back(t);
        InGoingVertices[t].push_back(s);
        ++numEdges;
    }
}

void Graph::DeleteVertex(NodeID v) {
    if (hasVertex(v)) {
        vector<int> outN = OutGoing(v);
        vector<int> inN  = InComingNodes(v);
        for (int i = 0; i < int(outN.size()); ++i) {
            DeleteEdge(v, outN[i]);
        }
        for (int i = 0; i < int(inN.size());  ++i) {
            DeleteEdge(inN[i], v);
        }
        vertices.erase(v);
        hasVertices[v] = false;
        --numNodes;
    }
}

void Graph::DeleteEdge(NodeID s, NodeID t) {
    if (hasEdge(s, t)) {
        OutGoingVertices[s].erase(remove(OutGoingVertices[s].begin(),
                                                OutGoingVertices[s].end(), t));
        InGoingVertices[t].erase(remove(InGoingVertices[t].begin(),
                                                InGoingVertices[t].end(), s));
        --numEdges;
    }
}


void Graph::DeleteVertices(vector<NodeID> &vec) {
    for(auto& v : vec)
    {
        this->DeleteVertex(v);
    }
}

void Graph::DeleteEdges(vector<Edge> &edges) {
    for(auto& e : edges)
    {
        this->DeleteEdge(e.u, e.v);
    }
}

void Graph::MergeVertex(NodeID &v) {
    if (hasVertex(v)) {
        vector<int> inN = InComingNodes(v);
        vector<int> outN = OutGoing(v);
        for (vector<int>::iterator it = inN.begin(); it != inN.end(); ++it)
            for (vector<int>::iterator jt = outN.begin(); jt != outN.end(); ++jt)
                AddEdge(*it, *jt);
        DeleteVertex(v);
    } else throw "The vertex does not exist.";
}

void Graph::MergeVertices(vector<NodeID> &vec) {
    if (vec.size() > 0) {
        int v = vec[0];
        for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
            if (*it != v) {
                vector<int> inN = InComingNodes(*it);
                vector<int> outN = OutGoing(*it);
                vector<int>::iterator kt;
                for (vector<int>::iterator jt = inN.begin(); jt != inN.end(); ++jt) {
                    kt = find(vec.begin(), vec.end(), *jt);
                    if (*kt == v || kt == vec.end()) AddEdge(*jt, v);
                }
                for (vector<int>::iterator jt = outN.begin(); jt != outN.end(); ++jt) {
                    kt = find(vec.begin(), vec.end(), *jt);
                    if (*kt == v || kt == vec.end()) AddEdge(v, *jt);
                }
                DeleteVertex(*it);
            }
        }
    }
}

Graph Graph::GetSubgraph(vector<NodeID> &vs) {
    Graph h;
    h.Init(this->currentMax);
    for (vector<int>::iterator it = vs.begin(); it != vs.end(); ++it)
        h.AddVertex(*it);
    for (vector<int>::iterator it = vs.begin(); it != vs.end(); ++it) {
        if (hasVertex(*it)) {
            vector<int> outN = OutGoing(*it);
            for (vector<int>::iterator jt = outN.begin(); jt != outN.end(); ++jt)
                if (h.hasVertex(*jt)) h.AddEdge(*it, *jt);
        }
    }
    return h;
}


int Graph::Indegree(NodeID v) const {
    if(hasVertex(v))
    {
        return InGoingVertices[v].size();
    }else{
        //throw "There is no such vertex";
        return -1;
    }
}

int Graph::OutDegree(NodeID v) const {
    if(hasVertex(v))
    {
        return OutGoingVertices[v].size();
    } else{
        return -1;
    }
}

int Graph::TotalDegree(NodeID v) const {
    return Indegree(v)+OutDegree(v);
}

pair<NodeID , int> Graph::MaxIndegree() const {
    pair<NodeID, int> max(0,0);
    for(auto v : vertices)
    {
        if(hasVertex(v)) {
            if (Indegree(v) > max.second) {
                max.first = v;
                max.second = Indegree(v);
            }
        }
    }
    return max;
}

pair<NodeID, int> Graph::MinIndegree() const {
    pair<NodeID, int> min(0,numeric_limits<int>::max());
    for(auto v : vertices)
    {
        if(hasVertex(v)) {
            if (Indegree(v) < min.second) {
                min.first = v;
                min.second = Indegree(v);
            }
        }
    }
    return min;
}

pair<NodeID, int> Graph::MaxOutdegree() const {
    pair<NodeID, int> max(0,0);
    for(auto v : vertices)
    {
        if(hasVertex(v)) {
            if (OutDegree(v) > max.second) {
                max.first = v;
                max.second = Indegree(v);
            }
        }
    }
    return max;
}

pair<NodeID , int> Graph::MaxTotalDegree() const {
    pair<NodeID, int> max(0,0);
    for(auto v : vertices)
    {
        if(hasVertex(v)) {
            if (OutDegree(v)+Indegree(v) > max.second) {
                max.first = v;
                max.second = Indegree(v);
            }
        }
    }
    return max;
}

pair<NodeID , int> Graph::MinTotalDegree() const {
    pair<NodeID, int> min(0,numeric_limits<int>::max());
    for(auto v : vertices)
    {
        if(hasVertex(v)) {
            if (OutDegree(v)+Indegree(v) < min.second) {
                min.first = v;
                min.second = Indegree(v);
            }
        }
    }
    return min;
}

bool Graph::isSinkNode(NodeID v) const {
    if(hasVertex(v))
    {
        if(OutDegree(v)==0)
        {
            return true;
        }
    }
    return false;
}

bool Graph::isSourceNode(NodeID v) const {
    if(hasVertex(v))
    {
        if(Indegree(v)==0)
        {
            return true;
        }
    }
    return false;
}

vector<NodeID> Graph::GetSinks() {
    vector<NodeID> sks;
    for(NodeID v : vertices)
    {
        if(isSinkNode(v))
        {
            sks.push_back(v);
        }
    }
    return sks;
}

vector<NodeID> Graph::GetSources() {
    vector<NodeID> src;
    for(NodeID v : vertices)
    {
        if(isSourceNode(v)) {
            src.push_back(v);
        }
    }
    return src;
}

vector<NodeID> Graph::GetLoops() {
    vector<NodeID> lps;
    for(NodeID v : vertices)
    {
        if(hasSelfLoop(v))
        {
            lps.push_back(v);
        }
    }
    return lps;
}

bool Graph::hasSelfLoop(NodeID v) const {
    if (!hasVertex(v)) return false;
    else {
        vector<int> outN = OutGoingVertices[v];
        return find(outN.begin(), outN.end(), v) != outN.end();
    }
}


vector<NodeID> Graph::loops() {
    vector<int> loopVertices = GetLoops();
    DeleteVertices(loopVertices);
    return loopVertices;
}


Graph Graph::GroundingKernel() {
    Graph h(*this);
    vector<int> out0Vertices;
    do {
        out0Vertices = h.Out0();
        h.DeleteVertices(out0Vertices);
    } while (int(out0Vertices.size()) != 0);
    return h;
}

vector<NodeID> Graph::Out0() {
    vector<NodeID> Out_0 = GetSinks();
    DeleteVertices(Out_0);
    return Out_0;
}


vector<NodeID> Graph::In0() {
    vector<NodeID> In_0 = GetSources();
    DeleteVertices(In_0);
    return In_0;
}

vector<NodeID> Graph::Out1() {
    if (int(GetLoops().size()) == 0) {
        vector<int> out1Vertices;
        vector<int> vs(vertices.size());
        auto begin = vertices.begin();
        auto end = vertices.end();
        copy(begin, end, vs.begin());
        for (vector<int>::iterator it = vs.begin(); it != vs.end(); ++it) {
            if (hasVertex(*it) && OutDegree(*it) == 1 && !hasSelfLoop(*it)) {
                out1Vertices.push_back(*it);
                MergeVertex(*it);
            }
        }
        return out1Vertices;
    } else
        return vector<NodeID>();
}

vector<NodeID> Graph::In1() {
    if (int(GetLoops().size()) == 0) {
        vector<int> in1Vertices = vector<int>();
        vector<int> vs(int(vertices.size()));
        copy(vertices.begin(), vertices.end(), vs.begin());
        for (vector<int>::iterator it = vs.begin(); it != vs.end(); ++it) {
            if (hasVertex(*it) && Indegree(*it) == 1 && !hasSelfLoop(*it)) {
                in1Vertices.push_back(*it);
                MergeVertex(*it);
            }
        }
        return in1Vertices;
    } else
        return vector<int>();
}


bool Graph::isPiVertex(int vertex) {
    vector<int> outN = OutGoing(vertex);
    bool valid = true;
    vector<int>::iterator it = outN.begin();
    while (valid && it != outN.end()) {
        valid = hasEdge(*it, vertex);
        ++it;
    }
    return valid;
}

vector<int> Graph::PieVertcices() {
    vector<int> vs;
    for (set<int>::iterator it = GetVertices().begin(); it != GetVertices().end(); ++it) {
        if (isPiVertex(*it)) vs.push_back(*it);
    }
    return vs;
}

vector<NodeID> Graph::core() {
    if (int(loops().size()) == 0) {
        vector<int> piVs = PieVertcices();
        for (vector<int>::iterator it = piVs.begin(); it != piVs.end(); ++it) {
            vector<int> potentialClique = OutGoing(*it);
            potentialClique.push_back(*it);
            if (isClique(potentialClique)) {
                DeleteVertices(potentialClique);
                potentialClique.pop_back();
                return potentialClique;
            }
        }
    }
    return vector<int>();
}

vector<int> Graph::piPredecessors(int vertex) {
    vector<int> piPreds;
    vector<int> preds = InComingNodes(vertex);
    for (vector<int>::iterator it = preds.begin(); it != preds.end(); ++it)
        if (hasEdge(vertex, *it)) piPreds.push_back(*it);
    return piPreds;
}

vector<int> Graph::piSuccessors(int vertex) {
    vector<int> piSucc;
    vector<int> succ = OutGoing(vertex);
    for (vector<int>::iterator it = succ.begin(); it != succ.end(); ++it)
        if (hasEdge(*it, vertex)) piSucc.push_back(*it);
    return piSucc;
}

vector<int> Graph::nonPiPredecessors(int vertex)  {
    vector<int> nonPiPreds;
    vector<int> preds = InComingNodes(vertex);
    for (vector<int>::iterator it = preds.begin(); it != preds.end(); ++it)
        if (!hasEdge(vertex, *it)) nonPiPreds.push_back(*it);
    return nonPiPreds;
}

vector<int> Graph::nonPiSuccessors(int vertex) {
    vector<int> nonPiSucc;
    vector<int> succ = OutGoing(vertex);
    for (vector<int>::iterator it = succ.begin(); it != succ.end(); ++it)
        if (!hasEdge(*it, vertex)) nonPiSucc.push_back(*it);
    return nonPiSucc;
}

bool Graph::isDominated(int source, int target)  {
    if (!hasEdge(source, target)) return false;
    else if (hasEdge(source, target) && hasEdge(target, source)) return false;
    else {
        vector<int> pTarget = InComingNodes(target);
        vector<int> pSource = nonPiPredecessors(source);
        sort(pTarget.begin(), pTarget.end());
        sort(pSource.begin(), pSource.end());
        if (includes(pTarget.begin(), pTarget.end(), pSource.begin(), pSource.end()))
            return true;
        vector<int> sSource = OutGoing(source);
        vector<int> sTarget = nonPiSuccessors(target);
        sort(sSource.begin(), sSource.end());
        sort(sTarget.begin(), sTarget.end());
        if (includes(sSource.begin(), sSource.end(), sTarget.begin(), sTarget.end()))
            return true;
        else
            return false;
    }
}

vector<Edge> Graph::dominatedEdges()  {
    vector<Edge> allEdges = GetEdges();
    vector<Edge> des;
    for (vector<Edge>::iterator it = allEdges.begin(); it != allEdges.end(); ++it)
        if (isDominated(it->u, it->v)) des.push_back(*it);
    return des;
}

vector<Edge> Graph::dome() {
    if (int(loops().size()) == 0) {
        vector<Edge> des = dominatedEdges();
        DeleteEdges(des);
        return des;
    } else
        return vector<Edge>();
}

vector<Edge> Graph::pie() {
    if (int(loops().size()) == 0) {
        vector<Edge> aes = acyclicEdges();
        vector<Edge> paes = pseudoAcyclicEdges();
        DeleteEdges(aes);
        DeleteEdges(paes);
        vector<Edge> es(aes);
        for (vector<Edge>::iterator it = paes.begin(); it != paes.end(); ++it) {
            es.push_back(*it);
        }
        return es;
    } else
        return vector<Edge>();
}


vector<Edge> Graph::acyclicEdges() {
    vector<Edge> ae;
    vector<Edge> allEdges = this->GetEdges();
    vector<int> vertexToSCC = vertexToStronglyConnectedComponentNumber();
    for (vector<Edge>::iterator it = allEdges.begin(); it != allEdges.end(); ++it) {
        if (vertexToSCC[it->u] != vertexToSCC[it->v]) {
            ae.push_back(*it);
        }
    }
    return ae;
}

vector<Edge> Graph::pseudoAcyclicEdges() {
    Graph h(*this);
    vector<Edge> pies = piEdges();
    auto acyc = acyclicEdges();
    h.DeleteEdges(acyc);
    h.DeleteEdges(pies);
    return h.acyclicEdges();
}

vector<Edge> Graph::piEdges() {
    vector<Edge> es;
    vector<Edge> allEdges = GetEdges();
    for (vector<Edge>::iterator it = allEdges.begin(); it != allEdges.end(); ++it) {
        if (hasEdge(it->u, it->v)) {
            es.push_back(*it);
        }
    }
    return es;
}


bool Graph::isClique(vector<NodeID> &nodes) {
    bool clique = true;
    vector<int>::iterator it = nodes.begin();
    while (clique && it != nodes.end()){
        vector<int>::iterator jt = nodes.begin();
        while (clique && jt != nodes.end()){
            clique = (*it == *jt && !hasEdge(*it, *jt)) || (*it != *jt && hasEdge(*it, *jt));
            ++jt;
        }
        ++it;
    }
    return clique;
}

vector<NodeID> Graph::reduce(bool print=false) {
    int n, m;
    vector<int> solution;
    do {
        n = GetNumVertices();
        m = GetNumEdges();
        vector<int> partialSolution = reduce(true, true, true, true, true,
                                             false, false, true, print);
        for (vector<int>::iterator it = partialSolution.begin();
             it != partialSolution.end(); ++it)
            solution.push_back(*it);
    } while (n != GetNumVertices() || m != GetNumEdges());
    return solution;
}

vector<NodeID> Graph::reduce(bool out0, bool in0, bool out1, bool in1, bool loop, bool applyPie, bool applyCore, bool applyDome, bool print) {
    vector<int> solution;
    if (in0) {
        vector<int> in0Vertices = In0();
        if (print) cout << "IN0  : " << int(in0Vertices.size())
                          << " vertex(vertices) has(have) been removed." << endl;
    }
    if (out0) {
        vector<int> out0Vertices = Out0();
        if (print) cout << "OUT0 : " << int(out0Vertices.size())
                          << " vertex(vertices) has(have) been removed." << endl;
    }
    if (loop) {
        vector<int> loopVertices = GetLoops();
        for (vector<int>::iterator it = loopVertices.begin(); it != loopVertices.end(); ++it)
            solution.push_back(*it);
        if (print) cout << "LOOP : " << int(loopVertices.size())
                          << " vertex(vertices) has(have) been removed." << endl;
    }
    if (in1) {
        vector<int> in1Vertices = In1();
        if (print) cout << "IN1  : " << int(in1Vertices.size())
                          << " vertex(vertices) has(have) been merged." << endl;
    }
    if (out1) {
        vector<int> out1Vertices = Out1();
        if (print) cout << "OUT1 : " << int(out1Vertices.size())
                          << " vertex(vertices) has(have) been merged." << endl;
    }
    if (applyPie) {
        vector<Edge> pieEdges = pie();
        if (print) cout << "PIE  : " << int(pieEdges.size())
                          << " edge(s) has(have) been removed." << endl;
    }
    if (applyCore) {
        vector<int> coreVertices = core();
        for (vector<int>::iterator it = coreVertices.begin(); it != coreVertices.end(); ++it)
            solution.push_back(*it);
        if (print) cout << "CORE : " << int(coreVertices.size())
                          << " vertex(vertices) has(have) been removed." << endl;
    }
    if (applyDome) {
        vector<Edge> domeEdges = dome();
        if (print) cout << "DOME : " << int(domeEdges.size())
                          << " edge(s) has(have) been removed." << endl;
    }
    return solution;
}


vector<vector<NodeID> > Graph::getstronglyconnectedcomponents() {
    _tarjanCurrentIndex = 0;
    _tarjanIndex = vector<int>(currentMax);
    _tarjanAncestor = vector<int>(currentMax);
    _tarjanStack = stack<int>();
    _tarjanInStack = vector<bool>(currentMax);
    _sccs = vector< vector<int> >();
    for (int i = 0; i < currentMax; ++i) {
        _tarjanIndex[i] = -1;
        _tarjanInStack[i] = false;
    }
    for (set<int>::iterator it = GetVertices().begin(); it != GetVertices().end(); ++it) {
        if (_tarjanIndex[*it] == -1) _tarjan(*it, false);
    }
    return _sccs;
}

vector<NodeID> Graph::vertexToStronglyConnectedComponentNumber() {
    _tarjanCurrentIndex = 0;
    _tarjanIndex = vector<int>(currentMax);
    _tarjanAncestor = vector<int>(currentMax);
    _tarjanStack = stack<int>();
    _tarjanInStack = vector<bool>(currentMax);
    _currentComponent = 0;
    _sccsByNum = vector<int>(currentMax);
    for (int i = 0; i < currentMax; ++i) {
        _tarjanIndex[i] = -1;
        _tarjanInStack[i] = false;
    }
    for (set<int>::iterator it = GetVertices().begin(); it != GetVertices().end(); ++it) {
        if (_tarjanIndex[*it] == -1) _tarjan(*it, true);
    }
    return _sccsByNum;
}


void Graph::_tarjan(int vertex, bool byNumber) {
    _tarjanIndex[vertex] = _tarjanCurrentIndex;
    _tarjanAncestor[vertex] = _tarjanCurrentIndex;
    ++_tarjanCurrentIndex;
    _tarjanStack.push(vertex);
    _tarjanInStack[vertex] = true;
    vector<int> outN = OutGoing(vertex);
    for (vector<int>::iterator it = outN.begin(); it != outN.end(); ++it) {
        if (_tarjanIndex[*it] == -1) {
            _tarjan(*it, byNumber);
            _tarjanAncestor[vertex] = min(_tarjanAncestor[vertex], _tarjanAncestor[*it]);
        } else if (_tarjanInStack[*it]) {
            _tarjanAncestor[vertex] = min(_tarjanAncestor[vertex], _tarjanIndex[*it]);
        }
    }
    if (_tarjanAncestor[vertex] == _tarjanIndex[vertex]) {
        if (byNumber) {
            int u;
            do {
                u = _tarjanStack.top();
                _sccsByNum[u] = _currentComponent;
                _tarjanStack.pop();
                _tarjanInStack[u] = false;
            } while (u != vertex);
            ++_currentComponent;
        } else {
            vector<int> scc = vector<int>();
            int u;
            do {
                u = _tarjanStack.top();
                scc.push_back(u);
                _tarjanStack.pop();
                _tarjanInStack[u] = false;
            } while (u != vertex);
            _sccs.push_back(scc);
        }
    }
}

bool Graph::isAcyclic() {
    return int(GetLoops().size()) == 0 &&
           int(getstronglyconnectedcomponents().size()) == GetNumVertices();
}


vector<NodeID> Graph::shortestCycle() {
    Graph h(*this);
    if (h.isAcyclic()) {
        throw "No shortest cycle ! This graph is acyclic";
    } else {
        int n;
        do {
            n = h.GetNumVertices();
            h.In0();
            h.Out0();
        } while (n != h.GetNumVertices());
        vector<int> shortestCycle;
        vector<int> currentCycle;
        int maxLength = h.GetNumVertices();
        for (set<int>::iterator it = GetVertices().begin(); it != GetVertices().end(); ++it) {
            if(h.hasVertex(*it))
            {
                currentCycle = h._shortestCycle(*it, maxLength);
                if (int(currentCycle.size() - 1)  < maxLength) {
                    shortestCycle = currentCycle;
                    maxLength = int(currentCycle.size()) - 1;
                }
            }else{
                continue;
            }
        }
        return shortestCycle;
    }
}

vector<int> Graph::_shortestCycle(int vertex, int maxLength) {
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
            vector<int> outN = OutGoing(path.back());
            for (auto it = outN.begin();
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


bool isAcyclic(Graph& graph, const std::vector<bool>& fvs) {
    int fvs_size = 0;
    for ( size_t i = 0; i < fvs.size(); ++i ) {
        fvs_size += fvs[i];
    }

    std::vector<int> in_degree(graph.GetNumVertices(), 0);
    for ( int u = 0; u < graph.GetNumVertices(); u++ ) {
        if ( !fvs[u] ) {
            for ( const int v : graph.OutGoing(u) ) {
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

        for ( const int v : graph.OutGoing(u) ) {
            --in_degree[v];
            if ( !fvs[v] && in_degree[v] == 0 ) {
                q.push(v);
            }
        }
    }

    return visited_nodes == (graph.GetNumVertices() - fvs_size);
}
#endif //BREAKINGCYCLES_GRAPH_H
